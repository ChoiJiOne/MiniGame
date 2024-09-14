#pragma warning(push)
#pragma warning(disable: 26451)

#include <map>

#include <glad/glad.h>
#include <stb_image.h>

#include "Assertion.h"
#include "GameUtils.h"
#include "IApp.h"
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

	/** DDS_PIXEL_FORMAT 시작. */
	uint32_t dwPixelSize;
	uint32_t dwPixelFlags;
	uint32_t dwFourCC;
	uint32_t dwRGBBitCount;
	uint32_t dwRBitMask;
	uint32_t dwGBitMask;
	uint32_t dwBBitMask;
	uint32_t dwABitMask;
	/** DDS_PIXEL_FORMAT 끝. */

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
	std::string extension = GameUtils::GetFileExtension(path);
	if (extension == "dds") /** S3TC(DXTn) 압축 포멧이라면. */
	{
		textureID_ = CreateTextureFromDDS(path, filter);
	}
	else /** 비압축 포멧 (PNG, JPG, BMP, TGA 등등...) */
	{
		textureID_ = CreateTextureFromImage(path, filter);
	}

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

uint32_t Texture2D::CreateTextureFromImage(const std::string& path, const Filter& filter)
{
	uint8_t* imagePtr = stbi_load(path.c_str(), &width_, &height_, &channels_, 0);
	ASSERT(imagePtr != nullptr, "failed to load %s file", path.c_str());
	
	std::size_t bufferSize = static_cast<std::size_t>(width_ * height_ * channels_);
	std::vector<uint8_t> buffer(bufferSize);
	std::copy(imagePtr, imagePtr + bufferSize, buffer.data());

	stbi_image_free(imagePtr);
	imagePtr = nullptr;

	static std::map<uint32_t, uint32_t> formats =
	{
		{ PIXEL_FORMAT_R,    GL_RED  },
		{ PIXEL_FORMAT_RG,   GL_RG   },
		{ PIXEL_FORMAT_RGB,  GL_RGB  },
		{ PIXEL_FORMAT_RGBA, GL_RGBA },
	};

	GLenum format = formats.at(channels_);
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

uint32_t Texture2D::CreateTextureFromDDS(const std::string& path, const Filter& filter)
{
	bool bHasExt = IApp::Get()->HasGLExtension("GL_EXT_texture_compression_s3tc");
	ASSERT(bHasExt, "This hardware does not support 'GL_EXT_texture_compression_s3tc'.");

	std::vector<uint8_t> buffer = GameUtils::ReadFile(path);
	DDSHeader* ddsFilePtr = reinterpret_cast<DDSHeader*>(buffer.data());

	std::string ddsFileCode;
	ddsFileCode += ddsFilePtr->magic[0];
	ddsFileCode += ddsFilePtr->magic[1];
	ddsFileCode += ddsFilePtr->magic[2];
	ddsFileCode += ddsFilePtr->magic[3];
	ASSERT(ddsFileCode == "DDS ", "Invalid %s DDS file code", path.c_str());

	width_ = ddsFilePtr->dwWidth;
	height_ = ddsFilePtr->dwHeight;
	channels_ = ddsFilePtr->dwPixelSize / 4;
	uint32_t linearSize = ddsFilePtr->dwPitchOrLinearSize;
	uint32_t mipMapCount = ddsFilePtr->dwMipMapCount;
	uint32_t bufferSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	uint8_t* bufferPtr = reinterpret_cast<uint8_t*>(&ddsFilePtr[1]);

	uint32_t format = GL_RGBA;
	uint32_t blockSize = 0;
	switch (ddsFilePtr->dwFourCC)
	{
	case FOURCC_DXT1:
		blockSize = 8;
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;

	case FOURCC_DXT3:
		blockSize = 16;
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;

	case FOURCC_DXT5:
		blockSize = 16;
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;

	default:
		blockSize = 4;
		format = GL_RGBA;
	}

	uint32_t textureID;
	GL_CHECK(glGenTextures(1, &textureID));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureID));
	GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	int32_t width = width_;
	int32_t height = height_;
	for (uint32_t level = 0, offset = 0; level <= mipMapCount; ++level)
	{
		if (width == 0 || height == 0)
		{
			mipMapCount--;
			continue;
		}

		uint32_t size = 0;
		if (!ddsFilePtr->dwFourCC) /** 압축 포멧 X */
		{
			size = width * height * blockSize;
			GL_CHECK(glTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, format, GL_UNSIGNED_BYTE, bufferPtr + offset));
		}
		else
		{
			size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
			GL_CHECK(glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, bufferPtr + offset));
		}

		offset += size;
		width /= 2;
		height /= 2;
	}

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

	return textureID;
}

#pragma warning(pop)