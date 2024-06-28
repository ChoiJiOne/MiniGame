#version 460 core

layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;

#define GEOMETRY_MODE 0
#define STRING_MODE 1

uniform int mode;

layout(binding = 0) uniform sampler2D glyphAtlasMap;

void main()
{
	if (mode == GEOMETRY_MODE)
	{
		outFragColor = inColor;
	}
	else if(mode == STRING_MODE)
	{
		vec3 fragColor = inColor.rgb;
		float alpha = inColor.a * texture(glyphAtlasMap, inTexCoords).r;
		outFragColor = vec4(fragColor, alpha);
	}
}