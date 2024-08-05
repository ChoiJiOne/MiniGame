#pragma once

#include <vector>
#include <string>

#include "ITexture.h"


namespace GameMaker
{
class Texture2D : public ITexture
{
public:
	explicit Texture2D(const std::string& path, bool bIsVerticallyFlip);
	virtual ~Texture2D();

	DISALLOW_COPY_AND_ASSIGN(Texture2D);

	virtual void Release() override;
	virtual void Active(uint32_t unit) const override;

	bool IsVerticallyFlip() { return bIsVerticallyFlip_; }

	int32_t GetWidth() const { return width_; }
	int32_t GetHeight() const { return height_; }
	int32_t GetChannels() const { return channels_; }

private:
	void ReadPixelBufferFromFile(const std::string& path, int32_t& outWidth, int32_t& outHeight, int32_t& outChannels, std::vector<uint8_t>& outPixels, bool bIsVerticallyFlip);

private:
	bool bIsVerticallyFlip_ = false;

	int32_t width_ = 0;
	int32_t height_ = 0;
	int32_t channels_ = 0;

	uint32_t textureID_ = 0;
};

}