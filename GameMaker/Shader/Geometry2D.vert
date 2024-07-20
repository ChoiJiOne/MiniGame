#version 460 core

layout(location = 0) in vec2 inPosition;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec4 outColor;

uniform mat4 ortho;

void main()
{
	gl_Position = ortho * vec4(vec3(inPosition, 0.0f), 1.0f);
	outColor = inColor;
}