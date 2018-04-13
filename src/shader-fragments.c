#include "shader-fragments.h"

#include <logger.h>

// Private members

/**
 * @brief Memory chunk containing all loaded shader fragments.
 */
static char **SHADER_FRAGMENTS_MEM = NULL;

// Private headers

/**
 * @brief Read from a file into a buffer.
 * @param buffer buffer to write into
 * @param filename file to load
 * @return true if success, false otherwise
 */
static GLboolean read_file(char **buffer, const char *filename);

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

// Public functions

const char * shader_fragment_load(ShaderFragment shader)
{
  static const char *SHADER_FRAGMENT_FILES[SHADER_FRAGMENT_COUNT] =
    {
#ifdef _WIN32
      "data\\vertex-default.frag",
      "data\\fragment-default.frag"
#else // POSIX
      "data/vertex-default.frag",
      "data/fragment-default.frag"
#endif // _WIN32 - POSIX
    };

  if (shader < 0 || SHADER_FRAGMENT_COUNT <= shader)
  {
    log_error("Tried to load an invalid shader");
    return NULL;
  }
  
  if (!SHADER_FRAGMENTS_MEM)
    SHADER_FRAGMENTS_MEM =
      (char **)calloc(SHADER_FRAGMENT_COUNT, sizeof(char *));
  if (!SHADER_FRAGMENTS_MEM)
  {
    log_error("Out of memory for shader data array!");
    return NULL;
  }

  if (SHADER_FRAGMENTS_MEM[shader])
    return SHADER_FRAGMENTS_MEM[shader];

  if (read_file(&(SHADER_FRAGMENTS_MEM[shader]),
                SHADER_FRAGMENT_FILES[shader]))
    return SHADER_FRAGMENTS_MEM[shader];
  
  return NULL;
}

const char * shader_fragment_reload(ShaderFragment shader)
{
  if (0 <= shader && shader < SHADER_FRAGMENT_COUNT &&
      SHADER_FRAGMENTS_MEM && SHADER_FRAGMENTS_MEM[shader])
    shader_fragment_unload(shader);
  
  return shader_fragment_load(shader);
}

void shader_fragment_unload(ShaderFragment shader)
{
  if (SHADER_FRAGMENTS_MEM && SHADER_FRAGMENTS_MEM[shader])
  {
    char *tmp = SHADER_FRAGMENTS_MEM[shader];
    SHADER_FRAGMENTS_MEM[shader] = NULL;
    free(tmp);
  }
}

void shader_fragment_unload_all()
{
  if (SHADER_FRAGMENTS_MEM)
  {
    char **mem_tmp = NULL;
    int i = 0;
    for (i = 0; i < SHADER_FRAGMENT_COUNT; i++)
    {
      if (SHADER_FRAGMENTS_MEM[i])
      {
        char *tmp = SHADER_FRAGMENTS_MEM[i];
        SHADER_FRAGMENTS_MEM[i] = NULL;
        free(tmp);
      }
    }
    mem_tmp = SHADER_FRAGMENTS_MEM;
    SHADER_FRAGMENTS_MEM = NULL;
    free(mem_tmp);
  }
}
