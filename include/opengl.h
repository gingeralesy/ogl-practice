#ifndef OPENGL_H
#define OPENGL_H

#include <common.h>
#include <shaders.h>

GLboolean opengl_setup(GLFWwindow **);
GLuint opengl_shader(Shader);
GLuint opengl_program(GLuint, GLuint);

#endif // SB_OPENGL_H
