#include "shapes.h"

#include <logger.h>

GLuint shape_create(Shape shape, GLenum data_usage)
{
  GLuint vertex_buffer = 0;

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  switch(shape)
  {
  case SHAPE_TRIANGLE:
    glBufferData(GL_ARRAY_BUFFER, SHAPE_TRIANGLE_SIZE, SHAPE_TRIANGLE_ARR,
                 data_usage);
    break;
  default:
    log_error("Invalid shape provided");
    vertex_buffer = 0;
    break;
  }

  if (glGetError() == GL_INVALID_ENUM)
  {
    log_error("Invalid data usage");
    return 0;
  }

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
  glEnableVertexAttribArray(0);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return vertex_buffer;
}

GLuint create_triangle(GLenum data_usage)
{
  return shape_create(SHAPE_TRIANGLE, data_usage);
}
