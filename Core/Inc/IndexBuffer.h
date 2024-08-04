#pragma once

#include <cstdint>

#include "IResource.h"


namespace GameMaker
{
class IndexBuffer : public IResource
{
public:
	explicit IndexBuffer(const void* bufferPtr, uint32_t indexCount);
	virtual ~IndexBuffer();

	DISALLOW_COPY_AND_ASSIGN(IndexBuffer);

	virtual void Release() override;

	void Bind();
	void Unbind();
	uint32_t GetIndexCount() const { return indexCount_; }

private:
	uint32_t indexBufferID_ = 0;
	uint32_t indexCount_ = 0;
};

}