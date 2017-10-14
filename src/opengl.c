#include "opengl.h"

#include <logger.h>
#include <handlers.h>

GLboolean opengl_setup(GLFWwindow **window)
{
  GLFWwindow *_window = NULL;
  if (!glfwInit())
  {
    log_error(0, "Failed to initialise GLFW");
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
    log_error(0, "Failed to create GLFW window");
    glfwTerminate();
    return GL_FALSE;
  }
  glfwMakeContextCurrent(_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    log_error(0, "Failed to initialise GLAD");
    glfwTerminate();
    return GL_FALSE;
  }

  glViewport(0, 0, 800, 600);
  glClearColor(.3f, .3f, .2f, 1.f);
  
  glfwSetFramebufferSizeCallback(_window, handle_resize);

  (*window) = _window;
  
  return GL_TRUE;
}
