/* Author : Darjengar 12/17/2021, License: GPL 3.0 */
/* written in ANSI C */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "toy.h"

#define MAX_LINE_LENGTH 64

int get_regnum(char*);
void set_toyop(char*[], int, unsigned short int[], int, int);
void print_usage();

int main(int argc, char *argv[])
{
    char *prog = argv[0];

    if (argc > 3) {
        fprintf(stderr, "%s: Error: Too many arguments!\n", prog);
        print_usage();
        exit(-1);
    }
    else if (argc < 3) {
        fprintf(stderr, "%s: Error: Too few arguments!\n", prog);
        print_usage();
        exit(-1);
    }
    else {
        char *filename = argv[1];
        char *outname = argv[2];
        char line[MAX_LINE_LENGTH] = {0};
        char *tmp;
        FILE *fp = NULL;

        if ((fp = fopen(filename, "r")) == NULL) {
            fprintf(
                stderr, "%s: Error: Can't open program %s!\n",
                prog, filename);
            exit(-1);
        }

        char *op[4] = {};
        unsigned short int program[MEM_SIZE] = {0};
        int iii = MEM_PROG_START;
        int num_lines = 0;
        while(fgets(line, MAX_LINE_LENGTH, fp)) {
            num_lines++;
            tmp = strtok(line, " ");
            int jjj = 0;
            while(tmp != NULL) {
                op[jjj] = tmp;
                tmp = strtok(NULL, ",[] \n");
                jjj++;
            }

            if (strcmp(op[0], "halt") == 0) {
                program[iii] = 0;
            }
            else if (strcmp(op[0],"add") == 0) {
                set_toyop(op, OP_ADD, program, iii, 1);
            }
            else if (strcmp(op[0],"sub") == 0) {
                set_toyop(op, OP_SUB, program, iii, 1);
            }
            else if (strcmp(op[0],"and") == 0) {
                set_toyop(op, OP_AND, program, iii, 1);
            }
            else if (strcmp(op[0],"xor") == 0) {
                set_toyop(op, OP_XOR, program, iii, 1);
            }
            else if (strcmp(op[0],"shl") == 0) {
                set_toyop(op, OP_SHL, program, iii, 1);
            }
            else if (strcmp(op[0],"shr") == 0) {
                set_toyop(op, OP_SHR, program, iii, 1);
            }
            else if (strcmp(op[0], "ldaddr") == 0) {
                set_toyop(op, OP_LDADDR, program, iii, 2);
            }
            else if (strcmp(op[0],"ld") == 0) {
                set_toyop(op, OP_LD, program, iii, 2);
            }
            else if (strcmp(op[0],"str") == 0) {
                set_toyop(op, OP_STR, program, iii, 2);
            }
            else if (strcmp(op[0],"ldi") == 0) {
                program[iii] = OP_LDI<<12 | (get_regnum(op[1])<<8)
                        | get_regnum(op[2]);
            }
            else if (strcmp(op[0],"stri") == 0) {
                program[iii] = OP_STRI<<12 | (get_regnum(op[1])<<8)
                        | get_regnum(op[2]);
            }
            else if (strcmp(op[0],"bz") == 0) {
                set_toyop(op, OP_BZ, program, iii, 2);
            }
            else if (strcmp(op[0],"bp") == 0) {
                set_toyop(op, OP_BP, program, iii, 2);
            }
            else if (strcmp(op[0],"jmpreg") == 0) {
                program[iii] = OP_JMPREG<<12 | (get_regnum(op[1])<<8);
            }
            else if (strcmp(op[0],"jmpl") == 0) {
                set_toyop(op, OP_JMPL, program, iii, 2);
            }
            iii++;
        }
        printf("\n");
        printf("%s", show_mem(program, 256));
        FILE *fp2 = NULL;
        strcat(outname, ".bin");
        if ((fp2 = fopen(outname, "w")) == NULL) {
            fprintf(stderr, "%s: Error: Can't open %s!", prog, outname);
            print_usage();
            exit(-1);
        }
        for (int iii = 0; iii < 16 + num_lines; iii++) {
            putc((program[iii] & 0xFF00) >> 8, fp2);
            putc((program[iii] & 0x00FF), fp2);
        }
        fclose(fp);
        fclose(fp2);
    }
    exit(0);
}

void print_usage() {
    printf(
        "Usage: toyasm filename outname\nfilename : *.toy\noutname : name without suffix\n");
}

int get_regnum(char *reg)
{
    if (strcmp(reg,"R0") == 0) {
        return 0;
    }
    if (strcmp(reg,"R1") == 0) {
        return 1;
    }
    if (strcmp(reg,"R2") == 0) {
        return 2;
    }
    if (strcmp(reg,"R3") == 0) {
        return 3;
    }
    if (strcmp(reg,"R4") == 0) {
        return 4;
    }
    if (strcmp(reg,"R5") == 0) {
        return 5;
    }
    if (strcmp(reg,"R6") == 0) {
        return 6;
    }
    if (strcmp(reg,"R7") == 0) {
        return 7;
    }
    if (strcmp(reg,"R8") == 0) {
        return 8;
    }
    if (strcmp(reg,"R9") == 0) {
        return 9;
    }
    if (strcmp(reg,"RA") == 0) {
        return 10;
    }
    if (strcmp(reg,"RB") == 0) {
        return 11;
    }
    if (strcmp(reg,"RC") == 0) {
        return 12;
    }
    if (strcmp(reg,"RD") == 0) {
        return 13;
    }
    if (strcmp(reg,"RE") == 0) {
        return 14;
    }
    if (strcmp(reg,"RF") == 0) {
        return 15;
    }
}

void set_toyop(
        char* op[4], int opcode, unsigned short int program[MEM_SIZE],
        int pos, int fmtnum)
{
    if (fmtnum == 1) {
        program[pos] = opcode<<12 | (get_regnum(op[1])<<8)
                        | (get_regnum(op[2])<<4)
                        | get_regnum(op[3]);
    }
    else if (fmtnum == 2) {
        program[pos] = opcode<<12 | (get_regnum(op[1])<<8)
                        | (int)strtol(op[2], (char**)NULL, 16);
    }
}