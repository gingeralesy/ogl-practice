#include "logger.h"

// Private constants

/**
 * @brief Names of the log levels for output.
 */
static const char *LOG_LEVEL_NAME[5] =
{
  "DEBUG", "INFO", "WARN", "ERROR", "UNKN"
};

/**
 * @brief Full names of the log levels.
 */
static const char *LOG_LEVEL_NAME_FULL[5] =
{
  "DEBUG", "INFO", "WARNING", "ERROR", "UNKNOWN"
};

// Private variables

/**
 * @brief Minimum level to be logged.
 */
static LogLevel min_level = LOGGER_INFO;

// Private headers

/**
 * @brief Prints log into output.
 * @param error if non-zero, this is expected to be a GLFW error message
 * @param level log event level
 * @param format the print format
 * @param args print args for the format
 */
static void print_log(int error, LogLevel level, const char *format,
                      va_list args);

// Private functions

void print_log(int error, LogLevel level, const char *format, va_list args)
{
  FILE *output = stderr;
  char text[1024] = {0};
  int msg_len = 0;

  if (level < min_level)
    return;
  if (LOGGER_UNKNOWN_LEVEL < level)
    level = LOGGER_UNKNOWN_LEVEL;

  if (level == LOGGER_INFO)
    output = stdout;

  msg_len = vsnprintf(text, 1024, format, args);

  text[msg_len] = '\0';
  
  if (error)
    fprintf(output, "GLFW %s [%x] - %s\n", log_level_str(level), error, text);
  else
    fprintf(output, "%s - %s\n", log_level_str(level), text);
  fflush(output);
}

// Public functions

LogLevel log_level()
{
  return min_level;
}

void set_log_level(LogLevel level)
{
  if (level < LOGGER_DEBUG)
  {
    log_warning("Too low logger level, setting to DEBUG.");
    min_level = LOGGER_DEBUG;
  }
  else if (LOGGER_UNKNOWN_LEVEL <= level)
  {
    log_warning("Too high logger level, setting to ERROR.");
    min_level = LOGGER_ERROR;
  }
  else
  {
    char message[32] = {0};
    sprintf(message, "Logging set to %s", log_level_str_full(level));
    log_debug(message);
    min_level = level;
  }
}

const char *log_level_str(LogLevel level)
{
  if (level < LOGGER_DEBUG || LOGGER_UNKNOWN_LEVEL <= level)
    return LOG_LEVEL_NAME[LOGGER_UNKNOWN_LEVEL];
  return LOG_LEVEL_NAME[level];
}

const char *log_level_str_full(LogLevel level)
{
  if (level < LOGGER_DEBUG || LOGGER_UNKNOWN_LEVEL <= level)
    return LOG_LEVEL_NAME_FULL[LOGGER_UNKNOWN_LEVEL];
  return LOG_LEVEL_NAME_FULL[level];
}

void log_debug(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  print_log(0, LOGGER_DEBUG, format, args);
  va_end(args);
}

void log_info(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  print_log(0, LOGGER_INFO, format, args);
  va_end(args);
}

void log_warning(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  print_log(0, LOGGER_WARN, format, args);
  va_end(args);
}

void log_error(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  print_log(0, LOGGER_ERROR, format, args);
  va_end(args);
}

void log_glfw_error(int error, const char *text)
{
  va_list dummy = {0};
  print_log(error, LOGGER_ERROR, text, dummy);
}
