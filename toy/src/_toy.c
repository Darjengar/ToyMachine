/* Author : Darjengar 08/23/2021, License: GPL 3.0 */
/* written in ANSI C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toy.h"

#include <Python.h>

#define START_OF_PROGRAM 0x10
#define MEM_SIZE 256
#define NUM_REGS 16

enum boolean {FALSE, TRUE};

unsigned short mem[256];
unsigned short regs[16];
unsigned char pc;
int eof;

char *pad8(unsigned char num);
char *pad16(unsigned short num);
char *show_mem(unsigned short mem[], int size);
char *show_regs(unsigned short regs[], int size);
void test();
//void init_toy(char *program);
void load_program(const char *program);
int exec_instr();
void scan_opcode1(unsigned char byte1, unsigned char byte2);
void scan_opcode2(unsigned char byte1, unsigned char byte2);
void scan_opcode12(unsigned char byte1, unsigned char byte2);

struct {
    unsigned int halt_flag : 1;
    unsigned int input_flag : 1;
    unsigned int output_flag : 1;
    unsigned int jmp_flag : 1;
} toyflags;

/*struct {
    unsigned int opcode : 4;
    unsigned int src : 4;
    unsigned int dst : 4;
    unsigned int src1: 4;
    unsigned int src2 : 4;
} curr_instr;*/

struct {
    unsigned char opcode;
    union {
        unsigned char src;
        unsigned char dst;
    } u1;
    union {
        unsigned char addr;
        struct {
            unsigned char src1;
            unsigned char src2;
        } src;
    } u2;
} curr_instr;


static PyObject *method_init_toy(PyObject *self, PyObject *args)
{
    const char *program;
    char *result_str = malloc(20000);

    if (!PyArg_ParseTuple(args, "s", &program)) {
        return NULL;
    }

    regs[0] = 0;
    pc = START_OF_PROGRAM;
    toyflags.halt_flag = 0;
    toyflags.input_flag = 0;
    toyflags.output_flag = 0;
    toyflags.jmp_flag = 0;
    mem[255] = 0;
    printf("Load program into ram\n");
    load_program(program);

    strcpy(result_str, show_regs(regs, 16));
    strcat(result_str, show_mem(mem, 256));

    PyObject *py_result_str = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result_str;
}

static PyObject *method_start_toy(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    /* Fetch-Execute */
    curr_instr.opcode = (mem[pc] & 0xF000) >> 12;
    regs[0] = 0;
    toyflags.jmp_flag = 0;
    if (exec_instr() != 0) {
        return NULL;
    }

    if (toyflags.halt_flag == 0 && toyflags.jmp_flag == 0) {
        pc++;
    }

    //PyObject *py_result_str = PyUnicode_FromString(result_str);
    //free(result_str);
    return Py_BuildValue("iiiii", toyflags.halt_flag, toyflags.input_flag, toyflags.output_flag, toyflags.jmp_flag, pc);
    //return 0;
}

static PyObject *method_restart_toy(PyObject *self, PyObject *args)
{
    char *result_str = malloc(20000);

    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    pc = START_OF_PROGRAM;
    toyflags.halt_flag = 0;
    toyflags.input_flag = 0;
    toyflags.output_flag = 0;
    mem[255] = 0;
    for (int iii = 0; iii < NUM_REGS; iii++) {
        regs[iii] = 0;
    }

    strcpy(result_str, show_regs(regs, 16));
    strcat(result_str, show_mem(mem, 256));

    PyObject *py_result_str = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result_str;
}

static PyObject *method_show_state(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    char *result_str = malloc(20000);

    strcpy(result_str, show_regs(regs, 16));
    strcat(result_str, show_mem(mem, 256));

    PyObject *py_result_str = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result_str;
}

