#version 460 core

layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec4 inColor;
layout(location = 2) flat in int inUnit;

layout(location = 0) out vec4 outFragColor;

#define GLYPH_UNIT_0 0
#define GLYPH_UNIT_1 1
#define GLYPH_UNIT_2 2
#define GLYPH_UNIT_3 3
#define GLYPH_UNIT_4 4
#define GLYPH_UNIT_5 5
#define GLYPH_UNIT_6 6
#define GLYPH_UNIT_7 7
#define GLYPH_UNIT_8 8
#define GLYPH_UNIT_9 9

layout(binding = GLYPH_UNIT_0) uniform sampler2D glyph0;
layout(binding = GLYPH_UNIT_1) uniform sampler2D glyph1;
layout(binding = GLYPH_UNIT_2) uniform sampler2D glyph2;
layout(binding = GLYPH_UNIT_3) uniform sampler2D glyph3;
layout(binding = GLYPH_UNIT_4) uniform sampler2D glyph4;
layout(binding = GLYPH_UNIT_5) uniform sampler2D glyph5;
layout(binding = GLYPH_UNIT_6) uniform sampler2D glyph6;
layout(binding = GLYPH_UNIT_7) uniform sampler2D glyph7;
layout(binding = GLYPH_UNIT_8) uniform sampler2D glyph8;
layout(binding = GLYPH_UNIT_9) uniform sampler2D glyph9;

void main()
{
	vec3 fragColor = inColor.rgb;
	float alpha = 0.0f;

	switch(inUnit)
	{
	case GLYPH_UNIT_0:
		alpha = inColor.a * texture(glyph0, inTexCoords).r;
		break;

	case GLYPH_UNIT_1:
		alpha = inColor.a * texture(glyph1, inTexCoords).r;
		break;

	case GLYPH_UNIT_2:
		alpha = inColor.a * texture(glyph2, inTexCoords).r;
		break;

	case GLYPH_UNIT_3:
		alpha = inColor.a * texture(glyph3, inTexCoords).r;
		break;

	case GLYPH_UNIT_4:
		alpha = inColor.a * texture(glyph4, inTexCoords).r;
		break;

	case GLYPH_UNIT_5:
		alpha = inColor.a * texture(glyph5, inTexCoords).r;
		break;

	case GLYPH_UNIT_6:
		alpha = inColor.a * texture(glyph6, inTexCoords).r;
		break;

	case GLYPH_UNIT_7:
		alpha = inColor.a * texture(glyph7, inTexCoords).r;
		break;

	case GLYPH_UNIT_8:
		alpha = inColor.a * texture(glyph8, inTexCoords).r;
		break;

	case GLYPH_UNIT_9:
		alpha = inColor.a * texture(glyph9, inTexCoords).r;
		break;
	}
	
	outFragColor = vec4(fragColor, alpha);
}