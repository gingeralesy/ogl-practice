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
#define min(x, y) (x < y ? x : y)
#endif // min
#ifndef max
#define max(x, y) (x < y ? y : x)
#endif // max

#ifndef array_length
#define array_length(x) (sizeof(x) / sizeof(x[0]))
#else
#error "array_length already defined"
#endif // array_length

#endif // COMMON_H
