#ifndef OPENGL_H
#define OPENGL_H

#include <common.h>

typedef enum shader_e
{
  SHADER_DEFAULT = 0,
  SHADER_SECOND
} Shader;

GLboolean opengl_setup(GLFWwindow **);

GLuint opengl_fragment_shader(Shader);
GLuint opengl_vertex_shader(Shader);

GLuint opengl_program(GLuint, GLuint);

#endif // SB_OPENGL_H
