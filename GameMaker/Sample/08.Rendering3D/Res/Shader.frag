#version 460 core

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out vec4 outFragColor;

layout(binding = 0) uniform sampler2D albedoMap;

void main()
{
	outFragColor.rgb = mix(texture(albedoMap, inTexCoords).rgb, inColor.rgb, 0.5f);
	outFragColor.a = texture(albedoMap, inTexCoords).a;
}