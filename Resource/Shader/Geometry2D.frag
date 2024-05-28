#version 460 core

layout(location = 0) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;
layout(location = 1) out vec4 outBrightColor;

void main()
{
    outFragColor = inColor;

    vec3 fragColor = inColor.rgb;
    float alpha = inColor.a;

    float brightness = dot(fragColor, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f)
    {
        outBrightColor = vec4(fragColor, alpha);
    }
    else
    {
        outBrightColor = vec4(0.0f, 0.0f, 0.0f, alpha);
    }
}