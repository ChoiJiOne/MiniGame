#pragma once

#include <string>
#include <vector>

#include "ITexture.h"

class Skybox : public ITexture
{
public:
	Skybox(const std::vector<std::string>& path, const Filter& filter);
	virtual ~Skybox();

	virtual void Release() override;
	virtual void Active(uint32_t unit) const override;

private:
	uint32_t textureID_ = 0;
};