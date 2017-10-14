#include "logger.h"

void print_log(int error, LogLevel level, const char *text)
{
  char *level_string = "UNKN";
  switch (level)
  {
  case LOGGER_DEBUG:
    level_string = "DEBUG";
    break;
  case LOGGER_INFO:
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
    fprintf(stderr, "GLFW %s [%d] - %s\n", level_string, error, text);
  else
    fprintf(stderr, "%s - %s\n", level_string, text);
}

void log_debug(int error, const char *text)
{
  print_log(error, LOGGER_DEBUG, text);
}

void log_info(int error, const char *text)
{
  print_log(error, LOGGER_INFO, text);
}

void log_warning(int error, const char *text)
{
  print_log(error, LOGGER_WARN, text);
}

void log_error(int error, const char *text)
{
  print_log(error, LOGGER_ERROR, text);
}
