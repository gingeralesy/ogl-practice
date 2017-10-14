#include "shapes.h"

#include <logger.h>

static const int valid_data_usage =
  (GL_STATIC_DRAW  | GL_STATIC_READ  | GL_STATIC_COPY |
   GL_STREAM_DRAW  | GL_STREAM_READ  | GL_STREAM_COPY |
   GL_DYNAMIC_DRAW | GL_DYNAMIC_READ | GL_DYNAMIC_COPY);

unsigned int shape_create(Shape shape, GLenum data_usage)
{
  unsigned int vertex_buffer = 0;

  if (!(data_usage & valid_data_usage))
  {
    log_error(0, "Invalid data usage");
    return 0;
  }

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  switch(shape)
  {
  case SHAPE_TRIANGLE:
    glBufferData(GL_ARRAY_BUFFER, SHAPE_TRIANGLE_SIZE, SHAPE_TRIANGLE_ARR,
                 data_usage);
    break;
  default:
    log_error(0, "Invalid shape provided");
    vertex_buffer = 0;
    break;
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return vertex_buffer;
}

unsigned int create_triangle(GLenum data_usage)
{
  return shape_create(SHAPE_TRIANGLE, data_usage);
}
