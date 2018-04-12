#ifndef SHADERS_H
#define SHADERS_H

#include <common.h>

/**
 * @brief Enumeration for identifying every shader used.
 */
typedef enum shader_fragment_e
{
  SHADER_DEFAULT_VERTEX = 0,
  SHADER_DEFAULT_FRAGMENT,
  SHADER_COUNT
} ShaderFragment;

/**
 * @brief Container for shader ID in GPU memory.
 */
typedef struct shader_t Shader;

/**
 * @brief Loads the wanted shader from a fragment file.
 * @param shader enumeration id value for the shader
 * @return the entire shader fragment as a string
 */
const char * shader_load(ShaderFragment shader);
/**
 * @brief Reloads the wanted shader from a fragment file.
 * @param shader enumeration id value for the shader
 * @return the entire shader fragment as a string
 */
const char * shader_reload(ShaderFragment shader);
/**
 * @brief Unloads a shader fragment and removes it from memory.
 * @param shader enumeration id value for the shader
 */
void shader_unload(ShaderFragment shader);
/**
 * @brief Unloads every shader fragment and removes them from memory.
 */
void shader_unload_all(void);

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
 * @brief Sets the shader program active in the GPU
 * @param shader shader program
 */
void shader_use(Shader *shader);

#endif // SHADERS_H
