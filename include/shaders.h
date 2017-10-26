#ifndef SHADERS_H
#define SHADERS_H

#include <common.h>

typedef enum shader_fragment_e
{
  SHADER_DEFAULT_VERTEX = 0,
  SHADER_DEFAULT_FRAGMENT,
  SHADER_COUNT
} ShaderFragment;

typedef struct shader_t Shader;

const char * shader_load(ShaderFragment);
void shader_unload(ShaderFragment);
void shader_unload_all(void);

Shader * shader_create(ShaderFragment *, GLuint, ShaderFragment *, GLuint);
GLuint shader_id(Shader *);
void shader_set_boolean(Shader *, const char *, GLboolean);
void shader_set_integer(Shader *, const char *, GLint);
void shader_set_float  (Shader *, const char *, GLfloat);
void shader_use(Shader *);

#endif // SHADERS_H
