#version 460 core

layout(location = 0) in vec3 inPosition;

uniform mat4 world;

void main()
{
	gl_Position = world * vec4(inPosition, 1.0f);
}