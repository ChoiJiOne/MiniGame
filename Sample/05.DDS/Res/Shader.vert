#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out vec2 outTexCoords;

void main()
{
    gl_Position = vec4(inPosition, 1.0f);
    outTexCoords = vec2(inTexCoords.x, 1.0f - inTexCoords.y);
}