static PyObject *method_stop_toy(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    pc = START_OF_PROGRAM;
    toyflags.halt_flag = 0;
    toyflags.input_flag = 0;
    toyflags.output_flag = 0;
    toyflags.jmp_flag = 0;
    mem[255] = 0;
    for (int iii = 0; iii < NUM_REGS; iii++) {
        regs[iii] = 0;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *method_reset_toy(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    pc = START_OF_PROGRAM;
    toyflags.halt_flag = 0;
    toyflags.input_flag = 0;
    toyflags.output_flag = 0;
    toyflags.jmp_flag = 0;
    for (int iii = 0; iii < MEM_SIZE; iii++) {
        mem[iii] = 0;
    }
    for (int iii = 0; iii < NUM_REGS; iii++) {
        regs[iii] = 0;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *method_toydisasm(PyObject *self, PyObject *args)
{
    char *result_str = malloc(5000);
    strcpy(result_str, "");
    char *tmp = malloc(50);
    PyObject *py_result_str;

    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    unsigned char byte1, byte2;

    int iii = START_OF_PROGRAM;
    while(iii != eof) {
        byte1 = (mem[iii] & 0xFF00) >> 8;
        byte2 = mem[iii] & 0x00FF;

        curr_instr.opcode = (byte1 & 0xF0) >> 4;
        switch(curr_instr.opcode) {
        case 0x0:
            sprintf(tmp, "%d | halt\n", iii+1 - START_OF_PROGRAM);
            strcat(result_str, tmp);
            break;
        case 0x1:
            scan_opcode1(byte1, byte2);
            sprintf(tmp, "%d | add R%X, R%X, R%X\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, curr_instr.u2.src.src1, curr_instr.u2.src.src2);
            strcat(result_str, tmp);
            break;
        case 0x2:
            scan_opcode1(byte1, byte2);
            sprintf(tmp, "%d | sub R%X, R%X, R%X\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, curr_instr.u2.src.src1, curr_instr.u2.src.src2);
            strcat(result_str, tmp);
            break;
        case 0x3:
            scan_opcode1(byte1, byte2);
            sprintf(tmp, "%d | and R%X, R%X, R%X\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, curr_instr.u2.src.src1, curr_instr.u2.src.src2);
            strcat(result_str, tmp);
            break;
        case 0x4:
            scan_opcode1(byte1, byte2);
            sprintf(tmp, "%d | xor R%X, R%X, R%X\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, curr_instr.u2.src.src1, curr_instr.u2.src.src2);
            strcat(result_str, tmp);
            break;
        case 0x5:
            scan_opcode1(byte1, byte2);
            sprintf(tmp, "%d | shl R%X, R%X, R%X\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, curr_instr.u2.src.src1, curr_instr.u2.src.src2);
            strcat(result_str, tmp);
            break;
        case 0x6:
            scan_opcode1(byte1, byte2);
            sprintf(tmp, "%d | shr R%X, R%X, R%X\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, curr_instr.u2.src.src1, curr_instr.u2.src.src2);
            strcat(result_str, tmp);
            break;
        case 0x7:
            scan_opcode2(byte1, byte2);
            sprintf(tmp, "%d | ldaddr R%X, 0x%s\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, pad8(curr_instr.u2.addr));
            strcat(result_str, tmp);
            break;
        case 0x8:
            scan_opcode2(byte1, byte2);
            sprintf(tmp, "%d | ld R%X, [0x%s]\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, pad8(curr_instr.u2.addr));
            strcat(result_str, tmp);
            break;
        case 0x9:
            scan_opcode2(byte1, byte2);
            sprintf(tmp, "%d | str R%X, [0x%s]\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, pad8(curr_instr.u2.addr));
            strcat(result_str, tmp);
            break;
        case 0xA:
            scan_opcode12(byte1, byte2);
            sprintf(tmp, "%d | ldi R%X, [R%X]\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, curr_instr.u2.src.src2);
            strcat(result_str, tmp);
            break;
        case 0xB:
            scan_opcode12(byte1, byte2);
            sprintf(tmp, "%d | stri R%X, [R%X]\n", iii+1 - START_OF_PROGRAM,  curr_instr.u1.dst, curr_instr.u2.src.src2);
            strcat(result_str, tmp);
            break;
        case 0xC:
            scan_opcode2(byte1, byte2);
            sprintf(tmp, "%d | bz R%X, 0x%s\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, pad8(curr_instr.u2.addr));
            strcat(result_str, tmp);
            break;
        case 0xD:
            scan_opcode2(byte1, byte2);
            sprintf(tmp, "%d | bp R%X, 0x%s\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, pad8(curr_instr.u2.addr));
            strcat(result_str, tmp);
            break;
        case 0xE:
            curr_instr.u1.dst = byte1 & 0x0F;
            sprintf(tmp, "%d | jmpreg [R%X]\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst);
            strcat(result_str, tmp);
            break;
        case 0xF:
            scan_opcode2(byte1, byte2);
            sprintf(tmp, "%d | jmpl [R%X], 0x%s\n", iii+1 - START_OF_PROGRAM, curr_instr.u1.dst, pad8(curr_instr.u2.addr));
            strcat(result_str, tmp);
            break;
        }
        iii += 1;
    }
    py_result_str = PyUnicode_FromString(result_str);
    free(result_str);
    free(tmp);
    return py_result_str;
}

static PyObject *method_input_toy(PyObject *self, PyObject *args)
{
    int value;
    if (!PyArg_ParseTuple(args, "i", &value)) {
        return NULL;
    }

    mem[255] = value;
    regs[curr_instr.u1.dst] = mem[255];
    toyflags.input_flag = 0;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *method_output_toy(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }
    toyflags.output_flag = 0;
    return PyLong_FromLong(regs[curr_instr.u1.src]);
}

static PyMethodDef ToyMethods[] = {
    {"init_toy", method_init_toy, METH_VARARGS,
     "Toy Machine initialization function"},
    {"start_toy", method_start_toy, METH_VARARGS,
     "Toy Machine start function"},
    {"toydisasm", method_toydisasm, METH_VARARGS,
     "Toy language disassembler"},
    {"reset_toy", method_reset_toy, METH_VARARGS,
     "Toy Machine reset function"},
    {"stop_toy", method_stop_toy, METH_VARARGS,
     "Toy Machine stop function"},
    {"restart_toy", method_restart_toy, METH_VARARGS,
     "Toy Machine restart function"},
    {"show_state", method_show_state, METH_VARARGS,
     "Show state of Toy Machine"},
    {"input_toy", method_input_toy, METH_VARARGS,
     "Toy Machine input function"},
    {"output_toy", method_output_toy, METH_VARARGS,
     "Toy Machine output function"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef toymodule = {
    PyModuleDef_HEAD_INIT,
    "toy",
    NULL,
    -1,
    ToyMethods
};

PyMODINIT_FUNC PyInit_toy(void)
{
    return PyModule_Create(&toymodule);
}

void load_program(const char* program)
{
    int byte1, byte2;
    FILE *fp = fopen(program, "rb");

    /*int prev = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fseek(fp, prev, SEEK_SET);*/
    /*fread(mem, size, 1, fp);*/

    int iii = 0;
    while((byte1 = getc(fp)) != EOF && (byte2 = getc(fp)) != EOF) {
        mem[iii++] = (byte1 << 8) | byte2;
    }
    eof = iii;
    fclose(fp);
}

void scan_opcode1(unsigned char byte1, unsigned char byte2)
{
    curr_instr.u1.dst = byte1 & 0x0F;
    curr_instr.u2.src.src1 = (byte2 & 0xF0) >> 4;
    curr_instr.u2.src.src2 = byte2 & 0x0F;
}

void scan_opcode2(unsigned char byte1, unsigned char byte2)
{
    curr_instr.u1.dst = byte1 & 0x0F;
    curr_instr.u2.addr = byte2 & 0xFF;
}

void scan_opcode12(unsigned char byte1, unsigned char byte2)
{
    curr_instr.u1.dst = byte1 & 0x0F;
    curr_instr.u2.src.src2 = byte2 & 0x0F;
}

int exec_instr()
{
    switch (curr_instr.opcode) {
        /* halt */
        case 0x0:
            printf("Halt TOY machine.\n");
            toyflags.halt_flag = 1;
        /* add */
        case 0x1:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.src.src1 = (mem[pc] & 0x00F0) >> 4;
            curr_instr.u2.src.src2 = mem[pc] & 0x000F;
            regs[curr_instr.u1.dst] = regs[curr_instr.u2.src.src1] + regs[curr_instr.u2.src.src2];
            break;
        /* subtract */
        case 0x2:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.src.src1 = (mem[pc] & 0x00F0) >> 4;
            curr_instr.u2.src.src2 = mem[pc] & 0x000F;
            regs[curr_instr.u1.dst] = regs[curr_instr.u2.src.src1] - regs[curr_instr.u2.src.src2];
            break;
        /* and */
        case 0x3:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.src.src1 = (mem[pc] & 0x00F0) >> 4;
            curr_instr.u2.src.src2 = mem[pc] & 0x000F;
            regs[curr_instr.u1.dst] = regs[curr_instr.u2.src.src1] & regs[curr_instr.u2.src.src2];
            break;
        /* xor */
        case 0x4:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.src.src1 = (mem[pc] & 0x00F0) >> 4;
            curr_instr.u2.src.src2 = mem[pc] & 0x000F;
            regs[curr_instr.u1.dst] = regs[curr_instr.u2.src.src1] ^ regs[curr_instr.u2.src.src2];
            break;
        /* shift left */
        case 0x5:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.src.src1 = (mem[pc] & 0x00F0) >> 4;
            curr_instr.u2.src.src2 = mem[pc] & 0x000F;
            regs[curr_instr.u1.dst] = regs[curr_instr.u2.src.src1] << regs[curr_instr.u2.src.src2];
            break;
        /* shift right */
        case 0x6:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.src.src1 = (mem[pc] & 0x00F0) >> 4;
            curr_instr.u2.src.src2 = mem[pc] & 0x000F;
            regs[curr_instr.u1.dst] = regs[curr_instr.u2.src.src1] >> regs[curr_instr.u2.src.src2];
            break;
        /* load addr */
        case 0x7:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.addr = mem[pc] & 0x00FF;
            regs[curr_instr.u1.dst] = curr_instr.u2.addr;
            break;
        /* load */
        case 0x8:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.addr = mem[pc] & 0x00FF;
            if (curr_instr.u2.addr == 0xFF) {
                toyflags.input_flag = 1;
            }
            else {
                regs[curr_instr.u1.dst] = mem[curr_instr.u2.addr];
            }
            break;
        /* store */
        case 0x9:
            curr_instr.u1.src = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.addr = mem[pc] & 0x00FF;
            if (curr_instr.u2.addr == 0xFF) {
                toyflags.output_flag = 1;
            }
            else {
                mem[curr_instr.u2.addr] = regs[curr_instr.u1.src];
            }
            break;
        /* load indirect */
        case 0xA:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.src.src2 = mem[pc] & 0x000F;
            regs[curr_instr.u1.dst] = mem[regs[curr_instr.u2.src.src2]];
            break;
        /* store indirect */
        case 0xB:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.src.src2 = mem[pc] & 0x000F;
            mem[regs[curr_instr.u2.src.src2]] = regs[curr_instr.u1.dst];
            break;
        /* branch zero */
        case 0xC:
            curr_instr.u1.src = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.addr = mem[pc] & 0x00FF;
            if (regs[curr_instr.u1.src] == 0) {
                pc = curr_instr.u2.addr;
                toyflags.jmp_flag = 1;
            }
            break;
        /* branch positive */
        case 0xD:
            curr_instr.u1.src = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.addr = mem[pc] & 0x00FF;
            if (regs[curr_instr.u1.src] > 0) {
                pc = curr_instr.u2.addr;
                toyflags.jmp_flag = 1;
            }
            break;
        /* jump register */
        case 0xE:
            curr_instr.u1.src = (mem[pc] & 0x0F00) >> 8;
            pc = regs[curr_instr.u1.src];
            toyflags.jmp_flag = 1;
            break;
        /* jump and link */
        case 0xF:
            curr_instr.u1.dst = (mem[pc] & 0x0F00) >> 8;
            curr_instr.u2.addr = mem[pc] & 0x00FF;
            regs[curr_instr.u1.dst] = pc;
            pc = curr_instr.u2.addr;
            toyflags.jmp_flag = 1;
            break;
    }
    return 0;
}

void test()
{
    printf("%X\n", 255 + 2);
}

char *pad8(unsigned char num)
{
    char *result = malloc(10);
    if (num <= 15) {
        sprintf(result, "0%X", num);
    }
    else if (num <= 255) {
        sprintf(result, "%X", num);
    }
    return result;
}

char *pad16(unsigned short num)
{
    char *result = malloc(10);
    if (num <= 15) {
        sprintf(result, "000%X", num);
    }
    else if (num <= 255) {
        sprintf(result, "00%X", num);
    }
    else if (num <= 4095) {
        sprintf(result, "0%X", num);
    }
    else {
        sprintf(result, "%X", num);
    }
    return result;
}

char *show_mem(unsigned short mem[], int size)
{
    char *mem_str = malloc(50);
    char *out_str = malloc(10000);
    strcpy(out_str, "----------------------------------------------\n");
    strcat(out_str, "| Memory                                     |\n");
    strcat(out_str, "----------------------------------------------\n");
    char *temp;
    temp = pad16(0);
    sprintf(mem_str, "%s | ", temp);
    strcat(out_str, mem_str);
    free(temp);
    for (int iii = 0; iii < size; iii++) {
        if (iii+1 == size) {
            /*printf("%s ", pad8((mem[iii] & 0xFF00)
                            >> 8));*/
            temp = pad16(mem[iii]);
            sprintf(mem_str, "%s\n", temp);
            strcat(out_str, mem_str);
            free(temp);
        }
        else if ((iii+1) % 8 == 0) {
            temp = pad16(mem[iii]);
            sprintf(mem_str, "%s\n", temp);
            strcat(out_str, mem_str);
            free(temp);
            temp = pad16(iii+1);
            sprintf(mem_str, "%s | ", temp);
            strcat(out_str, mem_str);
            free(temp);
        }
        else {
            temp = pad16(mem[iii]);
            sprintf(mem_str, "%s ", temp);
            strcat(out_str, mem_str);
            free(temp);
        }
    }
    strcat(out_str, "----------------------------------------------");
    free(mem_str);
    return out_str;
}

char *show_regs(unsigned short regs[], int size)
{
    char *out_str = malloc(10000);
    char *temp = malloc(50);
    strcpy(out_str, "---------------------------------------------------\n");
    strcat(out_str, "| Registers                                       |\n");
    strcat(out_str, "---------------------------------------------------\n");
    for (int iii = 0; iii < size; iii++) {
        if (iii + 1 == size) {
            sprintf(temp, "R%X = 0x%s\n", iii, pad16(regs[iii]));
            strcat(out_str, temp);
            strcat(out_str, "---------------------------------------------------"
                "\n");
            strcat(out_str, "| Program Counter                                 "
                "|\n");
            strcat(out_str, "---------------------------------------------------"
                "\n");
            sprintf(temp, "PC = 0x%s\n", pad8(pc));
            strcat(out_str, temp);
            strcat(out_str, "---------------------------------------------------"
                "\n");
        }
        else if ((iii+1) % 4 == 0) {
            sprintf(temp, "R%X = 0x%s, ", iii, pad16(regs[iii]));
            strcat(temp, "\n");
            strcat(out_str, temp);
        }
        else {
            sprintf(temp, "R%X = 0x%s, ", iii, pad16(regs[iii]));
            strcat(out_str, temp);
        }
    }
    free(temp);
    return out_str;
}
