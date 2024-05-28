#version 460 core

layout(location = 0) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;

void main()
{
    vec3 fragColor = inColor.rgb;
    float alpha = inColor.a;

    outFragColor = vec4(fragColor, alpha);
}