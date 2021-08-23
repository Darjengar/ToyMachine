/* Author : Darjengar 08/23/2021, License: GPL 3.0 */
/* written in ANSI C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#include <Python.h>*/

enum boolean {FALSE, TRUE};

unsigned short mem[256];
unsigned short regs[16];
unsigned char pc;

char *pad8(unsigned char num);
char *pad16(unsigned short num);
void show_mem(unsigned short mem[], int size);
void show_regs(unsigned short regs[], int size);
void test();
void init_toy(char *program);
int start_toy(char *program);
void load_program(char *program);
int exec_instr(unsigned short opcode);

int main(int argc, char *argv[])
{
    char *program = argv[1];
    /*test();*/
    if (start_toy(program) != 0) {
        return -1;
    }
    return 0;
}

void init_toy(char *program)
{
    regs[0] = 0;
    pc = 0x10;
    load_program(program);
}

int start_toy(char *program)
{
    init_toy(program);
    /* Fetch-Execute */
    unsigned short opcode;
    while (TRUE) {
        show_regs(regs, 16);
        show_mem(mem, 256);
        opcode = (mem[pc] & 0xF000) >> 12;
        if (exec_instr(opcode) != 0) {
            return -1;
        }
        pc++;
    }
}

void load_program(char* program)
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
    fclose(fp);
}

int exec_instr(unsigned short opcode)
{
    unsigned char dest;
    unsigned char src1;
    unsigned char src2;
    unsigned char addr;
    switch (opcode) {
        /* halt */
        case 0x0:
            printf("Halt TOY machine.\n");
            exit(0);
            break;
        /* add */
        case 0x1:
            dest = (mem[pc] & 0x0F00) >> 8;
            src1 = (mem[pc] & 0x00F0) >> 4;
            src2 = mem[pc] & 0x000F;
            regs[dest] = regs[src1] + regs[src2];
            break;
        /* subtract */
        case 0x2:
            dest = (mem[pc] & 0x0F00) >> 8;
            src1 = (mem[pc] & 0x00F0) >> 4;
            src2 = mem[pc] & 0x000F;
            regs[dest] = regs[src1] - regs[src2];
            break;
        /* and */
        case 0x3:
            dest = (mem[pc] & 0x0F00) >> 8;
            src1 = (mem[pc] & 0x00F0) >> 4;
            src2 = mem[pc] & 0x000F;
            regs[dest] = regs[src1] & regs[src2];
            break;
        /* xor */
        case 0x4:
            dest = (mem[pc] & 0x0F00) >> 8;
            src1 = (mem[pc] & 0x00F0) >> 4;
            src2 = mem[pc] & 0x000F;
            regs[dest] = regs[src1] ^ regs[src2];
            break;
        /* shift left */
        case 0x5:
            dest = (mem[pc] & 0x0F00) >> 8;
            src1 = (mem[pc] & 0x00F0) >> 4;
            src2 = mem[pc] & 0x000F;
            regs[dest] = regs[src1] << regs[src2];
            break;
        /* shift right */
        case 0x6:
            dest = (mem[pc] & 0x0F00) >> 8;
            src1 = (mem[pc] & 0x00F0) >> 4;
            src2 = mem[pc] & 0x000F;
            regs[dest] = regs[src1] >> regs[src2];
            break;
        /* load addr */
        case 0x7:
            dest = (mem[pc] & 0x0F00) >> 8;
            addr = mem[pc] & 0x00FF;
            regs[dest] = addr;
            break;
        /* load */
        case 0x8:
            dest = (mem[pc] & 0x0F00) >> 8;
            addr = mem[pc] & 0x00FF;
            regs[dest] = mem[addr];
            break;
        /* store */
        case 0x9:
            src1 = (mem[pc] & 0x0F00) >> 8;
            addr = mem[pc] & 0x00FF;
            mem[addr] = regs[src1];
            break;
        /* load indirect */
        case 0xA:
            dest = (mem[pc] & 0x0F00) >> 8;
            src2 = mem[pc] & 0x000F;
            regs[dest] = mem[regs[src2]];
            break;
        /* store indirect */
        case 0xB:
            dest = (mem[pc] & 0x0F00) >> 8;
            src2 = mem[pc] & 0x000F;
            mem[regs[src2]] = regs[dest];
            break;
        /* branch zero */
        case 0xC:
            src1 = (mem[pc] & 0x0F00) >> 8;
            addr = mem[pc] & 0x00FF;
            if (regs[src1] == 0) {
                pc = addr;
            }
            break;
        /* branch positive */
        case 0xD:
            src1 = (mem[pc] & 0x0F00) >> 8;
            addr = mem[pc] & 0x00FF;
            if (regs[src1] > 0) {
                pc = addr;
            }
            break;
        /* jump register */
        case 0xE:
            src1 = (mem[pc] & 0x0F00) >> 8;
            pc = regs[src1];
            break;
        /* jump and link */
        case 0xF:
            dest = (mem[pc] & 0x0F00) >> 8;
            addr = mem[pc] & 0x00FF;
            regs[dest] = pc;
            pc = addr;
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
    char *result = malloc(3);
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
    char *result = malloc(5);
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

void show_mem(unsigned short mem[], int size)
{
    printf("----------------------------------------------\n");
    printf("| Memory                                     |\n");
    printf("----------------------------------------------\n");
    char *temp;
    temp = pad16(0);
    printf("%s | ", temp);
    free(temp);
    for (int iii = 0; iii < size; iii++) {
        if (iii+1 == size) {
            /*printf("%s ", pad8((mem[iii] & 0xFF00)
                            >> 8));*/
            temp = pad16(mem[iii]);
            printf("%s\n", temp);
            free(temp);
        }
        else if ((iii+1) % 8 == 0) {
            temp = pad16(mem[iii]);
            printf("%s\n", temp);
            free(temp);
            temp = pad16(iii+1);
            printf("%s | ", temp);
            free(temp);
        }
        else {
            temp = pad16(mem[iii]);
            printf("%s ", temp);
            free(temp);
        }
    }
    printf("----------------------------------------------\n");
}

void show_regs(unsigned short regs[], int size)
{
    printf("---------------------------------------------------\n");
    printf("| Registers                                       |\n");
    printf("---------------------------------------------------\n");
    for (int iii = 0; iii < size; iii++) {
        if (iii + 1 == size) {
            printf("R%X = 0x%s\n", iii, pad16(regs[iii]));
            printf("---------------------------------------------------"
                "\n");
            printf("| Program Counter                                 "
                "|\n");
            printf("---------------------------------------------------"
                "\n");
            printf("PC = 0x%s\n", pad8(pc));
            printf("---------------------------------------------------"
                "\n");
        }
        else if ((iii+1) % 4 == 0) {
            printf("R%X = 0x%s, ", iii, pad16(regs[iii]));
            printf("\n");
        }
        else {
            printf("R%X = 0x%s, ", iii, pad16(regs[iii]));
        }
    }
}
