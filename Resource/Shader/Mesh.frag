#version 460 core

layout(location = 0) in vec2 inTexCoords;

layout(location = 0) out vec4 outFragColor;

layout(binding = 0) uniform sampler2D material;

void main()
{
    outFragColor = texture(material, inTexCoords);
}