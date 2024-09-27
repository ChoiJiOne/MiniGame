#version 460 core

layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec4 inColor;
layout(location = 2) flat in int inUnit;
layout(location = 3) in float inTransparent;

layout(location = 0) out vec4 outFragColor;

#define SPRITE_UNIT_0 0
#define SPRITE_UNIT_1 1
#define SPRITE_UNIT_2 2
#define SPRITE_UNIT_3 3
#define SPRITE_UNIT_4 4
#define SPRITE_UNIT_5 5
#define SPRITE_UNIT_6 6
#define SPRITE_UNIT_7 7
#define SPRITE_UNIT_8 8
#define SPRITE_UNIT_9 9

layout(binding = SPRITE_UNIT_0) uniform sampler2D sprite0;
layout(binding = SPRITE_UNIT_1) uniform sampler2D sprite1;
layout(binding = SPRITE_UNIT_2) uniform sampler2D sprite2;
layout(binding = SPRITE_UNIT_3) uniform sampler2D sprite3;
layout(binding = SPRITE_UNIT_4) uniform sampler2D sprite4;
layout(binding = SPRITE_UNIT_5) uniform sampler2D sprite5;
layout(binding = SPRITE_UNIT_6) uniform sampler2D sprite6;
layout(binding = SPRITE_UNIT_7) uniform sampler2D sprite7;
layout(binding = SPRITE_UNIT_8) uniform sampler2D sprite8;
layout(binding = SPRITE_UNIT_9) uniform sampler2D sprite9;

void main()
{
	vec3 fragColor = vec3(0.0f, 0.0f, 0.0f);
	float alpha = 0.0f;

	switch(inUnit)
	{
	case SPRITE_UNIT_0:
		fragColor = texture(sprite0, inTexCoords).rgb;
		alpha = texture(sprite0, inTexCoords).a;
		break;
	case SPRITE_UNIT_1:
		fragColor = texture(sprite1, inTexCoords).rgb;
		alpha = texture(sprite1, inTexCoords).a;
		break;

	case SPRITE_UNIT_2:
		fragColor = texture(sprite2, inTexCoords).rgb;
		alpha = texture(sprite2, inTexCoords).a;
		break;

	case SPRITE_UNIT_3:
		fragColor = texture(sprite3, inTexCoords).rgb;
		alpha = texture(sprite3, inTexCoords).a;
		break;

	case SPRITE_UNIT_4:
		fragColor = texture(sprite4, inTexCoords).rgb;
		alpha = texture(sprite4, inTexCoords).a;
		break;

	case SPRITE_UNIT_5:
		fragColor = texture(sprite5, inTexCoords).rgb;
		alpha = texture(sprite5, inTexCoords).a;
		break;

	case SPRITE_UNIT_6:
		fragColor = texture(sprite6, inTexCoords).rgb;
		alpha = texture(sprite6, inTexCoords).a;
		break;

	case SPRITE_UNIT_7:
		fragColor = texture(sprite7, inTexCoords).rgb;
		alpha = texture(sprite7, inTexCoords).a;
		break;

	case SPRITE_UNIT_8:
		fragColor = texture(sprite8, inTexCoords).rgb;
		alpha = texture(sprite8, inTexCoords).a;
		break;

	case SPRITE_UNIT_9:
		fragColor = texture(sprite9, inTexCoords).rgb;
		alpha = texture(sprite9, inTexCoords).a;
		break;
	}

	fragColor = mix(fragColor, inColor.rgb, inColor.a); /** Color Blending. */
	outFragColor = vec4(fragColor, alpha * inTransparent);
}