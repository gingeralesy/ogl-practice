#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef min
#define min(x, y) (x < y ? x : y)
#endif // min
#ifndef max
#define max(x, y) (x < y ? y : x)
#endif // max

#endif // COMMON_H
