#version 460 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec2 outTexCoords;
layout(location = 1) out vec4 outColor;

uniform mat4 transform;
uniform mat4 ortho;
uniform float pointSize;

void main()
{
	gl_Position = ortho * transform * vec4(vec3(inPosition, 0.0f), 1.0f);
	gl_PointSize = pointSize;

	outTexCoords = inTexCoords;
	outColor = inColor;
}