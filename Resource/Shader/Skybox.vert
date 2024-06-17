#version 460 core

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 outTexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	outTexCoords = inPosition;
	mat4 view3x3 = mat4(mat3(view));
	
	vec4 position = projection * view3x3 * vec4(inPosition, 1.0f);
	gl_Position = position.xyww;
}