#ifndef SHAPES_H
#define SHAPES_H

#include <common.h>

typedef enum shapes_e
{
  SHAPE_TRIANGLE = 0,
  SHAPE_SQUARE
} Shape;

static const GLfloat SHAPE_TRIANGLE_ARR[] =
{
  -.5f, -.5f, .0f, // left
   .5f, -.5f, .0f, // right
   .0f,  .5f, .0f  // top
};
static const GLuint SHAPE_TRIANGLE_SIZE = (9 * sizeof(GLfloat));
static const GLuint SHAPE_TRIANGLE_INDICES[] =
{
  0, 1, 2
};
static const GLuint SHAPE_TRIANGLE_INDEX_COUNT = 3;
static const GLuint SHAPE_TRIANGLE_INDEX_SIZE = (3 * sizeof(GLuint));

static const GLfloat SHAPE_SQUARE_ARR[] =
{
   .5f,  .5f, .0f, // top-right
   .5f, -.5f, .0f, // bottom-right
  -.5f, -.5f, .0f, // bottom-left
  -.5f,  .5f, .0f  // top-left
};
static const GLuint SHAPE_SQUARE_SIZE = (12 * sizeof(GLfloat));
static const GLuint SHAPE_SQUARE_INDICES[] =
{
  0, 1, 3, // first triangle
  1, 2, 3  // second triangle
};
static const GLuint SHAPE_SQUARE_INDEX_COUNT = 6;
static const GLuint SHAPE_SQUARE_INDEX_SIZE = (6 * sizeof(GLuint));

GLsizei create_triangle(GLuint *, GLuint *, GLenum);
GLsizei create_square(GLuint *, GLuint *, GLenum);

#endif // SHAPES_H
