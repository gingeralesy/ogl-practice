#include "shapes.h"

#include <logger.h>

// Private constants

/**
 * @brief Shape's name as a string.
 * 
 * The order has to match Shape enum.
 */
static const char *SHAPE_TYPE_STR[] =
{
  "triangle",
  "square"
};

// Private headers

/**
 * @brief Creates a shape into the data container
 * @param shape which shape is wanted to create
 * @param data container to be filled with the data
 * @param data_usage usage pattern of the data store, GL_STATIC_DRAW or GL_DYNAMIC_DRAW
 * @return GL_TRUE if success, GL_FALSE otherwise
 */
static GLboolean shape_create(Shape shape, ShapeData *data, GLenum data_usage);

// Private functions

GLboolean shape_create(Shape shape, ShapeData *data, GLenum data_usage)
{
  GLuint i = 0;
  const GLfloat *vertices = NULL;
  const GLuint *indices = NULL;
  const GLfloat *tex_coords = NULL;

  ShaderFragment vertex_shader = SHADER_DEFAULT | SHADER_TYPE_VERTEX;
  ShaderFragment fragment_shader = SHADER_DEFAULT | SHADER_TYPE_FRAGMENT;

  if (!data)
  {
    log_error("Must provide proper shape data container");
    return GL_FALSE;
  }

  data->type = shape;
  data->shader_program =
    shader_create(&vertex_shader, 1, &fragment_shader, 1);

  if (!data->shader_program)
  {
    log_error("Could not create shader program for shape '%s'",
              SHAPE_TYPE_STR[shape]);
    return GL_FALSE;
  }
  
  switch(shape)
  {
  case SHAPE_TRIANGLE:
    data->vertices_size = sizeof(SHAPE_TRIANGLE_ARR);
    data->vertex_count = array_length(SHAPE_TRIANGLE_ARR);

    data->indices_size = sizeof(SHAPE_SQUARE_INDICES);
    data->index_count = array_length(SHAPE_SQUARE_INDICES);

    vertices = SHAPE_TRIANGLE_ARR;
    indices = SHAPE_TRIANGLE_INDICES;
    tex_coords = SHAPE_TRIANGLE_TEX_COORDS;
    break;
  case SHAPE_SQUARE:
    data->vertices_size = sizeof(SHAPE_SQUARE_ARR);
    data->vertex_count = array_length(SHAPE_SQUARE_ARR);

    data->indices_size = sizeof(SHAPE_SQUARE_INDICES);
    data->index_count = array_length(SHAPE_SQUARE_INDICES);

    data->tex_crd_size = sizeof(SHAPE_SQUARE_TEX_COORDS);
    data->tex_crd_count = array_length(SHAPE_SQUARE_TEX_COORDS);

    vertices = SHAPE_SQUARE_ARR;
    indices = SHAPE_SQUARE_INDICES;
    tex_coords = SHAPE_SQUARE_TEX_COORDS;
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
  for (i = 0; i < data->tex_crd_count; i++)
    data->texture_crds[i] = tex_coords[i];

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), NULL);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
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
            "vertices      : [\n"
            "  // x,    y,    z,    r,    g,    b,",
            SHAPE_TYPE_STR[data->type],
            data->vertex_count,
            data->index_count);

  for (i = 0; i < data->vertex_count; i++)
  {
    if (i % 6 == 0)
      length += sprintf(tmp + length, "\n  ");

    length += sprintf(tmp + length, "% 4.1f", data->vertices[i]);

    if (i < (data->vertex_count - 1))
      length += sprintf(tmp + length, ", ");
  }

  length +=
    sprintf(tmp + length, "\n]\n"
            "indices       : [ ");

  for (i = 0; i < data->index_count; i++)
  {
    length += sprintf(tmp + length, "%d", data->indices[i]);

    if (i < (data->index_count - 1))
      length += sprintf(tmp + length, ", ");
  }
  length += sprintf(tmp + length, " ]\n");
  
  str = calloc(length + 1, sizeof(char));
  memcpy(str, tmp, length);
  str[length] = '\0';
  free(tmp);
  
  return str;
}

void shape_draw(ShapeData *data)
{
  shader_use(data->shader_program);
  switch(data->type)
  {
  case SHAPE_TRIANGLE:
  case SHAPE_SQUARE:
    shader_set_float(data->shader_program, "colourMultiplier", 1.f);
    shader_set_float(data->shader_program, "xOffset", sin(glfwGetTime()) / 2.f);
    break;
  default:
    break;
  }

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
    if (data->shader_program)
      shader_delete(data->shader_program);
    memset(data, 0, sizeof(ShapeData));
  }
}

void shape_refresh(ShapeData *data)
{
  if (data && data->shader_program)
    shader_refresh(data->shader_program);
}
