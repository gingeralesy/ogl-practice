#include "shapes.h"

#include <logger.h>
#include <opengl.h>

// Private constants

static const char *SHAPE_TYPE_STR[] =
{
  "triangle",
  "square"
};

// Private headers

static GLuint shader_program(Shape);
static GLboolean shape_create(Shape, ShapeData *, GLenum);

// Private functions

GLuint shader_program(Shape shape)
{
  GLuint vertex_shader = 0;
  GLuint fragment_shader = 0;
  GLuint shader_program = 0;
  
  vertex_shader = opengl_vertex_shader(SHADER_DEFAULT);
  if (!vertex_shader)
    return 0;

  switch (shape)
  {
  case SHAPE_TRIANGLE:
    fragment_shader = opengl_fragment_shader(SHADER_DEFAULT);
    break;
  case SHAPE_SQUARE:
    fragment_shader = opengl_fragment_shader(SHADER_SECOND);
    break;
  default:
    log_error("Invalid shape");
    break;
  }

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

GLboolean shape_create(Shape shape, ShapeData *data, GLenum data_usage)
{
  GLuint i = 0;
  const GLfloat *vertices = NULL;
  const GLuint *indices = NULL;
  if (!data)
  {
    log_error("Must provide proper shape data container");
    return GL_FALSE;
  }

  data->type = shape;
  data->shader_program = shader_program(shape);
  
  switch(shape)
  {
  case SHAPE_TRIANGLE:
    data->vertex_count = SHAPE_TRIANGLE_COUNT;
    data->vertices_size = SHAPE_TRIANGLE_SIZE;
    data->index_count = SHAPE_TRIANGLE_INDEX_COUNT;
    data->indices_size = SHAPE_TRIANGLE_INDEX_SIZE;

    vertices = SHAPE_TRIANGLE_ARR;
    indices = SHAPE_TRIANGLE_INDICES;
    break;
  case SHAPE_SQUARE:
    data->vertex_count = SHAPE_SQUARE_COUNT;
    data->vertices_size = SHAPE_SQUARE_SIZE;
    data->index_count = SHAPE_SQUARE_INDEX_COUNT;
    data->indices_size = SHAPE_SQUARE_INDEX_SIZE;

    vertices = SHAPE_SQUARE_ARR;
    indices = SHAPE_SQUARE_INDICES;
    break;
  default:
    log_error("Invalid shape provided");
    return GL_FALSE;
  }

  glGenVertexArrays(1, &(data->vertex_array));
  glBindVertexArray(data->vertex_array);

  for (i = 0; i < data->vertex_count; i++)
    data->vertices[i] = vertices[i];
  for (i = 0; i < data->index_count; i++)
    data->indices[i] = indices[i];

  glGenBuffers(1, &data->element_buffer);
  glGenBuffers(1, &data->vertex_buffer);

  glBindBuffer(GL_ARRAY_BUFFER, data->vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, data->vertices_size, data->vertices,
               data_usage);

  if (glGetError() == GL_INVALID_ENUM)
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    memset(data, 0, sizeof(ShapeData));
    log_error("Invalid data usage");
    return GL_FALSE;
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->element_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->indices_size, data->indices,
               data_usage);

  if (glGetError() == GL_INVALID_ENUM)
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    memset(data, 0, sizeof(ShapeData));
    log_error("Invalid data usage");
    return GL_FALSE;
  }

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return GL_TRUE;
}

// Public functions

GLboolean create_triangle(ShapeData *shape, GLenum data_usage)
{
  return shape_create(SHAPE_TRIANGLE, shape, data_usage);
}

GLboolean create_square(ShapeData *shape, GLenum data_usage)
{
  return shape_create(SHAPE_SQUARE, shape, data_usage);
}

char * shape_data_str(ShapeData *data)
{
  int length = 0, i = 0;
  char *tmp = calloc(1024, sizeof(char));
  char *str = NULL;
  length = 
    sprintf(tmp,
            "Shape Data\n"
            "type          : %s\n"
            "vertex count  : %d\n"
            "index count   : %d\n"
            "vertices      : [",
            SHAPE_TYPE_STR[data->type],
            data->vertex_count,
            data->index_count);

  for (i = 0; i < data->vertex_count; i++)
  {
    if (i % 3 == 0)
      length += sprintf(tmp + length, "\n  ");

    length += sprintf(tmp + length, "% 4.1f", data->vertices[i]);

    if (i < (data->vertex_count - 1))
      length += sprintf(tmp + length, ", ");
    else
      length += sprintf(tmp + length, "\n");
  }

  length +=
    sprintf(tmp + length, "]\n"
            "indices       : [");

  for (i = 0; i < data->index_count; i++)
  {
    if (i % 3 == 0)
      length += sprintf(tmp + length, "\n  ");

    length += sprintf(tmp + length, "%d", data->indices[i]);

    if (i < (data->index_count - 1))
      length += sprintf(tmp + length, ", ");
    else
      length += sprintf(tmp + length, "\n");
  }
  length += sprintf(tmp + length, "]\n");
  
  str = calloc(length + 1, sizeof(char));
  memcpy(str, tmp, length);
  str[length] = '\0';
  free(tmp);
  
  return str;
}

void shape_draw(ShapeData *data)
{
  glUseProgram(data->shader_program);
  glBindVertexArray(data->vertex_array);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->element_buffer);
  
  glDrawElements(GL_TRIANGLES, data->index_count, GL_UNSIGNED_INT, 0);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void shape_delete(ShapeData *data)
{
  if (data)
  {
    if (data->vertex_array)
      glDeleteVertexArrays(1, &(data->vertex_array));
    if (data->vertex_buffer)
      glDeleteBuffers(1, &(data->vertex_buffer));
    if (data->element_buffer)
      glDeleteBuffers(1, &(data->element_buffer));
    memset(data, 0, sizeof(ShapeData));
  }
}
