#pragma once

#include <cstdint>

#include "IResource.h"


namespace GameMaker
{
class UniformBuffer : public IResource
{
public:
	enum class EUsage
	{
		STREAM  = 0x88E0,
		STATIC  = 0x88E4,
		DYNAMIC = 0x88E8,
	};


public:
	UniformBuffer(uint32_t byteSize, const EUsage& usage);
	UniformBuffer(const void* bufferPtr, uint32_t byteSize, const EUsage& usage);
	virtual ~UniformBuffer();

	DISALLOW_COPY_AND_ASSIGN(UniformBuffer);

	virtual void Release() override;

	void Bind();
	void Bind(const uint32_t slot); /** https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBufferBase.xhtml */
	void Unbind();

	void SetBufferData(const void* bufferPtr, uint32_t bufferSize);

private:
	uint32_t uniformBufferID_ = 0;
	uint32_t byteSize_ = 0;
	EUsage usage_ = EUsage::STATIC;
};

}