#include <common.h>
#include <logger.h>
#include <handlers.h>
#include <opengl.h>
#include <shapes.h>

// Main

int main(int argc, char *argv[])
{
  GLFWwindow *window = NULL;
  ShapeData triangle = {0};
  ShapeData square = {0};
  ShapeData *shape = NULL;
  if (!opengl_setup(&window))
    return EXIT_FAILURE;
  log_info("OpenGL and GLFW initialised");

  if (!create_square(&square, GL_STATIC_DRAW))
  {
    shape_delete(&square);
    glfwTerminate();
    return EXIT_FAILURE;
  }
  if (!create_triangle(&triangle, GL_STATIC_DRAW))
  {
    shape_delete(&triangle);
    shape_delete(&square);
    glfwTerminate();
    return EXIT_FAILURE;
  }

  log_info("Shape data retrieved");

  shape = &triangle;
  while (!glfwWindowShouldClose(window))
  {
    handle_input(window);
    glClearColor(.3f, .3f, .2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO: Move to input handler
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      shape = NULL;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      shape = &triangle;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      shape = &square;

    if (shape)
    {
      shape_draw(shape);
    }
    else
    {
      shape_draw(&square);
      shape_draw(&triangle);
    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  log_info("Exitting..");

  shape_delete(&triangle);
  shape_delete(&square);
  glfwTerminate();
  log_info("Clean up complete!");

  return EXIT_SUCCESS;
}
