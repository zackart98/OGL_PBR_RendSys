#version 330 core

layout (location = 0) out vec4 fColor;
in vec2 texCoords;
in vec3 normal;

uniform vec3 color;
uniform sampler2D tex;

void main()
{
	fColor = vec4(normalize(normal), 1.0);
}