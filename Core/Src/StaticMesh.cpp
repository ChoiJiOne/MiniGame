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