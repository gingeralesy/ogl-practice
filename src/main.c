#include <common.h>
#include <logger.h>
#include <handlers.h>
#include <opengl.h>
#include <shapes.h>

// Main

int main(int argc, char *argv[])
{
  GLFWwindow *window = NULL;
  unsigned int vertex_buffer = 0;
  if (!opengl_setup(&window))
    return EXIT_FAILURE;

  vertex_buffer = create_triangle(GL_STATIC_DRAW);

  while (!glfwWindowShouldClose(window))
  {
    handle_input(window);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
  return EXIT_SUCCESS;
}
