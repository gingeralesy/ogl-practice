#ifndef SHAPES_H
#define SHAPES_H

#include <common.h>
#include <shaders.h>

/**
 * @brief Shapes that we have data for.
 */
typedef enum shapes_e
{
  SHAPE_TRIANGLE = 0,
  SHAPE_SQUARE
} Shape;

/**
 * @brief Container for all the shape data
 */
typedef struct shape_data_t
{
  Shape type;                       /**< The shape type identifier */
  Shader *shader_program;           /**< Shader program for drawing the shape */
  GLuint vertex_array;              /**< Vertex array ID */
  GLuint element_buffer;            /**< Element array buffer ID. */
  GLuint vertex_buffer;             /**< Vertex array buffer ID */
  GLsizei vertex_count;             /**< Number of vertices in the data */
  GLsizeiptr vertices_size;         /**< Size of vertex array in bytes  */
  GLsizei index_count;              /**< Number indices in the data */
  GLsizeiptr indices_size;          /**< Size of index array in bytes */
  GLsizei tex_crd_count;            /**< Texture coordinates */
  GLsizeiptr tex_crd_size;          /**< Size of a texture coordinate data */
  GLfloat vertices[UINT16_MAX];     /**< Vertex array */
  GLuint indices[UINT16_MAX];       /**< Index array */
  GLfloat texture_crds[UINT16_MAX]; /**< Texture coordinate array */
} ShapeData;

/**
 * @brief Triangle shape vertex data
 */
static const GLfloat SHAPE_TRIANGLE_ARR[] =
{
  // x,    y,    z,    r,    g,    b
  -.5f, -.5f,  .0f, 1.0f,  .0f,  .0f, // left
   .5f, -.5f,  .0f,  .0f, 1.0f,  .0f, // right
   .0f,  .5f,  .0f,  .0f,  .0f, 1.0f  // top
};
/**
 * @brief Triangle shape index data
 */
static const GLuint SHAPE_TRIANGLE_INDICES[] =
{
  0, 1, 2
};
/**
 * @brief Triangle shape texture coordinates
 */
static const GLfloat SHAPE_TRIANGLE_TEX_COORDS[] =
{
  .0f, .0f, // lover left
  1.f, .0f, // lower right
  .5f, 1.f  // upper middle
};

/**
 * @brief Square shape vertex data
 */
static const GLfloat SHAPE_SQUARE_ARR[] =
{
  // x,    y,    z,    r,    g,    b
   .5f,  .5f,  .0f, 1.0f,  .0f,  .0f, // top-right
   .5f, -.5f,  .0f, 1.0f, 1.0f,  .0f, // bottom-right
  -.5f, -.5f,  .0f,  .0f,  .0f, 1.0f, // bottom-left
  -.5f,  .5f,  .0f,  .0f, 1.0f,  .0f  // top-left
};
/**
 * @brief Square shape index data
 */
static const GLuint SHAPE_SQUARE_INDICES[] =
{
  0, 1, 3, // first triangle
  1, 2, 3  // second triangle
};
/**
 * @brief Square shape texture coordinates
 */
static const GLfloat SHAPE_SQUARE_TEX_COORDS[] =
{
  1.f, 1.f, // upper right
  1.f, .0f, // lower right
  .0f, .0f, // lower left
  .0f, 1.f  // upper left
};

/**
 * @brief Creates a triangle shape into the data container
 * @param data container to be filled with the data
 * @param data_usage usage pattern of the data store, GL_STATIC_DRAW or GL_DYNAMIC_DRAW
 * @return GL_TRUE if success, GL_FALSE otherwise
 */
GLboolean create_triangle(ShapeData *shape, GLenum data_usage);
/**
 * @brief Creates a square shape into the data container
 * @param data_usage usage pattern of the data store, GL_STATIC_DRAW or GL_DYNAMIC_DRAW
 * @return GL_TRUE if success, GL_FALSE otherwise
 */
GLboolean create_square(ShapeData *shape, GLenum data_usage);
/**
 * @brief Information about the shape in the data
 * @param data shape data
 * @return string format information on the shape
 */
char * shape_data_str(ShapeData *data);
/**
 * @brief Draws the shape using OpeNGL.
 * @param data shape data
 */
void shape_draw(ShapeData *data);
/**
 * @brief Deletes the shape from memory.
 * @param data shape data
 */
void shape_delete(ShapeData *data);
/**
 * @brief Refreshes the shape's shaders.
 * @param data shape data
 */
void shape_refresh(ShapeData *data);

#endif // SHAPES_H
