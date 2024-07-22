#version 460 core

layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;

layout(binding = 0) uniform sampler2D sprite;

void main()
{
	vec3 fragColor = texture(sprite, inTexCoords).rgb;
	float alpha = texture(sprite, inTexCoords).a;

	fragColor = mix(fragColor, inColor.rgb, inColor.a);
	outFragColor = vec4(fragColor, alpha);
}