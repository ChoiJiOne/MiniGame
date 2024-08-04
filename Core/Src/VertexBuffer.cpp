#include <glad/glad.h>

#include "Assertion.h"
#include "VertexBuffer.h"

using namespace GameMaker;

VertexBuffer::VertexBuffer(uint32_t byteSize, const EUsage& usage)
	: byteSize_(byteSize)
	, usage_(usage)
{
	GL_FAILED(glGenBuffers(1, &vertexBufferID_));
	GL_FAILED(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID_));
	GL_FAILED(glBufferData(GL_ARRAY_BUFFER, byteSize_, nullptr, static_cast<GLenum>(usage)));
	GL_FAILED(glBindBuffer(GL_ARRAY_BUFFER, 0));

	bIsInitialized_ = true;
}

VertexBuffer::VertexBuffer(const void* bufferPtr, uint32_t byteSize, const EUsage& usage)
	: byteSize_(byteSize)
	, usage_(usage)
{
	GL_FAILED(glGenBuffers(1, &vertexBufferID_));
	GL_FAILED(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID_));
	GL_FAILED(glBufferData(GL_ARRAY_BUFFER, byteSize_, bufferPtr, static_cast<GLenum>(usage)));
	GL_FAILED(glBindBuffer(GL_ARRAY_BUFFER, 0));

	bIsInitialized_ = true;
}

VertexBuffer::~VertexBuffer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void VertexBuffer::Release()
{
	CHECK(bIsInitialized_);

	GL_FAILED(glDeleteBuffers(1, &vertexBufferID_));

	bIsInitialized_ = false;
}

void VertexBuffer::Bind()
{
	GL_FAILED(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID_));
}

void VertexBuffer::Unbind()
{
	GL_FAILED(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::SetBufferData(const void* bufferPtr, uint32_t bufferSize)
{
	CHECK(bufferPtr != nullptr && bufferSize <= byteSize_);

	VertexBuffer::Bind();
	{
		void* vertexBufferPtr = nullptr;

		switch (usage_)
		{
		case EUsage::STREAM:
			GL_FAILED(glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferPtr, static_cast<GLenum>(usage_)));
			break;

		case EUsage::STATIC:
			GL_FAILED(glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, bufferPtr));
			break;

		case EUsage::DYNAMIC:
			vertexBufferPtr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			std::memcpy(vertexBufferPtr, bufferPtr, bufferSize);
			GL_FAILED(glUnmapBuffer(GL_ARRAY_BUFFER));
			break;

		default:
			ASSERT(false, "undefined buffer usage type");
			break;
		}
	}
	VertexBuffer::Unbind();
}