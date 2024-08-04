#pragma once

#include <vector>

#include "GameMath.h"

#include "IMesh.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"


namespace GameMaker
{
class SkinnedMesh : public IMesh
{
public:
	struct Vertex
	{
		Vec3f position;
		Vec3f normal;
		Vec3f tangent;
		Vec2f texcoord;
		Vec4f weight;
		Vec4i joints; 
	};

public:
	explicit SkinnedMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
	virtual ~SkinnedMesh();

	DISALLOW_COPY_AND_ASSIGN(SkinnedMesh);

	virtual void Release() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual uint32_t GetIndexCount() const override { return static_cast<uint32_t>(indices_.size()); }
	virtual uint32_t GetVertexCount() const override { return static_cast<uint32_t>(vertices_.size()); }

private:
	std::vector<Vertex> vertices_;
	VertexBuffer* vertexBuffer_ = nullptr;
	std::vector<uint32_t> indices_;
	IndexBuffer* indexBuffer_ = nullptr;
	uint32_t vertexArrayObject_ = 0;
};

}