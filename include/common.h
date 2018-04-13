#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else // POSIX
#include <unistd.h>
#endif // _WIN32 - POSIX

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef min
/**
 * @brief Returns the lower value of the two
 * 
 * This is type safe using GCC's typeof.
 * 
 * @param a first value
 * @param b second value
 * @return lower value of a and b
 */
#define min(a,b)                                \
  { __typeof__ (a) _a = (a);                    \
    __typeof__ (b) _b = (b);                    \
    _a < _b ? _a : _b; }
#endif // min
#ifndef max
/**
 * @brief Returns the higher value of the two
 * 
 * This is type safe using GCC's typeof.
 * 
 * @param a first value
 * @param b second value
 * @return higher value of a and b
 */
#define max(a,b)                                \
  { __typeof__ (a) _a = (a);                    \
    __typeof__ (b) _b = (b);                    \
    _b < _a ? _a : _b; }
#endif // max

#ifndef array_length
/**
 * @brief Returns the length of an array
 * @param x an array with values
 * @return length of the array with values of arbitrary size
 */
#define array_length(x) (sizeof(x) / sizeof(x[0]))
#else
#error "array_length already defined"
#endif // array_length

#endif // COMMON_H
