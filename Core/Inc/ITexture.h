#pragma once

#include <cstdint>

#include "IResource.h"

class ITexture : public IResource
{
public:
	ITexture() = default;
	virtual ~ITexture() {}

	virtual void Release() = 0;
	virtual void Active(uint32_t unit) const = 0;
};