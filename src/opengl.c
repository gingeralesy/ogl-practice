#include "opengl.h"

#include <logger.h>
#include <handlers.h>
#include <shaders.h>

// Private headers

static GLuint opengl_shader(Shader, GLenum);

// Private functions

GLuint opengl_shader(Shader shader, GLenum shader_type)
{
  GLint success = 0;
  GLuint _shader = 0;

  _shader = glCreateShader(shader_type);
  if (glGetError() == GL_INVALID_ENUM)
  {
    log_error("Invalid shader type");
    return 0;
  }

  switch (shader)
  {
  case SHADER_DEFAULT:
    if (shader_type == GL_VERTEX_SHADER)
      glShaderSource(_shader, 1, &VERTEX_SHADER_DEFAULT, NULL);
    else
      glShaderSource(_shader, 1, &FRAGMENT_SHADER_DEFAULT, NULL);
    break;
  case SHADER_SECOND:
    if (shader_type == GL_VERTEX_SHADER)
      glShaderSource(_shader, 1, &VERTEX_SHADER_DEFAULT, NULL);
    else
      glShaderSource(_shader, 1, &FRAGMENT_SHADER_SECOND, NULL);
    break;
  default:
    glDeleteShader(_shader);
    log_error("Invalid shader");
    return 0;
  }
  glCompileShader(_shader);

  glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char info[512] = {0};
    char message[1024] = {0};
    glGetShaderInfoLog(_shader, 512, NULL, info);
    glDeleteShader(_shader);
    sprintf(message, "Shader compilation failed: %s", info);
    log_error(message);
    return 0;
  }
  
  return _shader;
}

// Public functions

GLboolean opengl_setup(GLFWwindow **window)
{
  GLFWwindow *_window = NULL;
  if (!glfwInit())
  {
    log_error("Failed to initialise GLFW");
    return GL_FALSE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

  _window = glfwCreateWindow(800, 600, "Sandbox", NULL, NULL);
  if (!_window)
  {
    log_error("Failed to create GLFW window");
    glfwTerminate();
    return GL_FALSE;
  }
  glfwMakeContextCurrent(_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    log_error("Failed to initialise GLAD");
    glfwTerminate();
    return GL_FALSE;
  }

  glViewport(0, 0, 800, 600);
  
  glfwSetFramebufferSizeCallback(_window, handle_resize);
  glfwSetErrorCallback((GLFWerrorfun)log_glfw_error);

  (*window) = _window;
  
  return GL_TRUE;
}

GLuint opengl_fragment_shader(Shader shader)
{
  return opengl_shader(shader, GL_FRAGMENT_SHADER);
}

GLuint opengl_vertex_shader(Shader shader)
{
  return opengl_shader(shader, GL_VERTEX_SHADER);
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
