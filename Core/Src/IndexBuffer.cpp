#include <glad/glad.h>

#include "Assertion.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* bufferPtr, uint32_t indexCount)
	: indexCount_(indexCount)
{
	uint32_t indexBufferSize = indexCount_ * sizeof(uint32_t);

	GL_CHECK(glGenBuffers(1, &indexBufferID_));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID_));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, bufferPtr, GL_STATIC_DRAW));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	bIsInitialized_ = true;
}

IndexBuffer::~IndexBuffer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void IndexBuffer::Release()
{
	CHECK(bIsInitialized_);

	GL_CHECK(glDeleteBuffers(1, &indexBufferID_));

	bIsInitialized_ = false;
}

void IndexBuffer::Bind()
{
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID_));
}

void IndexBuffer::Unbind()
{
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}