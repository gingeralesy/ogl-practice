#ifndef OPENGL_H
#define OPENGL_H

#include <common.h>
#include <shaders.h>

GLuint opengl_shader(ShaderFragment);
GLuint opengl_program(GLuint, GLuint);

#endif // SB_OPENGL_H
