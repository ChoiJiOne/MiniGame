#pragma once

#include <cstdint>

#include "IResource.h"

class IMesh : public IResource
{
public:
	IMesh() = default;
	virtual ~IMesh() {}

	DISALLOW_COPY_AND_ASSIGN(IMesh);

	virtual void Release() = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual uint32_t GetIndexCount() const = 0;
	virtual uint32_t GetVertexCount() const = 0;
};