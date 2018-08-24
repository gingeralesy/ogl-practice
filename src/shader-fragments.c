#include "shader-fragments.h"

#include <logger.h>

// Private members

/**
 * @brief Memory chunk containing all loaded shader fragments.
 */
static char **SHADER_FRAGMENTS_MEM = NULL;

/**
 * @brief Number of shader fragments provided for each type of shader.
 */
static GLuint SHADER_FRAGMENT_COUNT = 1;

/**
 * @brief File paths to the vertex shader files in the order of ShaderFragment
 */
static char *VERTEX_SHADER_FILES[3] =
  {
   NULL,
#ifdef _WIN32
   "data\\vertex-default.frag",
#else // POSIX
   "data/vertex-default.frag",
#endif // _WIN32 - POSIX
  };

/**
 * @brief File paths to the fragment shader files in the order of ShaderFragment
 */
static char *FRAGMENT_SHADER_FILES[3] =
  {
   NULL,
#ifdef _WIN32
   "data\\fragment-default.frag"
#else // POSIX
   "data/fragment-default.frag"
#endif // _WIN32 - POSIX
  };

// Private headers

/**
 * @brief Read from a file into a buffer.
 * @param buffer buffer to write into
 * @param filename file to load
 * @return true if success, false otherwise
 */
static GLboolean read_file(char **buffer, const char *filename);

/**
 * @brief Converts a shader fragment ID into an index for the SHADER_FRAGMENTS_MEM.
 * @param shader a shader ID
 * @return a non-negative index for a shader memory slot, -1 if an invalid ID is passed
 */
static GLint shader_memory_index(ShaderFragment shader);

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

GLint shader_memory_index(ShaderFragment shader)
{
  ShaderFragment shader_id = (shader & SHADER_ID_MASK);
  ShaderFragment shader_type = (shader & SHADER_TYPE_MASK);
  
  if (shader_id == SHADER_NONE || shader_type == SHADER_NONE)
    return -1;

  return (((shader_type >> 12) - 1) * SHADER_FRAGMENT_COUNT) + (shader_id - 1);
}


// Public functions

const char * shader_fragment_load(ShaderFragment shader)
{
  GLint shader_ix = shader_memory_index(shader);
  char **file_buffer = NULL;

  if (shader_ix < 0)
  {
    log_error("Tried to load an invalid shader");
    return NULL;
  }

  if (SHADER_FRAGMENTS_MEM == NULL)
    SHADER_FRAGMENTS_MEM =
      (char **)calloc(2 * SHADER_FRAGMENT_COUNT, sizeof(char *));
  if (SHADER_FRAGMENTS_MEM == NULL)
  {
    log_error("Out of memory for shader data array!");
    return NULL;
  }

  if (SHADER_FRAGMENTS_MEM[shader_ix] != NULL)
    return SHADER_FRAGMENTS_MEM[shader_ix];

  file_buffer = &(SHADER_FRAGMENTS_MEM[shader_ix]);
  switch (shader & SHADER_TYPE_MASK)
  {
  case SHADER_TYPE_FRAGMENT:
    if (read_file(file_buffer, FRAGMENT_SHADER_FILES[shader & SHADER_ID_MASK]))
      return SHADER_FRAGMENTS_MEM[shader_ix];
    break;
  case SHADER_TYPE_VERTEX:
    if (read_file(file_buffer, VERTEX_SHADER_FILES[shader & SHADER_ID_MASK]))
      return SHADER_FRAGMENTS_MEM[shader_ix];
    break;
  }
  
  log_error("Failed to read shader '%x'", shader);
  return NULL;
}

const char * shader_fragment_reload(ShaderFragment shader)
{
  if ((shader & SHADER_ID_MASK) == SHADER_NONE ||
      (shader & SHADER_TYPE_MASK) == SHADER_NONE)
  {
    log_error("Tried to load an invalid shader");
    return NULL;
  }
  shader_fragment_unload(shader);
  return shader_fragment_load(shader);
}

GLenum shader_fragment_type(ShaderFragment shader)
{
  switch (shader & SHADER_TYPE_MASK)
  {
  case SHADER_TYPE_FRAGMENT:
    return GL_FRAGMENT_SHADER;
  case SHADER_TYPE_VERTEX:
    return GL_VERTEX_SHADER;
  default:
    break;
  }
  log_error("Invalid shader type for '%x'", shader);
  return 0;
}

void shader_fragment_unload(ShaderFragment shader)
{
  GLint shader_ix = shader_memory_index(shader);

  if (0 <= shader_ix && SHADER_FRAGMENTS_MEM != NULL &&
      SHADER_FRAGMENTS_MEM[shader_ix] != NULL)
  {
    char *tmp = SHADER_FRAGMENTS_MEM[shader_ix];
    SHADER_FRAGMENTS_MEM[shader_ix] = NULL;
    free(tmp);
  }
}

void shader_fragment_unload_all()
{
  if (SHADER_FRAGMENTS_MEM != NULL)
  {
    char **mem_tmp = NULL;
    int i = 0;
    for (i = 0; i < (2 * SHADER_FRAGMENT_COUNT); i++)
    {
      if (SHADER_FRAGMENTS_MEM[i] != NULL)
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
