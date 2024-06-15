#version 460 core

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec4 inWorldPositionLightSpace;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec2 inTexcoord;

layout(location = 0) out vec4 outFragColor;

layout(binding = 0) uniform sampler2D shadowMap;
layout(binding = 1) uniform sampler2D material;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

float ShadowFactor(vec4 worldPositionLightSpace)
{
	vec3 shadowCoords = worldPositionLightSpace.xyz / worldPositionLightSpace.w;
	shadowCoords = shadowCoords * 0.5f + 0.5f;

	float bias = 0.0001f;
	float closestDepth = texture(shadowMap, shadowCoords.xy).r;
	float currentDepth = shadowCoords.z;
	float shadow = 0.0f;
	vec2 size = 1.0 / textureSize(shadowMap, 0);

	int sampleBound = 7;
	float sampleCount = (2.0f * sampleBound + 1.0f) * (2.0f * sampleBound + 1.0f);
	for(int x = -sampleBound; x <= sampleBound; ++x)
	{
		for(int y = -sampleBound; y <= sampleBound; ++y)
		{			
			float pcf = texture(shadowMap, shadowCoords.xy + vec2(x, y) * size).r;
			shadow += currentDepth - bias > pcf ? 1.0f : 0.0f;
		}
	}

	shadow /= sampleCount;

	return shadow;
}

void main()
{
	vec3 colorRGB = texture(material, inTexcoord).rgb;
	vec3 normal = normalize(inNormal);
	vec3 lightColor = vec3(1.0f);

	// ambient
	vec3 ambient = 0.15f * lightColor;

	// diffuse
	vec3 lightDirection = lightPosition - inWorldPosition;
	float diff = max(dot(lightDirection, normal), 0.0f);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDirection = normalize(cameraPosition - inWorldPosition);
	float spec = 0.0f;
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);  
    spec = pow(max(dot(normal, halfwayDirection), 0.0f), 64.0f);
    vec3 specular = spec * lightColor;    

	// shadow
	float factor = ShadowFactor(inWorldPositionLightSpace);
	vec3 lighting = (ambient + (1.0f - factor) * (diffuse + specular)) * colorRGB;

	outFragColor = vec4(lighting, 1.0f);
}