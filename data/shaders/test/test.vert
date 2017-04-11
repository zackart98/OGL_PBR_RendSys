#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTangent;
layout (location = 3) in vec2 vTexCoords;
layout (location = 4) in mat4 vModelMat;

out vec2 texCoords;
out vec3 normal;

uniform mat4 projMat;
uniform mat4 viewMat;

void main()
{
	gl_Position = projMat * viewMat * vModelMat * vec4(vPosition, 1.0);

	normal = transpose(inverse(mat3(vModelMat))) * vNormal;
}