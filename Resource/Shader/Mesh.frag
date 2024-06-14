#version 460 core

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexcoord;

layout(location = 0) out vec4 outFragColor;

layout(binding = 0) uniform sampler2DArray cascadeShadowMap;
layout(binding = 1) uniform sampler2D material;

const int MAX_MATRICES = 16;

uniform vec3 lightDirection;
uniform vec3 cameraPosition;
uniform float farPlane;
uniform mat4 view;
uniform mat4 lightSpaceMatrices[MAX_MATRICES];
uniform float cascadePlaneDistances[MAX_MATRICES];
uniform int cascadeCount;

float GetShadowFactor(vec3 worldPosition)
{
	vec4 viewPosition = view * vec4(worldPosition, 1.0f);
	float depth = abs(viewPosition.z);

	int layer = -1;
	for(int index = 0; index < cascadeCount; ++index)
	{
		if(depth > cascadePlaneDistances[index])
		{
			layer = index;
			break;
		}
	}

	if(layer == -1)
	{
		layer = cascadeCount;
	}

	vec4 worldPositionLightSpace = lightSpaceMatrices[layer] * vec4(worldPosition, 1.0f);
	vec3 projectCoords = worldPositionLightSpace.xyz / worldPositionLightSpace.w;
	projectCoords = projectCoords * 0.5f + 0.5f;

	float currentDepth = projectCoords.z;

	if(currentDepth > 1.0f)
	{
		return 0.0f;
	}

	vec3 normal = normalize(inNormal);
	float bias = max(0.05f * (1.0f - dot(normal, lightDirection)), 0.005f);
	const float biasModifier = 0.5f;
	
	if(layer == cascadeCount)
	{
		bias *= 1.0f / (farPlane * biasModifier);
	}
	else
	{
		bias *= 1.0f / (cascadePlaneDistances[layer] * biasModifier);
	}

	float factor = 0.0f;
	vec2 texelSize = 1.0f / vec2(textureSize(cascadeShadowMap, 0));
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcf = texture(cascadeShadowMap, vec3(projectCoords.xy + vec2(x, y) * texelSize, layer)).r;
			factor += (currentDepth - bias) > pcf ? 1.0f : 0.0f;
		}
	}
	factor /= 9.0f;

	return factor;
}

void main()
{
	vec3 colorRGB = texture(material, inTexcoord).rgb;
	vec3 normal = normalize(inNormal);
	vec3 lightColor = vec3(0.3f);

	// ambient
	vec3 ambient = 0.4f * lightColor;

	// diffuse
	float diff = max(dot(lightDirection, normal), 0.0f);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDirection = normalize(cameraPosition - inWorldPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = 0.0f;
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);  
    spec = pow(max(dot(normal, halfwayDirection), 0.0f), 64.0f);
    vec3 specular = spec * lightColor;    

	// shadow
	float factor = GetShadowFactor(inWorldPosition);
	vec3 lighting = (ambient + (1.0f - factor) * (diffuse + specular)) * colorRGB;

	outFragColor = vec4(lighting, 1.0f);
}