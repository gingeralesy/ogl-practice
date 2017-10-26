#include "opengl.h"

#include <logger.h>
#include <handlers.h>
#include <shaders.h>

// Private headers


// Private functions

// Public functions

GLuint opengl_shader(ShaderFragment shader)
{
  GLint success = 0;
  GLuint _shader = 0;
  const GLchar *source_str;

  switch (shader)
  {
  case SHADER_DEFAULT_VERTEX:
    _shader = glCreateShader(GL_VERTEX_SHADER);
    break;
  case SHADER_DEFAULT_FRAGMENT:
    _shader = glCreateShader(GL_FRAGMENT_SHADER);
    break;
  default:
    glDeleteShader(_shader);
    log_error("Invalid shader");
    return 0;
  }
  source_str = shader_load(shader);
  glShaderSource(_shader, 1, &source_str, NULL);
  glCompileShader(_shader);

  glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char info[512] = {0};
    char message[1024] = {0};
    int msg_len = 0;
    glGetShaderInfoLog(_shader, 512, NULL, info);
    glDeleteShader(_shader);
    msg_len = sprintf(message, "Shader compilation failed: %s", info);
    message[msg_len] = '\0';
    log_error(message);
    return 0;
  }

  return _shader;
}

GLuint opengl_program(GLuint vertex_shader, GLuint fragment_shader)
{
  GLint success = 0;
  GLuint program = 0;

  program = glCreateProgram();

  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    char info[512] = {0};
    char message[1024] = {0};
    glGetProgramInfoLog(program, 512, NULL, info);
    sprintf(message, "Shader linking failed: %s", info);
    log_error(message);
    return 0;
  }
  
  return program;
}
