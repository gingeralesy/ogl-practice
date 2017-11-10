#ifndef HANDLERS_H
#define HANDLERS_H

#include <common.h>

void handle_input(GLFWwindow *);
void handle_resize(GLFWwindow *, GLint, GLint);

void register_keyboard_handler(int, void (*)(GLFWwindow *, int, void *), void *);
void register_default_handlers();

#endif // HANDLERS_H
