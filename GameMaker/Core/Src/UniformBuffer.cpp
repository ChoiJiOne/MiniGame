#include <glad/glad.h>

#include "Assertion.h"
#include "UniformBuffer.h"

using namespace GameMaker;

UniformBuffer::UniformBuffer(uint32_t byteSize, const EUsage& usage)
	: byteSize_(byteSize)
	, usage_(usage)
{
	GL_FAILED(glGenBuffers(1, &uniformBufferID_));
	GL_FAILED(glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferID_));
	GL_FAILED(glBufferData(GL_UNIFORM_BUFFER, byteSize_, nullptr, static_cast<GLenum>(usage)));
	GL_FAILED(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	bIsInitialized_ = true;
}

UniformBuffer::UniformBuffer(const void* bufferPtr, uint32_t byteSize, const EUsage& usage)
	: byteSize_(byteSize)
	, usage_(usage)
{
	GL_FAILED(glGenBuffers(1, &uniformBufferID_));
	GL_FAILED(glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferID_));
	GL_FAILED(glBufferData(GL_UNIFORM_BUFFER, byteSize_, bufferPtr, static_cast<GLenum>(usage)));
	GL_FAILED(glBindBuffer(GL_UNIFORM_BUFFER, 0));

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

	GL_FAILED(glDeleteBuffers(1, &uniformBufferID_));

	bIsInitialized_ = false;
}

void UniformBuffer::Bind()
{
	GL_FAILED(glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferID_));
}

void UniformBuffer::Bind(const uint32_t slot)
{
	GL_FAILED(glBindBufferBase(GL_UNIFORM_BUFFER, slot, uniformBufferID_));
}

void UniformBuffer::Unbind()
{
	GL_FAILED(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::SetBufferData(const void* bufferPtr, uint32_t bufferSize)
{
	CHECK(bufferPtr != nullptr && bufferSize <= byteSize_);

	UniformBuffer::Bind();
	{
		void* uniformBufferPtr = nullptr;

		switch (usage_)
		{
		case EUsage::STREAM:
			GL_FAILED(glBufferData(GL_UNIFORM_BUFFER, bufferSize, bufferPtr, static_cast<GLenum>(usage_)));
			break;

		case EUsage::STATIC:
			GL_FAILED(glBufferSubData(GL_UNIFORM_BUFFER, 0, bufferSize, bufferPtr));
			break;

		case EUsage::DYNAMIC:
			uniformBufferPtr = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
			std::memcpy(uniformBufferPtr, bufferPtr, bufferSize);
			GL_FAILED(glUnmapBuffer(GL_UNIFORM_BUFFER));
			break;

		default:
			ASSERT(false, "undefined buffer usage type");
			break;
		}
	}
	UniformBuffer::Unbind();
}
