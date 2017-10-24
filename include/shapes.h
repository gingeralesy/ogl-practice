#ifndef SHAPES_H
#define SHAPES_H

#include <common.h>
#include <shaders.h>

typedef enum shapes_e
{
  SHAPE_TRIANGLE = 0,
  SHAPE_SQUARE
} Shape;

typedef struct shape_data_t
{
  Shape type;
  Shader vertex_shader;
  Shader fragment_shader;
  GLuint shader_program;
  GLuint vertex_array;
  GLuint element_buffer;
  GLuint vertex_buffer;
  GLsizei vertex_count;
  GLsizeiptr vertices_size;
  GLsizei index_count;
  GLsizeiptr indices_size;
  GLfloat vertices[UINT16_MAX];
  GLuint indices[UINT16_MAX];
} ShapeData;

static const GLfloat SHAPE_TRIANGLE_ARR[] =
{
  // x,    y,    z,    r,    g,    b
  -.5f, -.5f,  .0f, 1.0f,  .0f,  .0f, // left
   .5f, -.5f,  .0f,  .0f, 1.0f,  .0f, // right
   .0f,  .5f,  .0f,  .0f,  .0f, 1.0f  // top
};
static const GLuint SHAPE_TRIANGLE_COUNT = 18;
static const GLuint SHAPE_TRIANGLE_SIZE = (18 * sizeof(GLfloat));
static const GLuint SHAPE_TRIANGLE_INDICES[] =
{
  0, 1, 2
};
static const GLuint SHAPE_TRIANGLE_INDEX_COUNT = 3;
static const GLuint SHAPE_TRIANGLE_INDEX_SIZE = (3 * sizeof(GLuint));

static const GLfloat SHAPE_SQUARE_ARR[] =
{
  // x,    y,    z,    r,    g,    b
   .5f,  .5f,  .0f, 1.0f,  .0f,  .0f, // top-right
   .5f, -.5f,  .0f,  .0f, 1.0f,  .0f, // bottom-right
  -.5f, -.5f,  .0f,  .0f,  .0f, 1.0f, // bottom-left
  -.5f,  .5f,  .0f, 1.0f, 1.0f,  .0f  // top-left
};
static const GLuint SHAPE_SQUARE_COUNT = 24;
static const GLuint SHAPE_SQUARE_SIZE = (24 * sizeof(GLfloat));
static const GLuint SHAPE_SQUARE_INDICES[] =
{
  0, 1, 3, // first triangle
  1, 2, 3  // second triangle
};
static const GLuint SHAPE_SQUARE_INDEX_COUNT = 6;
static const GLuint SHAPE_SQUARE_INDEX_SIZE = (6 * sizeof(GLuint));

GLboolean create_triangle(ShapeData *, GLenum);
GLboolean create_square(ShapeData *, GLenum);

char * shape_data_str(ShapeData *);
void shape_draw(ShapeData *);
void shape_delete(ShapeData *);

#endif // SHAPES_H
