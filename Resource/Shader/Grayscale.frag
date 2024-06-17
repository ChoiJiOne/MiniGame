#version 460 core

layout(location = 0) in vec2 inTexCoords;

layout(location = 0) out vec4 outFragColor;

layout(binding = 0) uniform sampler2D framebuffer;

void main()
{
    vec3 colorRGB = texture(framebuffer, inTexCoords).rgb;
    float weight = 0.2126f * colorRGB.r + 0.7152f * colorRGB.g + 0.0722f * colorRGB.b;
    
    outFragColor = vec4(vec3(weight, weight, weight), 1.0);
}