#pragma once

#include <cstdint>

#include "IResource.h"

class VertexBuffer : public IResource
{
public:
	enum class Usage
	{
		STREAM  = 0x88E0,
		STATIC  = 0x88E4,
		DYNAMIC = 0x88E8,
	};

public:
	VertexBuffer(uint32_t byteSize, const Usage& usage);
	VertexBuffer(const void* bufferPtr, uint32_t byteSize, const Usage& usage);
	virtual ~VertexBuffer();

	DISALLOW_COPY_AND_ASSIGN(VertexBuffer);

	virtual void Release() override;

	void Bind();
	void Unbind();

	void SetBufferData(const void* bufferPtr, uint32_t bufferSize);

private:
	uint32_t vertexBufferID_ = 0;
	uint32_t byteSize_ = 0;
	Usage usage_ = Usage::STATIC;
};