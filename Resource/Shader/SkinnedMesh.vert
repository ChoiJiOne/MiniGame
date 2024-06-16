#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexcoord;
layout(location = 4) in vec4 inWeights;
layout(location = 5) in ivec4 inJoints;

layout(location = 0) out vec3 outWorldPosition;
layout(location = 1) out vec4 outWorldPositionLightSpace;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outTangent;
layout(location = 4) out vec2 outTexcoord;

const int MAX_SIZE = 120;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform mat4 bindPose[MAX_SIZE];
uniform mat4 invBindPose[MAX_SIZE];

void main()
{
    mat4 m0 = (bindPose[inJoints.x] * invBindPose[inJoints.x]) * inWeights.x;
    mat4 m1 = (bindPose[inJoints.y] * invBindPose[inJoints.y]) * inWeights.y;
    mat4 m2 = (bindPose[inJoints.z] * invBindPose[inJoints.z]) * inWeights.z;
    mat4 m3 = (bindPose[inJoints.w] * invBindPose[inJoints.w]) * inWeights.w;

    mat4 skin = m0 + m1 + m2 + m3;

	gl_Position = projection * view * world * skin * vec4(inPosition, 1.0f);

    outWorldPosition = vec3(world * vec4(inPosition, 1.0f));
	outWorldPositionLightSpace = lightSpaceMatrix * vec4(outWorldPosition, 1.0f);
	outNormal = transpose(inverse(mat3(world))) * inNormal;
	outTangent = transpose(inverse(mat3(world))) * inTangent;
	outTexcoord = inTexcoord;
}