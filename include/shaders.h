#ifndef SHADERS_H
#define SHADERS_H

#include <common.h>
#include <shader-fragments.h>

/**
 * @brief Container for shader ID in GPU memory.
 */
typedef struct shader_t Shader;

/**
 * @brief Creates a shader program in memory and returns a shader program.
 * @param vertices an array of vertex shader IDs
 * @param vertex_count length of vertices
 * @param fragments an array of fragment shader IDs
 * @param fragment_count length of fragments
 * @return encapsulated information on the shader program
 */
Shader * shader_create(ShaderFragment *vertices, GLuint vertex_count,
                       ShaderFragment *fragments, GLuint fragment_count);
/**
 * @brief Deletes the associated shader program from memory.
 * @param shader shader program
 */
void shader_delete(Shader *shader);
/**
 * @brief Gets the ID of the shader program
 * @param shader shader program
 * @return shader ID
 */
GLuint shader_id(Shader *shader);
/**
 * @brief Sets a boolean uniform value in the shader program
 * @param shader shader program
 * @param name boolean value's name
 * @param value value to set it to
 */
void shader_set_boolean(Shader *shader, const char *name, GLboolean value);
/**
 * @brief Sets a integer uniform value in the shader program
 * @param shader shader program
 * @param name boolean value's name
 * @param value value to set it to
 */
void shader_set_integer(Shader *shader, const char *name, GLint value);
/**
 * @brief Sets a float uniform value in the shader program
 * @param shader shader program
 * @param name boolean value's name
 * @param value value to set it to
 */
void shader_set_float(Shader *shader, const char *name, GLfloat value);
/**
 * @brief Refreshes the shader program from the fragments.
 * @param shader shader program
 */
void shader_refresh(Shader *shader);
/**
 * @brief Sets the shader program active in the GPU
 * @param shader shader program
 */
void shader_use(Shader *shader);

#endif // SHADERS_H
