#pragma once

#include <cstdint>

#include "IResource.h"


namespace GameMaker
{
class IMesh : public IResource
{
public:
	IMesh() = default;
	virtual ~IMesh() {}

	virtual void Release() = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual uint32_t GetIndexCount() const = 0;
	virtual uint32_t GetVertexCount() const = 0;
};

}