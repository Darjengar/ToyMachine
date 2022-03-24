/*  License: GPL 3.0, written in ANSI C + CPython */
/**
 * @file util.h
 * @author Darjengar
 * @brief Helper functions for TOY machines code.
 * @version 0.1
 * @date 2022-03-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef UTIL_H
#define UTIL_H

/**
 * @brief 
 * 
 * @param num 
 * @return char* 
 */
char *pad8(unsigned char num);

/**
 * @brief 
 * 
 * @param num 
 * @return char* 
 */
char *pad16(unsigned short int num);

/**
 * @brief 
 * 
 * @param mem 
 * @param size 
 * @return char* 
 */
char *show_mem(unsigned short int mem[], int size);

#endif