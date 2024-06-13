#version 460 core

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec3 inTangent;
layout(location = 2) in vec2 inTexcoord;

layout(location = 0) out vec4 outFragColor;

layout(binding = 0) uniform sampler2D material;

void main()
{
	vec3 colorRGB = texture(material, inTexcoord).rgb;
	float alpha = texture(material, inTexcoord).a;

	outFragColor = vec4(colorRGB, alpha);
}