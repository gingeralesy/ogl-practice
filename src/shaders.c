#include "shaders.h"

#include <logger.h>

/**
 * @brief Total number of fragments supported by a shader program.
 */
#define SHADER_MAX_FRAGMENTS (16)

// Private structs

/**
 * @brief Container for information on a shader program.
 */
typedef struct shader_t
{
  GLuint id;                                             /**< Shader program ID from OpenGL */
  GLuint vertex_shader_count;                            /**< Number of vertex shaders */
  GLuint fragment_shader_count;                          /**< Number of fragment shaders */
  ShaderFragment vertex_shaders[SHADER_MAX_FRAGMENTS];   /**< Vertex shaders */
  ShaderFragment fragment_shaders[SHADER_MAX_FRAGMENTS]; /**< Fragment shaders */
} Shader;

// Private headers

/**
 * @brief Compiles a shader in the GPU
 * @param fragments shader fragments
 * @param count number of shader fragments
 * @return shader ID
 */
static GLuint shader_compile(ShaderFragment *fragments,
                             GLuint count);
/**
 * @brief Get the location of the uniform in the shader.
 * @param shader ID of a shader program
 * @param name name of the uniform
 * @return non-negative integer on success, -1 otherwise
 */
static GLint shader_get_uniform_loc(GLuint shader, const char *name);

// Private functions

GLuint shader_compile(ShaderFragment *fragments, GLuint count)
{
  GLint success = 0;
  GLuint shader = 0, i = 0;
  GLenum shader_type = 0;
  char **shaders = NULL;

  if (count == 0)
  {
    log_error("Nothing to compile!");
    return 0;
  }

  shader_type = shader_fragment_type(fragments[0]);
  if (shader_type == 0)
  {
    log_error("Missing shader type info from '%x'", fragments[0]);
    return 0;
  }

  shaders = (char **)calloc(count, sizeof(char *));
  if (shaders == NULL)
  {
    log_error("Out of memory for shader data array.");
    return 0;
  }

  for (i = 0; i < count; i++)
  {
    GLenum type = 0;
    const char *code = NULL;

    type = shader_fragment_type(fragments[i]);
    if (type == 0)
    {
      log_error("Missing shader type info from '%x'", fragments[i]);
      shader_type = 0;
      break;
    }

    if (shader_type == type)
    {
      code = shader_fragment_load(fragments[i]);
      if (code != NULL)
      {
        shaders[i] = (char *)calloc(strlen(code),sizeof(char));
        strcpy(shaders[i], code);
      }
      else
      {
        log_error("Out of memory for shader data.");
      }
    }
    else
    {
      log_error("Shader type mismatch!");
    }

    if (code == NULL)
    {
      GLuint j = 0;
      char *tmp = NULL;

      for (j = 0; j < i; j++)
      {
        tmp = shaders[i];
        shaders[i] = NULL;
        free(tmp);
      }
      free(shaders);
      
      return 0;
    }
  }

  shader = glCreateShader(shader_type);
  glShaderSource(shader, count, (const char **)shaders, NULL);
  glCompileShader(shader);
  for (i = 0; i < count; i++)
    free(shaders[i]);
  free(shaders);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char info_log[512] = {0};
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    log_error("Failed to compile %s shader: %s",
              shader_type == GL_VERTEX_SHADER
              ? VERTEX_SHADER_STR : FRAGMENT_SHADER_STR,
              info_log);
  }
  return shader;
}

GLint shader_get_uniform_loc(GLuint shader, const char *name)
{
  GLint location = glGetUniformLocation(shader, name);
  if (location < 0)
  {
    log_error("Could not get uniform location for '%s'", name);
    return -1;
  }
  return location;
}

// Public functions

Shader * shader_create(ShaderFragment *vertices, GLuint vertex_count,
                       ShaderFragment *fragments, GLuint fragment_count)
{
  GLuint vertex = 0, fragment = 0;
  GLint success = 0;
  GLuint i = 0;
  Shader *shader = NULL;

  vertex = shader_compile(vertices, vertex_count);
  if (!vertex)
  {
    glDeleteShader(vertex);
    return NULL;
  }

  fragment = shader_compile(fragments, fragment_count);
  if (!fragment)
  {
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return NULL;
  }

  shader = (Shader *)calloc(1, sizeof(Shader));
  shader->id = glCreateProgram();
  for (i = 0; i < vertex_count; i++)
    shader->vertex_shaders[i] = vertices[i];
  shader->vertex_shader_count = vertex_count;
  for (i = 0; i < fragment_count; i++)
    shader->fragment_shaders[i] = fragments[i];
  shader->fragment_shader_count = fragment_count;

  glAttachShader(shader->id, vertex);
  glAttachShader(shader->id, fragment);
  glLinkProgram(shader->id);

  glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
  if (!success)
  {
    char info_log[512] = {0};
    glGetShaderInfoLog(fragment, 512, NULL, info_log);
    log_error("Failed to link shader program: %s", info_log);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    free(shader);
    return NULL;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return shader;
}

void shader_delete(Shader *shader)
{
  if (shader)
  {
    Shader *tmp = shader;
    shader = NULL;
    
    glDeleteProgram(tmp->id);
    free(tmp);
  }
}

GLuint shader_id(Shader *shader)
{
  return shader->id;
}

void shader_set_boolean(Shader *shader, const char *name, GLboolean value)
{
  glUniform1i(shader_get_uniform_loc(shader->id, name), (GLint)value);
}

void shader_set_integer(Shader *shader, const char *name, GLint value)
{
  glUniform1i(shader_get_uniform_loc(shader->id, name), value);
}

void shader_set_float(Shader *shader, const char *name, GLfloat value)
{
  glUniform1f(shader_get_uniform_loc(shader->id, name), value);
}

void shader_refresh(Shader *shader)
{
  GLuint i = 0;
  Shader *tmp = NULL;
  log_debug("Refreshing shader program.");

  for (i = 0; i < shader->vertex_shader_count; i++)
    shader_fragment_unload(shader->vertex_shaders[i]);
  for (i = 0; i < shader->fragment_shader_count; i++)
    shader_fragment_unload(shader->fragment_shaders[i]);

  tmp =
    shader_create(shader->vertex_shaders, shader->vertex_shader_count,
                  shader->fragment_shaders, shader->fragment_shader_count);

  glDeleteProgram(shader->id);
  shader->id = tmp->id;
  free(tmp);
}

void shader_use(Shader *shader)
{
  glUseProgram(shader->id);
}
