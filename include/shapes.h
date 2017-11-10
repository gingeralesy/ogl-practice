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
  Shader *shader_program;
  GLuint vertex_array;
  GLuint element_buffer;
  GLuint vertex_buffer;
  GLsizei vertex_count;
  GLsizeiptr vertices_size;
  GLsizei index_count;
  GLsizeiptr indices_size;
  GLsizei tex_crd_count;
  GLsizeiptr tex_crd_size;
  GLfloat vertices[UINT16_MAX];
  GLuint indices[UINT16_MAX];
  GLfloat texture_crds[UINT16_MAX];
} ShapeData;

static const GLfloat SHAPE_TRIANGLE_ARR[] =
{
  // x,    y,    z,    r,    g,    b
  -.5f, -.5f,  .0f, 1.0f,  .0f,  .0f, // left
   .5f, -.5f,  .0f,  .0f, 1.0f,  .0f, // right
   .0f,  .5f,  .0f,  .0f,  .0f, 1.0f  // top
};
static const GLuint SHAPE_TRIANGLE_INDICES[] =
{
  0, 1, 2
};
static const GLfloat SHAPE_TRIANGLE_TEX_COORDS[] =
{
  .0f, .0f, // lover left
  1.f, .0f, // lower right
  .5f, 1.f  // upper middle
};

static const GLfloat SHAPE_SQUARE_ARR[] =
{
  // x,    y,    z,    r,    g,    b
   .5f,  .5f,  .0f, 1.0f,  .0f,  .0f, // top-right
   .5f, -.5f,  .0f, 1.0f, 1.0f,  .0f, // bottom-right
  -.5f, -.5f,  .0f,  .0f,  .0f, 1.0f, // bottom-left
  -.5f,  .5f,  .0f,  .0f, 1.0f,  .0f  // top-left
};
static const GLuint SHAPE_SQUARE_INDICES[] =
{
  0, 1, 3, // first triangle
  1, 2, 3  // second triangle
};
static const GLfloat SHAPE_SQUARE_TEX_COORDS[] =
{
  1.f, 1.f, // upper right
  1.f, .0f, // lower right
  .0f, .0f, // lower left
  .0f, 1.f  // upper left
};

GLboolean create_triangle(ShapeData *, GLenum);
GLboolean create_square(ShapeData *, GLenum);

char * shape_data_str(ShapeData *);
void shape_draw(ShapeData *);
void shape_delete(ShapeData *);

#endif // SHAPES_H
