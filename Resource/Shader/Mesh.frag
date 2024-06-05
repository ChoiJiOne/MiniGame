#version 460 core

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec3 inTangent;
layout(location = 2) in vec2 inTexcoord;

layout(location = 0) out vec4 outFragColor;

void main()
{
	outFragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}