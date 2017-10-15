#include "logger.h"

// Private headers

static void print_log(int, LogLevel, const char *);

// Private functions

void print_log(int error, LogLevel level, const char *text)
{
  FILE *output = stderr;
  char *level_string = "UNKN";
  switch (level)
  {
  case LOGGER_DEBUG:
    level_string = "DEBUG";
    break;
  case LOGGER_INFO:
    output = stdout;
    level_string = "INFO";
    break;
  case LOGGER_WARN:
    level_string = "WARN";
    break;
  case LOGGER_ERROR:
    level_string = "ERROR";
    break;
  default:
    break;
  }
  
  if (error)
    fprintf(output, "GLFW %s [%x] - %s\n", level_string, error, text);
  else
    fprintf(output, "%s - %s\n", level_string, text);
  fflush(output);
}

// Public functions

void log_debug(const char *text)
{
  print_log(0, LOGGER_DEBUG, text);
}

void log_info(const char *text)
{
  print_log(0, LOGGER_INFO, text);
}

void log_warning(const char *text)
{
  print_log(0, LOGGER_WARN, text);
}

void log_error(const char *text)
{
  print_log(0, LOGGER_ERROR, text);
}

void log_glfw_error(int error, const char *text)
{
  print_log(error, LOGGER_ERROR, text);
}
