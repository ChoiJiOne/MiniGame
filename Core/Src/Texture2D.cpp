#pragma warning(push)
#pragma warning(disable: 26451)

#include <map>

#include <glad/glad.h>
#include <stb_image.h>

#include "Assertion.h"
#include "FileUtils.h"
#include "Texture2D.h"

#define IMAGE_FORMAT_R    1
#define IMAGE_FORMAT_RG   2
#define IMAGE_FORMAT_RGB  3
#define IMAGE_FORMAT_RGBA 4

using namespace GameMaker;

Texture2D::Texture2D(const std::string& path, bool bIsVerticallyFlip)
	: bIsVerticallyFlip_(bIsVerticallyFlip)
{
	uint32_t format = 0xFFFF;
	std::vector<uint8_t> buffer;
	ReadPixelBufferFromFile(path, width_, height_, channels_, buffer, format, bIsVerticallyFlip_);

	textureID_ = CreateTextureResource(buffer, format);
	bIsInitialized_ = true;
}

Texture2D::~Texture2D()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Texture2D::Release()
{
	CHECK(bIsInitialized_);

	GL_FAILED(glDeleteTextures(1, &textureID_));

	bIsInitialized_ = false;
}

void Texture2D::Active(uint32_t unit) const
{
	GL_FAILED(glActiveTexture(GL_TEXTURE0 + unit));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D, textureID_));
}

void Texture2D::ReadPixelBufferFromFile(const std::string& path, int32_t& outWidth, int32_t& outHeight, int32_t& outChannels, std::vector<uint8_t>& outPixels, uint32_t& format, bool bIsVerticallyFlip)
{
	stbi_set_flip_vertically_on_load(static_cast<int32_t>(bIsVerticallyFlip));

	uint8_t* bufferPtr = stbi_load(path.c_str(), &outWidth, &outHeight, &outChannels, 0);
	ASSERT(bufferPtr != nullptr, "failed to load %s file", path.c_str());

	std::size_t bufferSize = static_cast<std::size_t>(outWidth * outHeight * outChannels);
	outPixels.resize(bufferSize);

	std::copy(bufferPtr, bufferPtr + bufferSize, outPixels.data());

	stbi_image_free(bufferPtr);
	bufferPtr = nullptr;

	static std::map<uint32_t, uint32_t> formats =
	{
		{ IMAGE_FORMAT_R,    GL_RED  },
		{ IMAGE_FORMAT_RG,   GL_RG   },
		{ IMAGE_FORMAT_RGB,  GL_RGB  },
		{ IMAGE_FORMAT_RGBA, GL_RGBA },
	};

	format = formats.at(channels_);
}

uint32_t Texture2D::CreateTextureResource(const std::vector<uint8_t>& buffer, uint32_t format)
{
	const void* bufferPtr = reinterpret_cast<const void*>(buffer.data());

	uint32_t textureID = 0;

	GL_FAILED(glGenTextures(1, &textureID));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D, textureID));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_FAILED(glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, bufferPtr));
	GL_FAILED(glGenerateMipmap(GL_TEXTURE_2D));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D, 0));

	return textureID;
}

#pragma warning(pop)