/*  License: GPL 3.0, written in C99 + CPython */
/**
 * @file toy.h
 * @author Darjengar
 * @brief TOY machines implementation.
 * @version 0.1
 * @date 2021-08-23
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef TOY_H
#define TOY_H

#define MEM_DATA_START 0x00
#define MEM_DATA_END 0x0F
#define MEM_PROG_START 0x10
#define MEM_PROG_END 0xFE
#define MEM_IO 0xFF
#define MEM_SIZE 256

#define MEM_DATA_SIZE 16
#define MEM_PROG_SIZE 240

#define NUM_REGS 16

#define MAX_NUM_STATES 1

enum boolean {FALSE, TRUE};

typedef struct {
    unsigned char data_mem[MEM_DATA_SIZE];
    unsigned short int prog_mem[MEM_PROG_SIZE];
    unsigned char io;
} ToyRam;

typedef enum opcode {
    OP_HALT,
    OP_ADD,
    OP_SUB,
    OP_AND,
    OP_XOR,
    OP_SHL,
    OP_SHR,
    OP_LDADDR,
    OP_LD,
    OP_STR,
    OP_LDI,
    OP_STRI,
    OP_BZ,
    OP_BP,
    OP_JMPREG,
    OP_JMPL
} OpCode;

/**
 * @brief 
 * 
 */
typedef struct {
    unsigned int halt_flag : 1;
    unsigned int input_flag : 1;
    unsigned int output_flag : 1;
    unsigned int jmp_flag : 1;
} ToyFlags;

/**
 * @brief 
 * 
 */
typedef struct {
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
} ToyInstr;

typedef struct {
    char *filename;
    int eof;
} ToyProgram;

typedef struct {
    /*unsigned short int mem[MEM_SIZE];*/
    unsigned short int regs[NUM_REGS];
    unsigned char pc;
    ToyRam *ram_p;
    ToyFlags *flags_p;
    ToyInstr *instr_p;
    ToyProgram *program_p;
} ToyState;

/**
 * @brief 
 * 
 * @param program 
 */
void load_program(ToyState *curr_state_p);

/**
 * @brief 
 * 
 */
void exec_instr();

/**
 * @brief 
 * 
 * @param idx 
 * @param curr_instr_p 
 * @param byte1 
 * @param byte2 
 */
void scan_opcode(int idx, ToyState *curr_state_p, int pos);

/**
 * @brief 
 * 
 * @param regs 
 * @param size 
 * @return char* 
 */
char *show_regs(ToyState *curr_state_p);

/**
 * @brief 
 * 
 * @param curr_state_p 
 * @return char* 
 */
char *show_state(ToyState *curr_state_p);

/**
 * @brief 
 * 
 * @param state_p 
 */
void reset_state(ToyState *state_p);

#endif