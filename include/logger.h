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

void print_log(int, LogLevel, const char *);
void log_debug(int, const char *);
void log_info(int, const char *);
void log_warning(int, const char *);
void log_error(int, const char *);

#endif // LOGGER_H
