#version 330 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexCoords;

out vec2 texCoords;

uniform mat4 mvpMat;


void main()
{
	gl_Position = mvpMat * vPosition;
	//color       = vColor;
	texCoords = vTexCoords.xy;
}