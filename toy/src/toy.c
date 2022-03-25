#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toy.h"
#include "util.h"

void load_program(ToyState *curr_state_p)
{
    ToyProgram *program_p = curr_state_p->program_p;
    ToyRam *ram_p = curr_state_p->ram_p;
    int byte1, byte2;
    FILE *fp = fopen(program_p->filename, "rb");

    int iii = 0;
    while (iii <= MEM_DATA_END) {
        ram_p->data_mem[iii++] = getc(fp);
    }
    iii = 0;
    while((byte1 = getc(fp)) != EOF && (byte2 = getc(fp)) != EOF) {
        ram_p->prog_mem[iii++] = (byte1 << 8) | byte2;
    }
    program_p->eof = iii;
    fclose(fp);
}

void exec_instr(ToyState *curr_state_p)
{
    unsigned short int *regs = curr_state_p->regs;
    ToyInstr *instr_p = curr_state_p->instr_p;
    ToyFlags *flags_p = curr_state_p->flags_p;
    ToyRam *ram_p = curr_state_p->ram_p;

    switch (instr_p->opcode) {
        case OP_HALT:
            flags_p->halt_flag = 1;
            break;
        case OP_ADD:
            scan_opcode(11, curr_state_p, curr_state_p->pc);
            regs[instr_p->u1.dst] = regs[instr_p->u2.src.src1] + regs[instr_p->u2.src.src2];
            break;
        case OP_SUB:
            scan_opcode(11, curr_state_p, curr_state_p->pc);
            regs[instr_p->u1.dst] = regs[instr_p->u2.src.src1] - regs[instr_p->u2.src.src2];
            break;
        case OP_AND:
            scan_opcode(11, curr_state_p, curr_state_p->pc);
            regs[instr_p->u1.dst] = regs[instr_p->u2.src.src1] & regs[instr_p->u2.src.src2];
            break;
        case OP_XOR:
            scan_opcode(11, curr_state_p, curr_state_p->pc);
            regs[instr_p->u1.dst] = regs[instr_p->u2.src.src1] ^ regs[instr_p->u2.src.src2];
            break;
        case OP_SHL:
            scan_opcode(11, curr_state_p, curr_state_p->pc);
            regs[instr_p->u1.dst] = regs[instr_p->u2.src.src1] << regs[instr_p->u2.src.src2];
            break;
        case OP_SHR:
            scan_opcode(11, curr_state_p, curr_state_p->pc);
            regs[instr_p->u1.dst] = regs[instr_p->u2.src.src1] >> regs[instr_p->u2.src.src2];
            break;
        case OP_LDADDR:
            scan_opcode(21, curr_state_p, curr_state_p->pc);
            regs[instr_p->u1.dst] = instr_p->u2.addr;
            break;
        case OP_LD:
            scan_opcode(21, curr_state_p, curr_state_p->pc);
            if (instr_p->u2.addr == MEM_IO) {
                flags_p->input_flag = 1;
            }
            else {
                if (instr_p->u2.addr <= MEM_DATA_END) {
                    regs[instr_p->u1.dst] = ram_p->data_mem[instr_p->u2.addr];
                }
                else {
                    regs[instr_p->u1.dst] = ram_p->prog_mem[instr_p->u2.addr - MEM_PROG_START];
                }
            }
            break;
        case OP_STR:
            scan_opcode(22, curr_state_p, curr_state_p->pc);
            if (instr_p->u2.addr == MEM_IO) {
                flags_p->output_flag = 1;
            }
            else {
                if (instr_p->u2.addr <= MEM_DATA_END) {
                    ram_p->data_mem[instr_p->u2.addr] = regs[instr_p->u1.src];
                }
                else {
                    ram_p->prog_mem[instr_p->u2.addr - MEM_PROG_START] = regs[instr_p->u1.src];
                }
            }
            break;
        case OP_LDI:
            scan_opcode(12, curr_state_p, curr_state_p->pc);
            if (regs[instr_p->u2.src.src2] <= MEM_DATA_END) {
                regs[instr_p->u1.dst] = ram_p->data_mem[regs[instr_p->u2.src.src2]];
            }
            else {
                regs[instr_p->u1.dst] = ram_p->prog_mem[regs[instr_p->u2.src.src2] - MEM_PROG_START];
            }
            break;
        case OP_STRI:
            scan_opcode(12, curr_state_p, curr_state_p->pc);
            if (regs[instr_p->u2.src.src2] <= MEM_DATA_END) {
                ram_p->data_mem[regs[instr_p->u2.src.src2]] = regs[instr_p->u1.dst];
            }
            else {
                ram_p->prog_mem[regs[instr_p->u2.src.src2] - MEM_PROG_START] = regs[instr_p->u1.dst];
            }
            
            break;
        case OP_BZ:
            scan_opcode(22, curr_state_p, curr_state_p->pc);
            if (regs[instr_p->u1.src] == 0) {
                curr_state_p->pc = instr_p->u2.addr;
                flags_p->jmp_flag = 1;
            }
            break;
        case OP_BP:
            scan_opcode(22, curr_state_p, curr_state_p->pc);
            if (regs[instr_p->u1.src] > 0) {
                curr_state_p->pc = instr_p->u2.addr;
                flags_p->jmp_flag = 1;
            }
            break;
        case OP_JMPREG:
            instr_p->u1.src = (ram_p->prog_mem[curr_state_p->pc - MEM_PROG_START] & 0x0F00) >> 8;
            curr_state_p->pc = regs[instr_p->u1.src];
            flags_p->jmp_flag = 1;
            break;
        case OP_JMPL:
            scan_opcode(21, curr_state_p, curr_state_p->pc);
            regs[instr_p->u1.dst] = ++curr_state_p->pc;
            curr_state_p->pc = instr_p->u2.addr;
            curr_state_p->flags_p->jmp_flag = 1;
            break;
    }
}

