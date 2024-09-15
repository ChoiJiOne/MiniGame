#pragma once

#include <cstdint>

#include "IResource.h"

class ITexture : public IResource
{
public:
	/** https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml */
	enum class Filter
	{
		NEAREST = 0x2600,
		LINEAR  = 0x2601,
	};

public:
	ITexture() = default;
	virtual ~ITexture() {}

	virtual void Release() = 0;
	virtual void Active(uint32_t unit) const = 0;
};