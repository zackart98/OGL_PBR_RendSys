#version 330 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexCoords;
layout (location = 2) in mat4 vModelMat;

out vec2 texCoords;

uniform mat4 projMat;


void main()
{
	gl_Position = projMat * vModelMat * vPosition;
	//color       = vColor;
	texCoords = vTexCoords.xy;
}