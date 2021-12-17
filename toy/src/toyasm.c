/* Author : Darjengar 12/17/2021, License: GPL 3.0 */
/* written in ANSI C */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 64

int get_regnum(char*);
char *pad8(unsigned char num);
char *pad16(unsigned short num);
void show_mem(unsigned short mem[], int size);
void format(char*[], int, unsigned short[], int, int);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Error\n");
        return -1;
    }
    else {
        char *filename = argv[1];
        char line[MAX_LINE_LENGTH] = {0};
        char *tmp;
        FILE *fp = fopen(filename, "r");
        char *op[4] = {};
        unsigned short program[256] = {0};
        int iii = 16;
        while(fgets(line, MAX_LINE_LENGTH, fp)) {
            printf("%s", line);
            tmp = strtok(line, " ");
            int jjj = 0;
            while(tmp != NULL) {
                op[jjj] = tmp;
                tmp = strtok(NULL, ", \n");
                jjj++;
            }

            if (strcmp(op[0], "halt") == 0) {
                program[iii] = 0;
            }
            else if (strcmp(op[0],"add") == 0) {
                format(op, 1, program, iii, 1);
            }
            else if (strcmp(op[0],"sub") == 0) {
                format(op, 2, program, iii, 1);
            }
            else if (strcmp(op[0],"and") == 0) {
                format(op, 3, program, iii, 1);
            }
            else if (strcmp(op[0],"xor") == 0) {
                format(op, 4, program, iii, 1);
            }
            else if (strcmp(op[0],"shl") == 0) {
                format(op, 5, program, iii, 1);
            }
            else if (strcmp(op[0],"shr") == 0) {
                format(op, 6, program, iii, 1);
            }
            else if (strcmp(op[0], "ldaddr") == 0) {
                format(op, 7, program, iii, 2);
            }
            else if (strcmp(op[0],"ld") == 0) {
                format(op, 8, program, iii, 2);
            }
            else if (strcmp(op[0],"str") == 0) {
                format(op, 9, program, iii, 2);
            }
            else if (strcmp(op[0],"ldi") == 0) {
                program[iii] = 10<<12 | (get_regnum(op[1])<<8)
                        | get_regnum(op[2]);
            }
            else if (strcmp(op[0],"stri") == 0) {
                program[iii] = 11<<12 | (get_regnum(op[1])<<8)
                        | get_regnum(op[2]);
            }
            else if (strcmp(op[0],"brz") == 0) {
                format(op, 12, program, iii, 2);
            }
            else if (strcmp(op[0],"brp") == 0) {
                format(op, 13, program, iii, 2);
            }
            else if (strcmp(op[0],"jmpreg") == 0) {
                format(op, 14, program, iii, 2);
            }
            else if (strcmp(op[0],"jmpl") == 0) {
                format(op, 15, program, iii, 2);
            }
            iii++;
        }
        printf("\n");
        show_mem(program, 256);
        fclose(fp);
    }
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

void format(char* op[4], int opcode, unsigned short program[256], int pos, int fmtnum)
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