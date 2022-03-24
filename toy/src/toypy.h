/*  License: GPL 3.0, written in C99 + CPython */
/**
 * @file toypy.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TOYPY_H
#define TOYPY_H

#include <Python.h>
#include "toy.h"

/**
 * @brief 
 * 
 * @param self 
 * @param args 
 * @return PyObject* 
 */
static PyObject *method_init_toy(PyObject *self, PyObject *args);

/**
 * @brief 
 * 
 * @param self 
 * @param args 
 * @return PyObject* 
 */
static PyObject *method_start_toy(PyObject *self, PyObject *args);

/**
 * @brief 
 * 
 * @param self 
 * @param args 
 * @return PyObject* 
 */
static PyObject *method_restart_toy(PyObject *self, PyObject *args);

/**
 * @brief 
 * 
 * @param self 
 * @param args 
 * @return PyObject* 
 */
static PyObject *method_show_state(PyObject *self, PyObject *args);

/**
 * @brief 
 * 
 * @param self 
 * @param args 
 * @return PyObject* 
 */
static PyObject *method_stop_toy(PyObject *self, PyObject *args);

/**
 * @brief 
 * 
 * @param self 
 * @param args 
 * @return PyObject* 
 */
static PyObject *method_reset_toy(PyObject *self, PyObject *args);

/**
 * @brief 
 * 
 * @param self 
 * @param args 
 * @return PyObject* 
 */
static PyObject *method_toydisasm(PyObject *self, PyObject *args);

/**
 * @brief 
 * 
 * @param self 
 * @param args 
 * @return PyObject* 
 */
static PyObject *method_input_toy(PyObject *self, PyObject *args);

/**
 * @brief 
 * 
 * @param self 
 * @param args 
 * @return PyObject* 
 */
static PyObject *method_output_toy(PyObject *self, PyObject *args);

#endif