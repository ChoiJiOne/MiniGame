#version 460 core

layout(location = 0) in vec3 inPosition;

uniform mat4 world;
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = lightSpaceMatrix * world * vec4(inPosition, 1.0f);
}