#include <common.h>
#include <logger.h>
#include <handlers.h>
#include <opengl.h>
#include <shapes.h>

// Private headers

static GLuint default_shader_program();

// Private functions

GLuint default_shader_program()
{
  GLuint vertex_shader = 0;
  GLuint fragment_shader = 0;
  GLuint shader_program = 0;
  
  vertex_shader = opengl_vertex_shader(SHADER_DEFAULT);
  if (!vertex_shader)
    return 0;
  
  fragment_shader = opengl_fragment_shader(SHADER_DEFAULT);
  if (!fragment_shader)
  {
    glDeleteShader(vertex_shader);
    return 0;
  }
  
  shader_program = opengl_program(vertex_shader, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  
  if (!shader_program)
    return 0;
  return shader_program;
}

// Main

int main(int argc, char *argv[])
{
  GLFWwindow *window = NULL;
  GLuint shader_program = 0;
  ShapeData triangle = {0};
  ShapeData square = {0};
  ShapeData *shape = NULL;
  if (!opengl_setup(&window))
    return EXIT_FAILURE;
  log_info("OpenGL and GLFW initialised");

  shader_program = default_shader_program();
  if (!shader_program)
  {
    glfwTerminate();
    return EXIT_FAILURE;
  }
  log_info("Shader program created");

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
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      shape = &triangle;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      shape = &square;

    glUseProgram(shader_program);
    glBindVertexArray(shape->vertex_array);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->element_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, shape->vertex_buffer);
    
    glDrawElements(GL_TRIANGLES, shape->index_count, GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
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
