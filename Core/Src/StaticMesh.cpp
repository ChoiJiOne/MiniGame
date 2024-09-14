#include <glad/glad.h>

#include "Assertion.h"
#include "IndexBuffer.h"
#include "ResourceManager.h"
#include "StaticMesh.h"
#include "VertexBuffer.h"

StaticMesh::StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	: vertices_(vertices)
	, indices_(indices)
{
	uint32_t stride = static_cast<uint32_t>(sizeof(Vertex));
	uint32_t byteSize = static_cast<uint32_t>(vertices_.size()) * stride;
	vertexBuffer_ = ResourceManager::Get().Create<VertexBuffer>(vertices_.data(), byteSize, VertexBuffer::Usage::STATIC);
	indexBuffer_ = ResourceManager::Get().Create<IndexBuffer>(indices_.data(), static_cast<uint32_t>(indices_.size()));

	GL_CHECK(glGenVertexArrays(1, &vertexArrayObject_));
	GL_CHECK(glBindVertexArray(vertexArrayObject_));
	{
		vertexBuffer_->Bind();
		indexBuffer_->Bind();

		GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, position))));
		GL_CHECK(glEnableVertexAttribArray(0));

		GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, normal))));
		GL_CHECK(glEnableVertexAttribArray(1));

		GL_CHECK(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, tangent))));
		GL_CHECK(glEnableVertexAttribArray(2));

		GL_CHECK(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, uv))));
		GL_CHECK(glEnableVertexAttribArray(3));

		vertexBuffer_->Unbind();
	}
	GL_CHECK(glBindVertexArray(0));

	bIsInitialized_ = true;
}

StaticMesh::~StaticMesh()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void StaticMesh::Release()
{
	CHECK(bIsInitialized_);

	if (indexBuffer_)
	{
		ResourceManager::Get().Destroy(indexBuffer_);
		indexBuffer_ = nullptr;
	}

	if (vertexBuffer_)
	{
		ResourceManager::Get().Destroy(vertexBuffer_);
		vertexBuffer_ = nullptr;
	}

	GL_CHECK(glDeleteVertexArrays(1, &vertexArrayObject_));

	bIsInitialized_ = false;
}

void StaticMesh::Bind() const
{
	GL_CHECK(glBindVertexArray(vertexArrayObject_));
}

void StaticMesh::Unbind() const
{
	GL_CHECK(glBindVertexArray(0));
}

/** https://github.com/microsoft/DirectXTK/blob/main/Src/Geometry.cpp#L71 */
StaticMesh* StaticMesh::CreateBox(const GameMath::Vec3f& size)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	static const int32_t faceCount = 6;
	static const std::array<GameMath::Vec3f, 6> normals =
	{
		GameMath::Vec3f( 0.0f,  0.0f, +1.0f),
		GameMath::Vec3f( 0.0f,  0.0f, -1.0f),
		GameMath::Vec3f(+1.0f,  0.0f,  0.0f),
		GameMath::Vec3f(-1.0f,  0.0f,  0.0f),
		GameMath::Vec3f( 0.0f, +1.0f,  0.0f),
		GameMath::Vec3f( 0.0f, -1.0f,  0.0f),
	};
	static const std::array<GameMath::Vec2f, 4> uvs =
	{
		GameMath::Vec2f(0.0f, 1.0f),
		GameMath::Vec2f(1.0f, 1.0f),
		GameMath::Vec2f(1.0f, 0.0f),
		GameMath::Vec2f(0.0f, 0.0f),
	};

	GameMath::Vec3f size2 = size * 0.5f;
	for (int32_t index = 0; index < faceCount; ++index)
	{
		const GameMath::Vec3f normal = normals[index];
		const GameMath::Vec3f basis = (index >= 4) ? GameMath::Vec3f(0.0f, 0.0f, 1.0f) : GameMath::Vec3f(0.0f, 1.0f, 0.0f);
		const GameMath::Vec3f side1 = GameMath::Vec3f::Cross(normal, basis);
		const GameMath::Vec3f side2 = GameMath::Vec3f::Cross(normal, side1);
		
		const std::size_t vertexBase = vertices.size();
		indices.push_back(vertexBase + 0);
		indices.push_back(vertexBase + 1);
		indices.push_back(vertexBase + 2);

		indices.push_back(vertexBase + 0);
		indices.push_back(vertexBase + 2);
		indices.push_back(vertexBase + 3);

		vertices.push_back(Vertex{ (normal + side1 + side2) * size2, normal, GameMath::Vec3f(0.0f, 0.0f, 0.0f), uvs[0] });
		vertices.push_back(Vertex{ (normal - side1 + side2) * size2, normal, GameMath::Vec3f(0.0f, 0.0f, 0.0f), uvs[1] });
		vertices.push_back(Vertex{ (normal - side1 - side2) * size2, normal, GameMath::Vec3f(0.0f, 0.0f, 0.0f), uvs[2] });
		vertices.push_back(Vertex{ (normal + side1 - side2) * size2, normal, GameMath::Vec3f(0.0f, 0.0f, 0.0f), uvs[3] });
	}

	return ResourceManager::Get().Create<StaticMesh>(vertices, indices);
}
