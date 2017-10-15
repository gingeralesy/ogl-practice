#include "handlers.h"

void handle_input(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void handle_resize(GLFWwindow *window, GLint width, GLint height)
{
  glViewport(0, 0, width, height);
}
