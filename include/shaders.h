#ifndef SHADERS_H
#define SHADERS_H

#include <common.h>

typedef enum shader_e
{
  SHADER_DEFAULT_VERTEX = 0,
  SHADER_DEFAULT_FRAGMENT,
  SHADER_COUNT
} Shader;

const char * shader_load(Shader);
void shader_prepare_attributes(Shader);
void shader_unload(Shader);
void shader_unload_all(void);

#endif // SHADERS_H
