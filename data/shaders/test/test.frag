#version 330 core

layout (location = 0) out vec4 fColor;
in vec2 texCoords;


uniform sampler2D tex;

void main()
{
	fColor = vec4(texture(tex, texCoords).rgb, 1.0);
}