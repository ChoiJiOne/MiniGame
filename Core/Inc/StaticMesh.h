#pragma once

#include <vector>

#include "GameMath.h"

#include "IMesh.h"

class IndexBuffer;
class VertexBuffer;

class StaticMesh : public IMesh
{
public:
	struct Vertex
	{
		static uint32_t GetStride()
		{
			return sizeof(Vertex);
		}

		Vec3f position;
		Vec3f normal;
		Vec3f tangent;
		Vec2f uv;
	};

public:
	StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
	virtual ~StaticMesh();

	DISALLOW_COPY_AND_ASSIGN(StaticMesh);

	virtual void Release() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual uint32_t GetIndexCount() const override { return static_cast<uint32_t>(indices_.size()); }
	virtual uint32_t GetVertexCount() const override { return static_cast<uint32_t>(vertices_.size()); }

	static StaticMesh* CreateBox(const Vec3f& size);
	static StaticMesh* CreateSphere(float radius, uint32_t tessellation);

private:
	std::vector<Vertex> vertices_;
	VertexBuffer* vertexBuffer_ = nullptr;

	std::vector<uint32_t> indices_;
	IndexBuffer* indexBuffer_ = nullptr;

	uint32_t vertexArrayObject_ = 0;
};