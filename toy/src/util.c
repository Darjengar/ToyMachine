/*  License: GPL 3.0, written in C99 */
/**
 * @file util.c
 * @author Darjengar
 * @brief Helper functions for TOY machines code.
 * @version 0.1
 * @date 2021-08-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

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

char *pad16(unsigned short int num)
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

char *show_mem(unsigned short int mem[], int size, int start_num)
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
    for (int iii = start_num; iii < size; iii++) {
        if (iii+1 == size) {
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