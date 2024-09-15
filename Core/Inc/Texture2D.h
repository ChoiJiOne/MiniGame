#pragma once

#include <vector>
#include <string>

#include "ITexture.h"

class Texture2D : public ITexture
{
public:
	Texture2D(const std::string& path, const Filter& filter);
	virtual ~Texture2D();

	DISALLOW_COPY_AND_ASSIGN(Texture2D);

	virtual void Release() override;
	virtual void Active(uint32_t unit) const override;

	int32_t GetWidth() const { return width_; }
	int32_t GetHeight() const { return height_; }
	int32_t GetChannels() const { return channels_; }
	
private:
	uint32_t CreateTextureFromImage(const std::string& path, const Filter& filter);
	uint32_t CreateTextureFromDDS(const std::string& path, const Filter& filter);

private:
	int32_t width_ = 0;
	int32_t height_ = 0;
	int32_t channels_ = 0;

	uint32_t textureID_ = 0;
};