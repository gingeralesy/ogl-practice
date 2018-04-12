#include "shaders.h"

#include <logger.h>

// Private structs

typedef struct shader_t
{
  GLuint id;
} Shader;

// Private members

static char **SHADERS_MEM = NULL;

// Private headers

static GLboolean read_file(char **, const char *);
static GLuint shader_compile(GLenum, ShaderFragment *, GLuint);
static GLint shader_get_uniform_loc(GLuint, const char *);

// Private functions

GLboolean read_file(char **buffer, const char *filename)
{
  GLuint64 filesize = 0, bytes_read = 0;
  char *_buffer = NULL;
  FILE *file = NULL;

  if (!(file = fopen(filename, "r+")))
  {
    log_error("Tried to load an invalid shader fragment file: %s", filename);
    return GL_FALSE;
  }

  if (fseek(file, 0L, SEEK_END) != 0)
  {
    log_error("Failed to get the size of shader fragment file: %s", filename);
    fclose(file);
    return GL_FALSE;
  }

  filesize = ftell(file);
  rewind(file);

  _buffer = (char *)calloc(filesize + 1, sizeof(char));
  if (!_buffer)
  {
    log_error("Out of memory for shader fragment file: %s", filename);
    fclose(file);
    return GL_FALSE;
  }

  bytes_read = fread(_buffer, 1, filesize, file);
  if (bytes_read != filesize)
  {
    log_error("Failed to read shader fragment file: %s", filename);
    fclose(file);
    return GL_FALSE;
  }

  _buffer[filesize] = '\0';
  (*buffer) = _buffer;

  fclose(file);

  return GL_TRUE;
}

GLuint shader_compile(GLenum shader_type, ShaderFragment *fragments,
                      GLuint count)
{
  GLint success = 0;
  GLuint shader = 0, i = 0;
  char **shaders = NULL;
  shaders = (char **)calloc(count, sizeof(char *));
  for (i = 0; i < count; i++)
  {
    const char *code = shader_load(fragments[i]);
    shaders[i] = (char *)calloc(strlen(code),sizeof(char));
    strcpy(shaders[i], code);
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
              ((shader_type == GL_VERTEX_SHADER) ? "vertex" : "fragment"),
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

const char * shader_load(ShaderFragment shader)
{
  static const char *SHADER_FILES[SHADER_COUNT] =
  {
#ifdef _WIN32
    "data\\vertex-default.frag",
    "data\\fragment-default.frag"
#else // POSIX
    "data/vertex-default.frag",
    "data/fragment-default.frag"
#endif // _WIN32 - POSIX
  };

  if (shader < 0 || SHADER_COUNT <= shader)
  {
    log_error("Tried to load an invalid shader");
    return NULL;
  }
  
  if (!SHADERS_MEM)
    SHADERS_MEM = (char **)calloc(SHADER_COUNT, sizeof(char *));
  if (!SHADERS_MEM)
  {
    log_error("Out of memory for shader data array!");
    return NULL;
  }

  if (SHADERS_MEM[shader])
    return SHADERS_MEM[shader];

  if (read_file(&(SHADERS_MEM[shader]), SHADER_FILES[shader]))
    return SHADERS_MEM[shader];
  
  return NULL;
}

const char * shader_reload(ShaderFragment shader)
{
  if (0 <= shader && shader < SHADER_COUNT &&
      SHADERS_MEM && SHADERS_MEM[shader])
    shader_unload(shader);
  
  return shader_load(shader);
}

void shader_unload(ShaderFragment shader)
{
  if (SHADERS_MEM && SHADERS_MEM[shader])
  {
    char *shader_tmp = SHADERS_MEM[shader];
    SHADERS_MEM[shader] = NULL;
    free(shader_tmp);
  }
}

void shader_unload_all()
{
  if (SHADERS_MEM)
  {
    char **mem_tmp = NULL;
    int i = 0;
    for (i = 0; i < SHADER_COUNT; i++)
    {
      if (SHADERS_MEM[i])
      {
        char *shader_tmp = SHADERS_MEM[i];
        SHADERS_MEM[i] = NULL;
        free(shader_tmp);
      }
    }
    mem_tmp = SHADERS_MEM;
    SHADERS_MEM = NULL;
    free(mem_tmp);
  }
}

Shader * shader_create(ShaderFragment *vertices, GLuint vertex_count,
                       ShaderFragment *fragments, GLuint fragment_count)
{
  GLuint vertex = 0, fragment = 0;
  GLint success = 0;
  Shader *shader = NULL;

  vertex = shader_compile(GL_VERTEX_SHADER, vertices, vertex_count);
  if (!vertex)
  {
    glDeleteShader(vertex);
    return NULL;
  }

  fragment = shader_compile(GL_FRAGMENT_SHADER, fragments, fragment_count);
  if (!fragment)
  {
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return NULL;
  }

  shader = (Shader *)calloc(1, sizeof(Shader));
  shader->id = glCreateProgram();
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

void shader_use(Shader *shader)
{
  glUseProgram(shader->id);
}
