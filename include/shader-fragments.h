#ifndef SHADER_FRAGMENTS_H
#define SHADER_FRAGMENTS_H

#include <common.h>

/**
 * @brief Enumeration for identifying every shader used.
 */
typedef enum shader_fragment_e
{
  SHADER_DEFAULT_VERTEX = 0,
  SHADER_DEFAULT_FRAGMENT,
  SHADER_FRAGMENT_COUNT
} ShaderFragment;

/**
 * @brief Shader fragment name as a string.
 * 
 * The order has to match ShaderFragment enum.
 */
static const char *SHADER_FRAGMENT_TYPE_STR[] =
{
  "vertex",
  "fragment"
};

/**
 * @brief Loads the wanted shader from a fragment file.
 * @param shader enumeration id value for the shader
 * @return the entire shader fragment as a string
 */
const char * shader_fragment_load(ShaderFragment shader);
/**
 * @brief Reloads the wanted shader from a fragment file.
 * @param shader enumeration id value for the shader
 * @return the entire shader fragment as a string
 */
const char * shader_fragment_reload(ShaderFragment shader);
/**
 * @brief Unloads a shader fragment and removes it from memory.
 * @param shader enumeration id value for the shader
 */
void shader_fragment_unload(ShaderFragment shader);
/**
 * @brief Unloads every shader fragment and removes them from memory.
 */
void shader_fragment_unload_all(void);

#endif // SHADER_FRAGMENTS_H
