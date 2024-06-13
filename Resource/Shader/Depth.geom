#version 460 core

layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;

const int MAX_MATRICES = 16;
const int MAX_VERTICES = 3;

uniform mat4 lightSpaceMatrices[MAX_MATRICES];

void main()
{
	for(int index = 0; index < MAX_VERTICES; ++index)
	{
		gl_Position = lightSpaceMatrices[gl_InvocationID] * gl_in[index].gl_Position;
		gl_Layer = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
}