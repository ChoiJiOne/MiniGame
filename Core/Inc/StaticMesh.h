#pragma once

#include <vector>

#include "GameMath.h"

#include "IMesh.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class StaticMesh : public IMesh
{
public:
	struct Vertex
	{
		static uint32_t GetStride()
		{
			return sizeof(Vertex);
		}

		GameMath::Vec3f position;
		GameMath::Vec3f normal;
		GameMath::Vec3f tangent;
		GameMath::Vec2f uv;
	};
};