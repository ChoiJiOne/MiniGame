#version 460 core

layout(location = 0) in vec2 inPosition;

layout(location = 0) out vec2 outTexCoords;

void main()
{
    gl_Position = vec4(inPosition, 0.0f, 1.0f);
    outTexCoords = (inPosition + vec2(1.0f)) * 0.5f;
}