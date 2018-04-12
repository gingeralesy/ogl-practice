#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

uniform float colourMultiplier;
uniform float xOffset;

out vec4 vertexColour;

void main()
{
  gl_Position = vec4(aPos.x + xOffset, aPos.yz, 1.0);
  vertexColour = colourMultiplier * vec4(aColour, 1.0);
}
