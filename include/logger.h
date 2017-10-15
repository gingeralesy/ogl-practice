#ifndef LOGGER_H
#define LOGGER_H

#include <common.h>

typedef enum log_level_e
{
  LOGGER_DEBUG,
  LOGGER_INFO,
  LOGGER_WARN,
  LOGGER_ERROR
} LogLevel;

void log_debug(const char *);
void log_info(const char *);
void log_warning(const char *);
void log_error(const char *);
void log_glfw_error(int, const char *);

#endif // LOGGER_H
