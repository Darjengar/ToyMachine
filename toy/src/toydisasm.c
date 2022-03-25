/* Author : Darjengar 12/17/2021, License: GPL 3.0 */
/* written in ANSI C */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *toydisasm(const char *filepath);
void scan_opcode1(unsigned char byte1, unsigned char byte2, unsigned char *dst_ptr, unsigned char *src1_ptr, unsigned char *src2_ptr);
void scan_opcode2(unsigned char byte1, unsigned char byte2, unsigned char *dst_ptr, unsigned char *addr_ptr);
void scan_opcode12(unsigned char byte1, unsigned char byte2, unsigned char *dst_ptr, unsigned char *src2_ptr);
char *pad8(unsigned char num);

int main(int argc, char * argv[])
{
    if (argc != 2) {
        printf("Error\n");
        return -1;
    }
    
    const char *filepath = argv[1];
    int *test = toydisasm(filepath);
    return 0;
}

void load_program(const char* program)
{
    int byte1, byte2;
    FILE *fp = fopen(program, "rb");
    unsigned short mem[256] = {0};

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

void scan_opcode1(unsigned char byte1, unsigned char byte2, unsigned char *dst_ptr, unsigned char *src1_ptr, unsigned char *src2_ptr)
{
    *dst_ptr = byte1 & 0x0F;
    *src1_ptr = (byte2 & 0xF0) >> 4;
    *src2_ptr = byte2 & 0x0F;
}

void scan_opcode2(unsigned char byte1, unsigned char byte2, unsigned char *dst_ptr, unsigned char *addr_ptr)
{
    *dst_ptr = byte1 & 0x0F;
    *addr_ptr = byte2 & 0xFF;
}

void scan_opcode12(unsigned char byte1, unsigned char byte2, unsigned char *dst_ptr, unsigned char *src2_ptr)
{
    *dst_ptr = byte1 & 0x0F;
    *src2_ptr = byte2 & 0x0F;
}

int *toydisasm(const char *filepath)
{
    int byte1, byte2;
    unsigned short program[256] = {0};

    FILE *fp = fopen(filepath, "rb");

    int iii = 0;
    unsigned char opcode;
    unsigned char dst;
    unsigned char src1;
    unsigned char src2;
    unsigned char addr;

    fseek(fp, 0x20, SEEK_SET);
    while((byte1 = getc(fp)) != EOF && (byte2 = getc(fp)) != EOF) {
        opcode = (byte1 & 0xF0) >> 4;
        switch(opcode) {
        case 0x0:
            printf("halt\n");
            break;
        case 0x1:
            scan_opcode1(byte1, byte2, &dst, &src1, &src2);
            printf("add R%X, R%X, R%X\n", dst, src1, src2);
            break;
        case 0x2:
            scan_opcode1(byte1, byte2, &dst, &src1, &src2);
            printf("sub R%X, R%X, R%X\n", dst, src1, src2);
            break;
        case 0x3:
            scan_opcode1(byte1, byte2, &dst, &src1, &src2);
            printf("and R%X, R%X, R%X\n", dst, src1, src2);
            break;
        case 0x4:
            scan_opcode1(byte1, byte2, &dst, &src1, &src2);
            printf("xor R%X, R%X, R%X\n", dst, src1, src2);
            break;
        case 0x5:
            scan_opcode1(byte1, byte2, &dst, &src1, &src2);
            printf("shl R%X, R%X, R%X\n", dst, src1, src2);
            break;
        case 0x6:
            scan_opcode1(byte1, byte2, &dst, &src1, &src2);
            printf("shr R%X, R%X, R%X\n", dst, src1, src2);
            break;
        case 0x7:
            scan_opcode2(byte1, byte2, &dst, &addr);
            printf("ldaddr R%X, 0x%s\n", dst, pad8(addr));
            break;
        case 0x8:
            scan_opcode2(byte1, byte2, &dst, &addr);
            printf("ld R%X, [0x%s]\n", dst, pad8(addr));
            break;
        case 0x9:
            scan_opcode2(byte1, byte2, &dst, &addr);
            printf("str R%X, [0x%s]\n", dst, pad8(addr));
            break;
        case 0xA:
            scan_opcode12(byte1, byte2, &dst, &src2);
            printf("ldi R%X, [R%X]\n", dst, src2);
            break;
        case 0xB:
            scan_opcode12(byte1, byte2, &dst, &src2);
            printf("stri R%X, [R%X]\n", dst, src2);
            break;
        case 0xC:
            scan_opcode2(byte1, byte2, &dst, &addr);
            printf("bz R%X, 0x%s\n", dst, pad8(addr));
            break;
        case 0xD:
            scan_opcode2(byte1, byte2, &dst, &addr);
            printf("bp R%X, 0x%s\n", dst, pad8(addr));
            break;
        case 0xE:
            dst = byte1 & 0x0F;
            printf("jmpreg\n");
            printf("jmpreg [R%X]\n", dst);
            break;
        case 0xF:
            scan_opcode2(byte1, byte2, &dst, &addr);
            printf("jmpl\n");
            printf("jmpl R%X, 0x%s\n", dst, pad8(addr));
            break;
        }
    }
    fclose(fp);
    return 0;
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