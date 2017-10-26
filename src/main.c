#include <common.h>
#include <logger.h>
#include <handlers.h>
#include <opengl.h>
#include <shapes.h>

// Private headers

static int opengl_info(char *);
static GLboolean opengl_setup(GLFWwindow **);

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

// Main

int main(int argc, char *argv[])
{
  GLFWwindow *window = NULL;
  ShapeData triangle = {0};
  ShapeData square = {0};
  ShapeData *shape = NULL;
  if (!opengl_setup(&window))
    return EXIT_FAILURE;
  log_info("OpenGL and GLFW initialised");

  if (!create_square(&square, GL_STATIC_DRAW))
  {
    shape_delete(&square);
    glfwTerminate();
    return EXIT_FAILURE;
  }
  if (!create_triangle(&triangle, GL_STATIC_DRAW))
  {
    shape_delete(&triangle);
    shape_delete(&square);
    glfwTerminate();
    return EXIT_FAILURE;
  }

  log_info("Shape data retrieved");

  shape = &triangle;
  while (!glfwWindowShouldClose(window))
  {
    handle_input(window);
    glClearColor(.3f, .3f, .2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO: Move to input handler
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      shape = NULL;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      shape = &triangle;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      shape = &square;

    if (shape)
    {
      shape_draw(shape);
    }
    else
    {
      shape_draw(&square);
      shape_draw(&triangle);
    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  log_info("Exitting..");

  shape_delete(&triangle);
  shape_delete(&square);
  glfwTerminate();
  log_info("Clean up complete!");

  return EXIT_SUCCESS;
}
