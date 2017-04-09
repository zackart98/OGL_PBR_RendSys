#version 330 core

layout (location = 0) out vec4 fColor;

uniform vec3 color;

uniform vec3 colors[3];

void main()
{
	fColor = vec4(colors[2], 1.0);
}