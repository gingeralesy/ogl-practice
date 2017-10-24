#include "shaders.h"

#include <logger.h>

// Private members

static char **SHADERS_MEM = NULL;

// Private headers

static void log_file_error(const char *, const char *);
static GLboolean read_file(char **, const char *);

// Private functions

void log_file_error(const char *format, const char *filename)
{
  int msg_len = 0;
  char message[1024] = {0};
  msg_len = sprintf(message, format, filename);
  message[msg_len] = '\0';

  log_error(message);
}

GLboolean read_file(char **buffer, const char *filename)
{
  GLuint64 filesize = 0, bytes_read = 0;
  char *_buffer = NULL;
  FILE *file = NULL;

  if (!(file = fopen(filename, "r+")))
  {
    log_file_error("Tried to load an invalid shader fragment file: %s", filename);
    return GL_FALSE;
  }

  if (fseek(file, 0L, SEEK_END) != 0)
  {
    log_file_error("Failed to get the size of shader fragment file: %s", filename);
    fclose(file);
    return GL_FALSE;
  }

  filesize = ftell(file);
  rewind(file);

  _buffer = (char *)calloc(filesize + 1, sizeof(char));
  if (!_buffer)
  {
    log_file_error("Out of memory for shader fragment file: %s", filename);
    fclose(file);
    return GL_FALSE;
  }

  bytes_read = fread(_buffer, 1, filesize, file);
  if (bytes_read != filesize)
  {
    log_file_error("Failed to read shader fragment file: %s", filename);
    fclose(file);
    return GL_FALSE;
  }

  _buffer[filesize] = '\0';
  (*buffer) = _buffer;

  fclose(file);

  return GL_TRUE;
}

// Public functions

const char * shader_load(Shader shader)
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

void shader_prepare_attributes(Shader shader)
{
  switch (shader)
  {
  case SHADER_DEFAULT_VERTEX:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    break;
  case SHADER_DEFAULT_FRAGMENT:
    break;
  case SHADER_COUNT:
  default:
    log_error("Tried to prepare an invalid shader");
  }
}

void shader_unload(Shader shader)
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
