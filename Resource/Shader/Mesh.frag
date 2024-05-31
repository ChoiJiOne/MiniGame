#version 460 core

layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec3 inWorldPosition;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec4 inWorldPositionInLightSpace;

layout(location = 0) out vec4 outFragColor;

//layout(binding = 0) uniform sampler2D shadowMap;
layout(binding = 1) uniform sampler2D material;

struct Light
{
    vec3 direction;
    vec3 color;
};

uniform vec3 viewPosition;
uniform Light light;

//float ComputeShadow(vec4 worldPositionInLightSpace)
//{
//    vec3 projectionCoords = worldPositionInLightSpace.xyz / worldPositionInLightSpace.w;
//	projectionCoords = projectionCoords * 0.5f + 0.5f;
//
//	float closestDepth = texture(shadowMap, projectionCoords.xy).r;
//	float currentDepth = projectionCoords.z;
//
//	float bias = 0.001f;
//	float shadow = 0.0f;
//	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
//	for(int x = -2; x <= 2; ++x)
//	{
//		for(int y = -2; y <= 2; ++y)
//		{
//			float pcf = texture(shadowMap, projectionCoords.xy + vec2(x, y) * texelSize).r;
//			shadow += currentDepth - bias > pcf  ? 1.0f : 0.0f;
//		}
//	}
//	shadow /= 25.0f;
//	
//	return shadow;
//}

void main()
{
	vec3 colorRGB = texture(material, inTexCoords).rgb;
	float alpha = texture(material, inTexCoords).a;

	// ambient
	vec3 ambientRGB = 0.5f * light.color;
	
	// diffuise
	vec3 direction = normalize(-light.direction);
	float diff = max(dot(direction, inNormal), 0.0f);
	vec3 diffuseRGB = diff * light.color;

	// specular
	vec3 viewDirection = normalize(viewPosition - inWorldPosition);
	vec3 halfDirection = normalize(direction + viewDirection);
	float spec = pow(max(dot(inNormal, halfDirection), 0.0f), 64.0f);
	vec3 specularRGB = spec * light.color;

//	float shadow = ComputeShadow(inWorldPositionInLightSpace);
	vec3 finalColor = (ambientRGB + (diffuseRGB + specularRGB)) * colorRGB;

	outFragColor = vec4(finalColor, alpha);
}