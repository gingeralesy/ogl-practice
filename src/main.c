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
  GLuint vertex_buffer = 0, vertex_array = 0, shader_program = 0;
  if (!opengl_setup(&window))
    return EXIT_FAILURE;
  log_info("OpenGL and GLFW initialised");

  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);
    
  vertex_buffer = create_triangle(GL_STATIC_DRAW);
  if (!vertex_buffer)
  {
    glfwTerminate();
    return EXIT_FAILURE;
  }
  log_info("Triangle data retrieved");
  
  shader_program = default_shader_program();
  if (!shader_program)
  {
    glfwTerminate();
    return EXIT_FAILURE;
  }
  log_info("Shader program created");

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  log_info("Vertex attribute pointers set");
  while (!glfwWindowShouldClose(window))
  {
    handle_input(window);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
  return EXIT_SUCCESS;
}