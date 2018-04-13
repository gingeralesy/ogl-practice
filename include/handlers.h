#ifndef HANDLERS_H
#define HANDLERS_H

#include <common.h>

/**
 * @brief Handles the input given to the GLFW window.
 * @param window GLFW window
 */
void handle_input(GLFWwindow *window);
/**
 * @brief Resizes the viewport to given width and height.
 * @param window GLFW window
 * @param width width
 * @param height height
 */
void handle_resize(GLFWwindow *window, GLint width, GLint height);

/**
 * @brief Registers a key handler for GLFW window.
 * @param key key to register the handler for
 * @param handler handler function to handle the key
 * @param args additional args to pass for the handler
 */
void register_keyboard_handler(int key,
                               void (*handler)(GLFWwindow *, int, void *),
                               void *args);
/**
 * @brief Registers the default handlers.
 * 
 * Registers handlers for Escape to close the window, key 1 to use polygon
 * fill mode, key 2 to use polygon line mode.
 */
void register_default_handlers(void);

#endif // HANDLERS_H
