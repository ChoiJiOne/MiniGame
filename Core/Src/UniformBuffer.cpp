#include <glad/glad.h>

#include "Assertion.h"
#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(uint32_t byteSize, const Usage& usage)
	: byteSize_(byteSize)
	, usage_(usage)
{
	GL_CHECK(glGenBuffers(1, &uniformBufferID_));
	GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferID_));
	GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, byteSize_, nullptr, static_cast<GLenum>(usage)));
	GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	bIsInitialized_ = true;
}

UniformBuffer::UniformBuffer(const void* bufferPtr, uint32_t byteSize, const Usage& usage)
	: byteSize_(byteSize)
	, usage_(usage)
{
	GL_CHECK(glGenBuffers(1, &uniformBufferID_));
	GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferID_));
	GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, byteSize_, bufferPtr, static_cast<GLenum>(usage)));
	GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	bIsInitialized_ = true;
}

UniformBuffer::~UniformBuffer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void UniformBuffer::Release()
{
	CHECK(bIsInitialized_);

	GL_CHECK(glDeleteBuffers(1, &uniformBufferID_));

	bIsInitialized_ = false;
}

void UniformBuffer::Bind()
{
	GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferID_));
}

void UniformBuffer::Bind(const uint32_t slot)
{
	GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, slot, uniformBufferID_));
}

void UniformBuffer::Unbind()
{
	GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::SetBufferData(const void* bufferPtr, uint32_t bufferSize)
{
	CHECK(bufferPtr != nullptr && bufferSize <= byteSize_);

	UniformBuffer::Bind();
	{
		void* uniformBufferPtr = nullptr;

		switch (usage_)
		{
		case Usage::STREAM:
			GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, bufferSize, bufferPtr, static_cast<GLenum>(usage_)));
			break;

		case Usage::STATIC:
			GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, 0, bufferSize, bufferPtr));
			break;

		case Usage::DYNAMIC:
			uniformBufferPtr = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
			std::memcpy(uniformBufferPtr, bufferPtr, bufferSize);
			GL_CHECK(glUnmapBuffer(GL_UNIFORM_BUFFER));
			break;

		default:
			ASSERT(false, "undefined buffer usage type");
		}
	}
	UniformBuffer::Unbind();
}