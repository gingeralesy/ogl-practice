#include "logger.h"

// Private constants

static const char *LOG_LEVEL_NAME[5] =
  {
    "DEBUG", "INFO", "WARN", "ERROR", "UNKN"
  };

static const char *LOG_LEVEL_NAME_FULL[5] =
  {
    "DEBUG", "INFO", "WARNING", "ERROR", "UNKNOWN"
  };

// Private variables

static LogLevel min_level = LOGGER_INFO;

// Private headers

static void print_log(int, LogLevel, const char *);

// Private functions

void print_log(int error, LogLevel level, const char *text)
{
  FILE *output = stderr;

  if (level < min_level)
    return;
  if (LOGGER_UNKNOWN_LEVEL < level)
    level = LOGGER_UNKNOWN_LEVEL;

  if (level == LOGGER_INFO)
    output = stdout;

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
