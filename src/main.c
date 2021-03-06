#include <common.h>
#include <logger.h>
#include <handlers.h>
#include <shapes.h>

// Private structs

/**
 * @brief Keeps track of the state which shapes are shown and how many there are.
 */
typedef struct shape_state_t
{
  int count;             /**< Number of shapes in memory */
  ShapeData *current;    /**< Current shape displayed */
  ShapeData *shapes[32]; /**< Shapes in memory */
} ShapeState;

// Private headers

/**
 * @brief Prints the Open GL infor into the buffer.
 * @param buffer Buffer to write into
 * @return length of the data written into the buffer
 */
static int opengl_info(char *buffer);
/**
 * @brief Sets up OpeNGL and fills the window pointer with an initialised window
 * @param window pointer to the window to be filled
 * @return true if success
 */
static GLboolean opengl_setup(GLFWwindow **window);
/**
 * @brief Callback for keyboard event where shape is switched.
 * @param win GLFW window
 * @param key key pressed
 * @param arg additional callback data argument, this is ignored
 */
static void switch_shape(GLFWwindow *win, int key, void *arg);
/**
 * @brief Adds a shape into the shape state
 * @param shapes Shape state in memory
 * @param shape Shape to add into state
 * @return true if success
 */
static GLboolean add_shape(ShapeState *shapes, Shape shape);
/**
 * @brief Adds all known shapes into the shape state
 * @param shapes Shape state in memory
 * @return true if success
 */
static GLboolean add_shapes(ShapeState *shapes);
/**
 * @brief Removes all shapes from memory.
 * @param shapes Shape state in memory
 */
static void remove_shapes(ShapeState *shapes);

// Private functions

int opengl_info(char *buffer)
{
  GLint max_v_attribs = 0;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_v_attribs);
  return sprintf(buffer,
                 "OpenGL Info\n"
                 "Maximum nr of vertex attributes: %d",
                 max_v_attribs);
}

GLboolean opengl_setup(GLFWwindow **window)
{
  GLFWwindow *_window = NULL;
  char buffer[1024] = {0};
  if (!glfwInit())
  {
    log_error("Failed to initialise GLFW");
    return GL_FALSE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

  _window = glfwCreateWindow(800, 600, "Sandbox", NULL, NULL);
  if (!_window)
  {
    log_error("Failed to create GLFW window");
    glfwTerminate();
    return GL_FALSE;
  }
  glfwMakeContextCurrent(_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    log_error("Failed to initialise GLAD");
    glfwTerminate();
    return GL_FALSE;
  }

  glViewport(0, 0, 800, 600);
  
  glfwSetFramebufferSizeCallback(_window, handle_resize);
  glfwSetErrorCallback((GLFWerrorfun)log_glfw_error);

  buffer[opengl_info(buffer)] = '\0';
  log_debug(buffer);

  (*window) = _window;
  
  return GL_TRUE;
}

void switch_shape(GLFWwindow *win, int key, void *arg)
{
  ShapeState *shapes = (ShapeState *)arg;
  int i = 0;

  switch (key)
  {
  case GLFW_KEY_A:
    shapes->current = shapes->shapes[0];
    break;
  case GLFW_KEY_S:
    shapes->current = shapes->shapes[1];
    break;
  case GLFW_KEY_D:
    shapes->current = NULL;
    break;
  case GLFW_KEY_R:
    for (i = 0; i < shapes->count; i++)
    {
      if (shapes->shapes[i])
        shape_refresh(shapes->shapes[i]);
    }
    break;
  }
}

GLboolean add_shape(ShapeState *shapes, Shape shape)
{
  GLboolean success = GL_FALSE;
  shapes->shapes[shapes->count] = (ShapeData *)calloc(1, sizeof(ShapeData));
  switch(shape)
  {
  case SHAPE_TRIANGLE:
    success = create_triangle(shapes->shapes[shapes->count], GL_STATIC_DRAW);
    break;
  case SHAPE_SQUARE:
    success = create_square(shapes->shapes[shapes->count], GL_STATIC_DRAW);
    break;
  }
  shapes->count += 1;
  return success;
}

GLboolean add_shapes(ShapeState *shapes)
{
  if (!add_shape(shapes, SHAPE_SQUARE))
    return GL_FALSE;
  if (!add_shape(shapes, SHAPE_TRIANGLE))
    return GL_FALSE;
  return GL_TRUE;
}

void remove_shapes(ShapeState *shapes)
{
  int i = 0;
  for (i = 0; i < shapes->count; i++)
    shape_delete(shapes->shapes[i]);
  memset(shapes, 0, sizeof(ShapeState));
}

// Main

int main(int argc, char *argv[])
{
  int i = 0;
  GLFWwindow *window = NULL;
  ShapeState shapes = {0};
  if (!opengl_setup(&window))
    return EXIT_FAILURE;
  log_info("OpenGL and GLFW initialised");

  if (!add_shapes(&shapes))
  {
    remove_shapes(&shapes);
    glfwTerminate();
    return EXIT_FAILURE;
  }
  log_info("Shape data retrieved");

  register_default_handlers();
  register_keyboard_handler(GLFW_KEY_A, switch_shape, (void *)&shapes);
  register_keyboard_handler(GLFW_KEY_S, switch_shape, (void *)&shapes);
  register_keyboard_handler(GLFW_KEY_D, switch_shape, (void *)&shapes);
  register_keyboard_handler(GLFW_KEY_R, switch_shape, (void *)&shapes);
  log_info("Handlers registered");

  shapes.current = shapes.shapes[1];
  while (!glfwWindowShouldClose(window))
  {
    handle_input(window);
    glClearColor(.3f, .3f, .2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (shapes.current)
    {
      shape_draw(shapes.current);
    }
    else
    {
      for (i = 0; i < shapes.count; i++)
        shape_draw(shapes.shapes[i]);
    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  log_info("Exitting..");

  remove_shapes(&shapes);
  glfwTerminate();
  log_info("Clean up complete!");

  return EXIT_SUCCESS;
}
