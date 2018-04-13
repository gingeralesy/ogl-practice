#ifndef LOGGER_H
#define LOGGER_H

#include <common.h>
#include <stdarg.h>

/**
 * @brief Logging levels
 */
typedef enum log_level_e
{
  LOGGER_DEBUG = 0,    /**< Debug log level */
  LOGGER_INFO,         /**< Info log level */
  LOGGER_WARN,         /**< Warning log level */
  LOGGER_ERROR,        /**< Error log level */
  LOGGER_UNKNOWN_LEVEL /**< Unknown log level */
} LogLevel;

/**
 * @brief Gets the current minimum log level.
 * @return minimum log level
 */
LogLevel log_level(void);
/**
 * @brief Sets the minimum logging level.
 * @param level log level
 */
void set_log_level(LogLevel level);
/**
 * @brief Gets the string representation of the log level.
 * @param level log level
 * @return log level as string (as in "WARN" instead of "WARNING")
 */
const char *log_level_str(LogLevel level);
/**
 * @brief Gets the full string representation of the log level.
 * @param level log level
 * @return full log level as string (as in "WARNING" instead of "WARN")
 */
const char *log_level_str_full(LogLevel level);

/**
 * @brief Prints a debug message.
 * @param format print format
 * @param ... format arguments
 */
void log_debug(const char *format, ...);
/**
 * @brief Prints an info message.
 * @param format print format
 * @param ... format arguments
 */
void log_info(const char *format, ...);
/**
 * @brief Prints a warning message.
 * @param format print format
 * @param ... format arguments
 */
void log_warning(const char *format, ...);
/**
 * @brief Prints an error message.
 * @param format print format
 * @param ... format arguments
 */
void log_error(const char *format, ...);
/**
 * @brief Prints a GLFW error message.
 * 
 * This is used as a callback for GLFW to print error messages with.
 * 
 * @param error error number
 * @param text error text
 */
void log_glfw_error(int error, const char *text);

#endif // LOGGER_H
