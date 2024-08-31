#pragma warning(push)
#pragma warning(disable: 26451)

#include <map>

#include <glad/glad.h>
#include <stb_image.h>

#include "Assertion.h"
#include "Texture2D.h"

#define PIXEL_FORMAT_R    1
#define PIXEL_FORMAT_RG   2
#define PIXEL_FORMAT_RGB  3
#define PIXEL_FORMAT_RGBA 4

/** https://www.oldunreal.com/editing/s3tc/ARB_texture_compression.pdf */
#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

/** https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dds-header */
struct DDSHeader 
{
	uint8_t  magic[4];
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwHeight;
	uint32_t dwWidth;
	uint32_t dwPitchOrLinearSize;
	uint32_t dwDepth;
	uint32_t dwMipMapCount;
	uint32_t dwReserved1[11];

	/** DDS_PIXEL_FORMAT Ω√¿€. */
	uint32_t dwPixelSize;
	uint32_t dwPixelFlags;
	uint32_t dwFourCC;
	uint32_t dwRGBBitCount;
	uint32_t dwRBitMask;
	uint32_t dwGBitMask;
	uint32_t dwBBitMask;
	uint32_t dwABitMask;
	/** DDS_PIXEL_FORMAT ≥°. */

	uint32_t dwCaps;
	uint32_t dwCaps2;
	uint32_t dwCaps3;
	uint32_t dwCaps4;
	uint32_t dwReserved2;
};

Texture2D::Texture2D(const std::string& path, const Filter& filter)
{
	uint32_t format = 0xFFFF;
	std::vector<uint8_t> buffer;
	ReadPixelBufferFromImageFile(path, width_, height_, channels_, buffer, format);

	textureID_ = CreateTextureResource(buffer, format, filter);
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

	GL_CHECK(glDeleteTextures(1, &textureID_));

	bIsInitialized_ = false;
}

void Texture2D::Active(uint32_t unit) const
{
	GL_CHECK(glActiveTexture(GL_TEXTURE0 + unit));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureID_));
}

void Texture2D::ReadPixelBufferFromImageFile(const std::string& path, int32_t& outWidth, int32_t& outHeight, int32_t& outChannels, std::vector<uint8_t>& outPixels, uint32_t& format)
{
	uint8_t* bufferPtr = stbi_load(path.c_str(), &outWidth, &outHeight, &outChannels, 0);
	ASSERT(bufferPtr != nullptr, "failed to load %s file", path.c_str());

	std::size_t bufferSize = static_cast<std::size_t>(outWidth * outHeight * outChannels);
	outPixels.resize(bufferSize);

	std::copy(bufferPtr, bufferPtr + bufferSize, outPixels.data());

	stbi_image_free(bufferPtr);
	bufferPtr = nullptr;

	static std::map<uint32_t, uint32_t> formats =
	{
		{ PIXEL_FORMAT_R,    GL_RED  },
		{ PIXEL_FORMAT_RG,   GL_RG   },
		{ PIXEL_FORMAT_RGB,  GL_RGB  },
		{ PIXEL_FORMAT_RGBA, GL_RGBA },
	};

	format = formats.at(channels_);
}

uint32_t Texture2D::CreateTextureResource(const std::vector<uint8_t>& buffer, uint32_t format, const Filter& filter)
{
	const void* bufferPtr = reinterpret_cast<const void*>(buffer.data());

	uint32_t textureID = 0;

	GL_CHECK(glGenTextures(1, &textureID));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureID));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter)));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter)));
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, bufferPtr));
	GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

	return textureID;
}

#pragma warning(pop)