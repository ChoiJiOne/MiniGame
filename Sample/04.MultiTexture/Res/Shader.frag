#version 460 core

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out vec4 outFragColor;

layout(binding = 0) uniform sampler2D texture0;
layout(binding = 1) uniform sampler2D texture1;

void main()
{
	outFragColor = mix(texture(texture0, inTexCoords), texture(texture1, inTexCoords), 0.2f);
}