void scan_opcode(int idx, ToyState *curr_state_p, int pos)
{
    ToyRam *ram_p = curr_state_p->ram_p;
    ToyInstr *instr_p = curr_state_p->instr_p;

    unsigned char byte1 = (ram_p->prog_mem[pos] & 0xFF00) >> 8;
    unsigned char byte2 = ram_p->prog_mem[pos] & 0x00FF;

    switch (idx) {
        case 11:
            instr_p->u1.dst = byte1 & 0x0F;
            instr_p->u2.src.src1 = (byte2 & 0xF0) >> 4;
            instr_p->u2.src.src2 = byte2 & 0x0F;
            break;
        case 12:
            instr_p->u1.dst = byte1 & 0x0F;
            instr_p->u2.src.src2 = byte2 & 0x0F;
            break;
        case 21:
            instr_p->u1.dst = byte1 & 0x0F;
            instr_p->u2.addr = byte2 & 0xFF;
            break;
        case 22:
            instr_p->u1.src = byte1 & 0x0F;
            instr_p->u2.addr = byte2 & 0xFF;
            break;
    }
}

char *show_regs(ToyState *curr_state_p)
{
    unsigned short int *regs = curr_state_p->regs;

    char *out_str = malloc(10000);
    char *temp = malloc(50);
    strcpy(out_str, "---------------------------------------------------\n");
    strcat(out_str, "| Registers                                       |\n");
    strcat(out_str, "---------------------------------------------------\n");
    for (int iii = 0; iii < NUM_REGS; iii++) {
        if (iii + 1 == NUM_REGS) {
            sprintf(temp, "R%X = 0x%s\n", iii, pad16(regs[iii]));
            strcat(out_str, temp);
            strcat(out_str, "---------------------------------------------------"
                "\n");
            strcat(out_str, "| Program Counter                                 "
                "|\n");
            strcat(out_str, "---------------------------------------------------"
                "\n");
            sprintf(temp, "PC = 0x%s\n", pad8(curr_state_p->pc));
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

char *show_state(ToyState *curr_state_p)
{
    ToyRam *ram_p = curr_state_p->ram_p;

    char *result_str = malloc(20000);

    strcpy(result_str, show_regs(curr_state_p));
    strcat(result_str, show_mem(ram_p->data_mem, MEM_DATA_SIZE, 0));
    strcat(result_str, show_mem(ram_p->prog_mem, MEM_PROG_SIZE, MEM_PROG_START));

    return result_str;
}

void reset_state(ToyState *state_p, Boolean res_ram)
{
    state_p->pc = MEM_PROG_START;
    state_p->flags_p->halt_flag = 0;
    state_p->flags_p->input_flag = 0;
    state_p->flags_p->output_flag = 0;
    state_p->ram_p->io = 0;

    for (int iii = 0; iii < NUM_REGS; iii++) {
        state_p->regs[iii] = 0;
    }

    if (res_ram == TRUE) {
        for (int iii = 0; iii < MEM_DATA_SIZE; iii++) {
            state_p->ram_p->data_mem[iii] = 0;
        }
        for (int iii = 0; iii < MEM_PROG_SIZE; iii++) {
            state_p->ram_p->prog_mem[iii] = 0;
        }
    }
}