/*  License: GPL 3.0, written in C99 + CPython */
/**
 * @file _toy.c
 * @author Darjengar
 * @brief TOY machines implementation.
 * @version 0.1
 * @date 2021-08-23
 * 
 * @copyright Copyright (c) 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Python.h>

#include "toy.h"
#include "toypy.h"
#include "util.h"

ToyFlags flags;
ToyInstr curr_instr;
ToyRam ram;
ToyProgram curr_program;
ToyState curr_state = {.instr_p = &curr_instr, .flags_p = &flags, .program_p = &curr_program, .ram_p = &ram};
ToyState states[MAX_NUM_STATES];

static PyObject *method_init_toy(PyObject *self, PyObject *args)
{
    char *result_str = malloc(20000);

    if (!PyArg_ParseTuple(args, "s", &curr_program.filename)) {
        return NULL;
    }

    curr_state.regs[0] = 0;
    curr_state.pc = MEM_PROG_START;
    curr_state.flags_p->halt_flag = 0;
    curr_state.flags_p->input_flag = 0;
    curr_state.flags_p->output_flag = 0;
    curr_state.flags_p->jmp_flag = 0;
    curr_state.ram_p->io = 0;

    load_program(&curr_state);
    result_str = show_state(&curr_state);

    PyObject *py_result_str = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result_str;
}

static PyObject *method_start_toy(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    ToyInstr *instr_p = curr_state.instr_p;
    ToyRam *ram_p = curr_state.ram_p;
    ToyFlags *flags_p = curr_state.flags_p;

    /* Fetch-Execute */
    instr_p->opcode = (ram_p->prog_mem[curr_state.pc - MEM_PROG_START] & 0xF000) >> 12;
    curr_state.regs[0] = 0;
    flags_p->jmp_flag = 0;
    exec_instr(&curr_state);

    if (flags_p->halt_flag == 0 && flags_p->jmp_flag == 0) {
        curr_state.pc++;
    }

    //PyObject *py_result_str = PyUnicode_FromString(result_str);
    //free(result_str);
    return Py_BuildValue(
        "iiiii", flags_p->halt_flag, flags_p->input_flag,
        flags_p->output_flag, flags_p->jmp_flag, curr_state.pc);
    //return 0;
}

static PyObject *method_restart_toy(PyObject *self, PyObject *args)
{
    char *result_str = malloc(20000);

    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    reset_state(&curr_state);
    result_str = show_state(&curr_state);

    PyObject *py_result_str = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result_str;
}

static PyObject *method_show_state(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    char *result_str = show_state(&curr_state);

    PyObject *py_result_str = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result_str;
}

