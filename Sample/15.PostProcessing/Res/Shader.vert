#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 3) in vec2 inTexCoords;

layout(location = 0) out vec2 outTexCoords;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * world * vec4(inPosition, 1.0f);
	outTexCoords = inTexCoords;
}