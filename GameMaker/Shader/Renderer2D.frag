#version 460 core

layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;

#define GEOMETRY_MODE 0
#define STRING_MODE 1
#define SPRITE_MODE 2

uniform int mode;
uniform float factor;

layout(binding = 0) uniform sampler2D glyphAtlasMap;
layout(binding = 1) uniform sampler2D spriteMap;

void main()
{
	if (mode == STRING_MODE)
	{		
		vec3 fragColor = inColor.rgb;
		float alpha = inColor.a * texture(glyphAtlasMap, inTexCoords).r;
		outFragColor = vec4(fragColor, alpha);
	}
	else if (mode == SPRITE_MODE)
	{
		vec3 fragColor = texture(spriteMap, inTexCoords).rgb;
		float alpha = texture(spriteMap, inTexCoords).a;
		fragColor = mix(fragColor, inColor.rgb, factor);

		outFragColor = vec4(fragColor, alpha);
	}
	else // mode == GEOMETRY_MODE
	{
		outFragColor = inColor;
	}
}