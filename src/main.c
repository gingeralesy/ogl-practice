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
  GLsizei element_count = 0;
  GLuint element_buffer = 0, vertex_buffer = 0, vertex_array = 0,
    shader_program = 0;
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

  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  element_count =
    create_square(&element_buffer, &vertex_buffer, GL_STATIC_DRAW);
  glBindVertexArray(0);
  if (element_count <= 0)
  {
    glDeleteVertexArrays(1, &vertex_array);
    glfwTerminate();
    return EXIT_FAILURE;
  }
  log_info("Shape data retrieved");

  while (!glfwWindowShouldClose(window))
  {
    handle_input(window);
    glClearColor(.3f, .3f, .2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    
    glDrawElements(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertex_array);
  glDeleteBuffers(1, &vertex_buffer);
  glDeleteBuffers(1, &element_buffer);
  glfwTerminate();

  return EXIT_SUCCESS;
}
