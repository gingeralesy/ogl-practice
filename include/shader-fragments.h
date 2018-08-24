#ifndef SHADER_FRAGMENTS_H
#define SHADER_FRAGMENTS_H

#include <common.h>

/**
 * @brief Enumeration for identifying every shader used.
 */
typedef enum shader_fragment_e
{
 SHADER_NONE = 0x00,
 SHADER_DEFAULT = 0x01,
 SHADER_ID_MASK = 0xfff,

 SHADER_TYPE_VERTEX = 0x1000,
 SHADER_TYPE_FRAGMENT = 0x2000,
 SHADER_TYPE_MASK = 0xf000,

 SHADER_INFO_MASK = 0xffff
} ShaderFragment;

#define VERTEX_SHADER_STR ("vertex") //!< vertex shadder name as a string
#define FRAGMENT_SHADER_STR ("fragment") //!< fragmentshadder name as a string

/**
 * @brief Loads the wanted shader from a fragment file.
 * @param shader id and type value for the shader
 * @return the entire shader fragment as a string
 */
const char * shader_fragment_load(ShaderFragment shader);
/**
 * @brief Reloads the wanted shader from a fragment file.
 * @param shader id and type value for the shader
 * @return the entire shader fragment as a string
 */
const char * shader_fragment_reload(ShaderFragment shader);
/**
 * @brief Converts a ShaderFragment to a GL shader type
 * @param shader id and type value for the shader
 * @return GL shader type
 */
GLenum shader_fragment_type(ShaderFragment shader);
/**
 * @brief Unloads a shader fragment and removes it from memory.
 * @param shader id and type value for the shader
 */
void shader_fragment_unload(ShaderFragment shader);
/**
 * @brief Unloads every shader fragment and removes them from memory.
 */
void shader_fragment_unload_all(void);

#endif // SHADER_FRAGMENTS_H
