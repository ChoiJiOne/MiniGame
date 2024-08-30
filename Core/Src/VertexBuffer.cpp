#include <glad/glad.h>

#include "Assertion.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(uint32_t byteSize, const Usage& usage)
	: byteSize_(byteSize)
	, usage_(usage)
{
	GL_CHECK(glGenBuffers(1, &vertexBufferID_));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID_));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, byteSize_, nullptr, static_cast<GLenum>(usage)));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

	bIsInitialized_ = true;
}

VertexBuffer::VertexBuffer(const void* bufferPtr, uint32_t byteSize, const Usage& usage)
	: byteSize_(byteSize)
	, usage_(usage)
{
	GL_CHECK(glGenBuffers(1, &vertexBufferID_));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID_));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, byteSize_, bufferPtr, static_cast<GLenum>(usage)));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

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

	GL_CHECK(glDeleteBuffers(1, &vertexBufferID_));

	bIsInitialized_ = false;
}

void VertexBuffer::Bind()
{
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID_));
}

void VertexBuffer::Unbind()
{
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::SetBufferData(const void* bufferPtr, uint32_t bufferSize)
{
	CHECK(bufferPtr != nullptr && bufferSize <= byteSize_);

	VertexBuffer::Bind();
	{
		void* vertexBufferPtr = nullptr;

		switch (usage_)
		{
		case Usage::STREAM:
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferPtr, static_cast<GLenum>(usage_)));
			break;

		case Usage::STATIC:
			GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, bufferPtr));
			break;

		case Usage::DYNAMIC:
			vertexBufferPtr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			std::memcpy(vertexBufferPtr, bufferPtr, bufferSize);
			GL_CHECK(glUnmapBuffer(GL_ARRAY_BUFFER));
			break;

		default:
			ASSERT(false, "undefined buffer usage type");
		}
	}
	VertexBuffer::Unbind();
}