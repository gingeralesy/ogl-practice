#ifndef LOGGER_H
#define LOGGER_H

#include <common.h>
#include <stdarg.h>

typedef enum log_level_e
{
  LOGGER_DEBUG = 0,
  LOGGER_INFO,
  LOGGER_WARN,
  LOGGER_ERROR,
  LOGGER_UNKNOWN_LEVEL
} LogLevel;

LogLevel log_level();
void set_log_level(LogLevel);
const char *log_level_str(LogLevel);
const char *log_level_str_full(LogLevel);

void log_debug(const char *, ...);
void log_info(const char *, ...);
void log_warning(const char *, ...);
void log_error(const char *, ...);
void log_glfw_error(int, const char *);

#endif // LOGGER_H
