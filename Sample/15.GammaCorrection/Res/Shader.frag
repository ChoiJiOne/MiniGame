#version 460 core

layout(location = 0) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;

uniform bool bEnableGammaCorrection;
uniform float gamma;

void main()
{
    vec3 fragColor = inColor.rgb;
    float alpha = inColor.a;

    if(bEnableGammaCorrection)
    {
        fragColor = pow(fragColor, vec3(1.0f / gamma));
    }

    outFragColor = vec4(fragColor, alpha);
}