#include "handlers.h"

void handle_input(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void handle_resize(GLFWwindow *window, GLint width, GLint height)
{
  glViewport(0, 0, width, height);
}