static PyObject *method_stop_toy(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    reset_state(&curr_state);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *method_reset_toy(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    curr_state.pc = MEM_PROG_START;
    curr_state.flags_p->halt_flag = 0;
    curr_state.flags_p->input_flag = 0;
    curr_state.flags_p->output_flag = 0;
    curr_state.flags_p->jmp_flag = 0;
    for (int iii = 0; iii < MEM_SIZE; iii++) {
        curr_state.mem[iii] = 0;
    }
    for (int iii = 0; iii < NUM_REGS; iii++) {
        curr_state.regs[iii] = 0;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *method_toydisasm(PyObject *self, PyObject *args)
{
    ToyInstr *instr_p = curr_state.instr_p;

    char *result_str = malloc(5000);
    strcpy(result_str, "");
    char *tmp = malloc(50);
    PyObject *py_result_str;

    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    unsigned char byte1;

    int iii = MEM_PROG_START;
    while(iii != curr_program.eof) {
        byte1 = (curr_state.mem[iii] & 0xFF00) >> 8;

        instr_p->opcode = (byte1 & 0xF0) >> 4;
        printf("%X\n", instr_p->opcode);
        switch(instr_p->opcode) {
            case 0x0:
                sprintf(tmp, "%d | halt\n", iii+1 - MEM_PROG_START);
                strcat(result_str, tmp);
                break;
            case 0x1:
                scan_opcode(11, &curr_state, iii);
                sprintf(tmp, "%d | add R%X, R%X, R%X\n", iii+1 - MEM_PROG_START, instr_p->u1.dst, instr_p->u2.src.src1, instr_p->u2.src.src2);
                strcat(result_str, tmp);
                break;
            case 0x2:
                scan_opcode(11, &curr_state, iii);
                sprintf(tmp, "%d | sub R%X, R%X, R%X\n", iii+1 - MEM_PROG_START, instr_p->u1.dst, instr_p->u2.src.src1, instr_p->u2.src.src2);
                strcat(result_str, tmp);
                break;
            case 0x3:
                scan_opcode(11, &curr_state, iii);
                sprintf(tmp, "%d | and R%X, R%X, R%X\n", iii+1 - MEM_PROG_START, instr_p->u1.dst, instr_p->u2.src.src1, instr_p->u2.src.src2);
                strcat(result_str, tmp);
                break;
            case 0x4:
                scan_opcode(11, &curr_state, iii);
                sprintf(tmp, "%d | xor R%X, R%X, R%X\n", iii+1 - MEM_PROG_START, curr_instr.u1.dst, instr_p->u2.src.src1, instr_p->u2.src.src2);
                strcat(result_str, tmp);
                break;
            case 0x5:
                scan_opcode(11, &curr_state, iii);
                sprintf(tmp, "%d | shl R%X, R%X, R%X\n", iii+1 - MEM_PROG_START, instr_p->u1.dst, instr_p->u2.src.src1, instr_p->u2.src.src2);
                strcat(result_str, tmp);
                break;
            case 0x6:
                scan_opcode(11, &curr_state, iii);
                sprintf(tmp, "%d | shr R%X, R%X, R%X\n", iii+1 - MEM_PROG_START, instr_p->u1.dst, instr_p->u2.src.src1, instr_p->u2.src.src2);
                strcat(result_str, tmp);
                break;
            case 0x7:
                scan_opcode(21, &curr_state, iii);
                sprintf(tmp, "%d | ldaddr R%X, 0x%s\n", iii+1 - MEM_PROG_START, instr_p->u1.dst, pad8(instr_p->u2.addr));
                strcat(result_str, tmp);
                break;
            case 0x8:
                scan_opcode(21, &curr_state, iii);
                sprintf(tmp, "%d | ld R%X, [0x%s]\n", iii+1 - MEM_PROG_START, instr_p->u1.dst, pad8(instr_p->u2.addr));
                strcat(result_str, tmp);
                break;
            case 0x9:
                scan_opcode(22, &curr_state, iii);
                sprintf(tmp, "%d | str R%X, [0x%s]\n", iii+1 - MEM_PROG_START, instr_p->u1.src, pad8(instr_p->u2.addr));
                strcat(result_str, tmp);
                break;
            case 0xA:
                scan_opcode(12, &curr_state, iii);
                sprintf(tmp, "%d | ldi R%X, [R%X]\n", iii+1 - MEM_PROG_START, instr_p->u1.dst, instr_p->u2.src.src2);
                strcat(result_str, tmp);
                break;
            case 0xB:
                scan_opcode(12, &curr_state, iii);
                sprintf(tmp, "%d | stri R%X, [R%X]\n", iii+1 - MEM_PROG_START,  instr_p->u1.dst, instr_p->u2.src.src2);
                strcat(result_str, tmp);
                break;
            case 0xC:
                scan_opcode(22, &curr_state, iii);
                sprintf(tmp, "%d | bz R%X, 0x%s\n", iii+1 - MEM_PROG_START, instr_p->u1.src, pad8(instr_p->u2.addr));
                strcat(result_str, tmp);
                break;
            case 0xD:
                scan_opcode(22, &curr_state, iii);
                sprintf(tmp, "%d | bp R%X, 0x%s\n", iii+1 - MEM_PROG_START, instr_p->u1.src, pad8(instr_p->u2.addr));
                strcat(result_str, tmp);
                break;
            case 0xE:
                instr_p->u1.dst = byte1 & 0x0F;
                sprintf(tmp, "%d | jmpreg [R%X]\n", iii+1 - MEM_PROG_START, instr_p->u1.dst);
                strcat(result_str, tmp);
                break;
            case 0xF:
                scan_opcode(21, &curr_state, iii);
                sprintf(tmp, "%d | jmpl [R%X], 0x%s\n", iii+1 - MEM_PROG_START, instr_p->u1.dst, pad8(instr_p->u2.addr));
                strcat(result_str, tmp);
                break;
        }
        iii++;
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

    curr_state.mem[MEM_IO] = value;
    curr_state.regs[curr_state.instr_p->u1.dst] = curr_state.mem[MEM_IO];
    curr_state.flags_p->input_flag = 0;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *method_output_toy(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }
    curr_state.flags_p->output_flag = 0;
    return PyLong_FromLong(curr_state.regs[curr_instr.u1.src]);
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