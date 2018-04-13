#include "handlers.h"

#include <logger.h>

/**
 * @brief An event handler
 */
typedef struct handler_function_t
{
  int key;        /**< Key that matches this handler */
  void *args;     /**< Extra args passed to the handler function */
  void *function; /**< Handler function */
} HandlerFunction;

/**
 * @brief A callback for window close event.
 * @param window GLFW window
 * @param key key to register the handler for
 * @param args additional args to pass for the handler
 */
static void handle_close_window(GLFWwindow *window, int key, void *args);
/**
 * @brief A callback for polygon fill draw mode.
 * @param window GLFW window
 * @param key key to register the handler for
 * @param args additional args to pass for the handler
 */
static void handle_fill_polygons(GLFWwindow *window, int key, void *args);
/**
 * @brief A callback for polygon line draw mode.
 * @param window GLFW window
 * @param key key to register the handler for
 * @param args additional args to pass for the handler
 */
static void handle_line_polygons(GLFWwindow *window, int key, void *args);

/**
 * @brief Memory chunk for handlers.
 */
static HandlerFunction handler_functions[2048] = {0};
/**
 * @brief Number of handlers in memory.
 */
static GLuint handler_count = 0;

// Private functions

void handle_close_window(GLFWwindow *window, int key, void *args)
{
  glfwSetWindowShouldClose(window, GL_TRUE);
}

void handle_fill_polygons(GLFWwindow *window, int key, void *args)
{
  GLint cur_mode = 0;
  glGetIntegerv(GL_POLYGON_MODE, &cur_mode);
  if (cur_mode != GL_FILL)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void handle_line_polygons(GLFWwindow *window, int key, void *args)
{
  GLint cur_mode = 0;
  glGetIntegerv(GL_POLYGON_MODE, &cur_mode);
  if (cur_mode != GL_LINE)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Public functions

void handle_input(GLFWwindow *window)
{
  GLuint i = 0;
  for (i = 0; i < handler_count; i++)
  {
    HandlerFunction handler = handler_functions[i];
    if (glfwGetKey(window, handler.key) == GLFW_PRESS)
      ((void (*)(GLFWwindow *, int, void *))handler.function)(window, handler.key,
                                                              handler.args);
  }
}

void handle_resize(GLFWwindow *window, GLint width, GLint height)
{
  glViewport(0, 0, width, height);
}

void register_keyboard_handler(int key, void (*handler)(GLFWwindow *, int, void *),
                               void *args)
{
  handler_functions[handler_count].key = key;
  handler_functions[handler_count].args = args;
  handler_functions[handler_count].function = handler;
  handler_count += 1;
}

void register_default_handlers()
{
  register_keyboard_handler(GLFW_KEY_ESCAPE, handle_close_window, NULL);
  register_keyboard_handler(GLFW_KEY_1, handle_fill_polygons, NULL);
  register_keyboard_handler(GLFW_KEY_2, handle_line_polygons, NULL);
}
