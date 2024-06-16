#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 4) in vec4 inWeights;
layout(location = 5) in ivec4 inJoints;

const int MAX_SIZE = 120;

uniform mat4 world;
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

	gl_Position = lightSpaceMatrix * world * skin * vec4(inPosition, 1.0f);
}