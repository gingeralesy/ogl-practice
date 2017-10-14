#ifndef SHAPES_H
#define SHAPES_H

#include <common.h>

typedef enum shapes_e
{
  SHAPE_TRIANGLE = 0
} Shape;

static const float SHAPE_TRIANGLE_ARR[] =
{
  -.5f, -.5f, .0f, // left
   .5f, -.5f, .0f, // right
   .0f,  .5f, .0f  // top
};
static const unsigned int SHAPE_TRIANGLE_LENGTH = 9;
static const unsigned int SHAPE_TRIANGLE_SIZE = (9 * sizeof(float));

unsigned int shape_create(Shape, GLenum);
unsigned int create_triangle(GLenum);

#endif // SHAPES_H
