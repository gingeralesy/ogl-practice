#include "opengl.h"

#include <logger.h>
#include <handlers.h>
#include <shaders.h>

// Private headers

static int opengl_info(char *);

// Private functions

int opengl_info(char *buffer)
{
  GLint max_v_attribs = 0;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_v_attribs);
  return sprintf(buffer,
                 "OpenGL Info\n"
                 "Maximum nr of vertex attributes: %d",
                 max_v_attribs);
}

// Public functions

GLboolean opengl_setup(GLFWwindow **window)
{
  GLFWwindow *_window = NULL;
  char buffer[1024] = {0};
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

  buffer[opengl_info(buffer)] = '\0';
  log_debug(buffer);

  (*window) = _window;
  
  return GL_TRUE;
}

GLuint opengl_shader(Shader shader)
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
