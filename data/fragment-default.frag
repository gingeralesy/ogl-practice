#version 330 core
in vec4 vertexColour;

out vec4 fragColour;

void main()
{
  fragColour = vec4(1.0, 1.0, 1.0, 1.0);
  fragColour *= vertexColour;
}
