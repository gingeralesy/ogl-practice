#include "shapes.h"

#include <logger.h>

// Private headers

static GLsizei shape_create(Shape, GLuint *, GLuint *, GLenum);

// Private functions

GLsizei shape_create(Shape shape, GLuint *element_buffer, GLuint *vertex_buffer,
                     GLenum data_usage)
{
  GLsizei element_count = 0;
  GLuint _element_buffer = 0, _vertex_buffer = 0;

  glGenBuffers(1, &_element_buffer);
  glGenBuffers(1, &_vertex_buffer);
  switch(shape)
  {
  case SHAPE_TRIANGLE:
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, SHAPE_TRIANGLE_SIZE,
                 SHAPE_TRIANGLE_ARR, data_usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, SHAPE_TRIANGLE_INDEX_SIZE,
                 SHAPE_TRIANGLE_INDICES, data_usage);
    element_count = SHAPE_TRIANGLE_INDEX_COUNT;
    break;
  case SHAPE_SQUARE:
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, SHAPE_SQUARE_SIZE,
                 SHAPE_SQUARE_ARR, data_usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, SHAPE_SQUARE_INDEX_SIZE,
                 SHAPE_SQUARE_INDICES, data_usage);
    element_count = SHAPE_SQUARE_INDEX_COUNT;
    break;
  default:
    log_error("Invalid shape provided");
    return 0;
  }

  if (glGetError() == GL_INVALID_ENUM)
  {
    log_error("Invalid data usage");
    return 0;
  }

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if (element_buffer)
    (*element_buffer) = _element_buffer;
  if (vertex_buffer)
    (*vertex_buffer) = _vertex_buffer;

  return element_count;
}

// Public functions

GLsizei create_triangle(GLuint *ebo, GLuint *vbo, GLenum data_usage)
{
  return shape_create(SHAPE_TRIANGLE, ebo, vbo, data_usage);
}

GLsizei create_square(GLuint *ebo, GLuint *vbo, GLenum data_usage)
{
  return shape_create(SHAPE_SQUARE, ebo, vbo, data_usage);
}
