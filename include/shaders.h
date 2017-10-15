#ifndef SHADERS_H
#define SHADERS_H

static const char *VERTEX_SHADER_DEFAULT =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
  "}\n\0";

static const char *FRAGMENT_SHADER_DEFAULT =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";

static const char *FRAGMENT_SHADER_SECOND =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(0.7f, 0.3f, 0.2f, 1.0f);\n"
  "}\n\0";

#endif // SHADERS_H
