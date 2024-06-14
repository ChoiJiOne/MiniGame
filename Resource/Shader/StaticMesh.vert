#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexcoord;

layout(location = 0) out vec3 outWorldPosition;
layout(location = 1) out vec4 outWorldPositionLightSpace;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outTangent;
layout(location = 4) out vec2 outTexcoord;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = projection * view * world * vec4(inPosition, 1.0f);

	outWorldPosition = vec3(world * vec4(inPosition, 1.0f));
	outWorldPositionLightSpace = lightSpaceMatrix * vec4(outWorldPosition, 1.0f);
	outNormal = transpose(inverse(mat3(world))) * inNormal;
	outTangent = transpose(inverse(mat3(world))) * inTangent;
	outTexcoord = inTexcoord;
}