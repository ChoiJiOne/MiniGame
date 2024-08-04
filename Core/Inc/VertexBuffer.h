#pragma once

#include <cstdint>

#include "IResource.h"


namespace GameMaker
{
class VertexBuffer : public IResource
{
public:
	enum class EUsage
	{
		STREAM  = 0x88E0,
		STATIC  = 0x88E4,
		DYNAMIC = 0x88E8,
	};


public:
	explicit VertexBuffer(uint32_t byteSize, const EUsage& usage);
	explicit VertexBuffer(const void* bufferPtr, uint32_t byteSize, const EUsage& usage);
	virtual ~VertexBuffer();

	DISALLOW_COPY_AND_ASSIGN(VertexBuffer);

	virtual void Release() override;

	void Bind();
	void Unbind();
	
	void SetBufferData(const void* bufferPtr, uint32_t bufferSize);


private:
	uint32_t vertexBufferID_ = 0;
	uint32_t byteSize_ = 0;
	EUsage usage_ = EUsage::STATIC;
};

}