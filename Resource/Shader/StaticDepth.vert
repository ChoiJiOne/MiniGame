#version 460 core

layout(location = 0) in vec3 inPosition;

uniform mat4 world;
uniform mat4 lightView;
uniform mat4 lightProjection;

void main()
{
	gl_Position = lightProjection * lightView * world * vec4(inPosition, 1.0f);
}