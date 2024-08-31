#pragma once

#include <vector>
#include <string>

#include "ITexture.h"

class Texture2D : public ITexture
{
public:
	/** https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml */
	enum class Filter
	{
		NEAREST = 0x2600,
		LINEAR  = 0x2601,
	};

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
	void ReadPixelBufferFromFile(const std::string& path, int32_t& outWidth, int32_t& outHeight, int32_t& outChannels, std::vector<uint8_t>& outPixels, uint32_t& format);
	uint32_t CreateTextureResource(const std::vector<uint8_t>& buffer, uint32_t format, const Filter& filter);

private:
	int32_t width_ = 0;
	int32_t height_ = 0;
	int32_t channels_ = 0;

	uint32_t textureID_ = 0;
};