#include <glad/glad.h>

#include "GLAssertion.h"
#include "RenderModule.h"
#include "StaticMesh.h"

StaticMesh::StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	: vertices_(vertices)
	, indices_(indices)
{
	uint32_t stride = static_cast<uint32_t>(sizeof(Vertex));
	uint32_t byteSize = static_cast<uint32_t>(vertices_.size()) * stride;

	vertexBuffer_ = RenderModule::CreateResource<VertexBuffer>(vertices_.data(), byteSize, VertexBuffer::EUsage::STATIC);
	indexBuffer_ = RenderModule::CreateResource<IndexBuffer>(indices_.data(), static_cast<uint32_t>(indices_.size()));

	GL_FAILED(glGenVertexArrays(1, &vertexArrayObject_));
	GL_FAILED(glBindVertexArray(vertexArrayObject_));
	{
		vertexBuffer_->Bind();
		indexBuffer_->Bind();

		GL_FAILED(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, position))));
		GL_FAILED(glEnableVertexAttribArray(0));

		GL_FAILED(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, normal))));
		GL_FAILED(glEnableVertexAttribArray(1));

		GL_FAILED(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, tangent))));
		GL_FAILED(glEnableVertexAttribArray(2));

		GL_FAILED(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, texcoord))));
		GL_FAILED(glEnableVertexAttribArray(3));

		vertexBuffer_->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));

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
		RenderModule::DestroyResource(indexBuffer_);
		indexBuffer_ = nullptr;
	}

	if (vertexBuffer_)
	{
		RenderModule::DestroyResource(vertexBuffer_);
		vertexBuffer_ = nullptr;
	}

	GL_FAILED(glDeleteVertexArrays(1, &vertexArrayObject_));

	bIsInitialized_ = false;
}

void StaticMesh::Bind()
{
	GL_FAILED(glBindVertexArray(vertexArrayObject_));
}

void StaticMesh::Unbind()
{
	GL_FAILED(glBindVertexArray(0));
}