#version 460 core

layout(location = 0) in vec2 inTexCoords;

layout(location = 0) out vec4 outFragColor;

layout(binding = 0) uniform sampler2D framebuffer;

uniform float fadeBias;

void main()
{
    vec3 colorRGB = texture(framebuffer, inTexCoords).rgb;
    outFragColor = vec4(colorRGB * fadeBias, 1.0f);
}