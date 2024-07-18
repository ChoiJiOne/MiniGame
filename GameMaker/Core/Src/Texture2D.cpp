#pragma warning(push)
#pragma warning(disable: 26451)

#include <glad/glad.h>
#include <ktx.h>
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
{
	std::string extension = FileUtils::GetFileExtension(path);

	if (extension == "ktx") /** 반드시 엔진에 내장된 ktx.exe를 사용해서 변환된 텍스처만 사용! */
	{
		ktxTexture* ktxTexturePtr = nullptr;
		CHECK(ktxTexture_CreateFromNamedFile(path.c_str(), KTX_TEXTURE_CREATE_NO_FLAGS, &ktxTexturePtr) == KTX_SUCCESS);

		GL_FAILED(glGenTextures(1, &textureID_));

		GLenum target;
		GLenum glerror;
		CHECK(ktxTexture_GLUpload(ktxTexturePtr, &textureID_, &target, &glerror) == KTX_SUCCESS);

		ktxTexture_Destroy(ktxTexturePtr);
		ktxTexturePtr = nullptr;
	}
	else
	{
		int32_t width;
		int32_t height;
		int32_t channels;
		std::vector<uint8_t> buffer;
		ReadPixelBufferFromFile(path, width, height, channels, buffer, bIsVerticallyFlip);

		GLenum format = 0xFFFF;
		switch (channels)
		{
		case IMAGE_FORMAT_R:
			format = GL_RED;
			break;

		case IMAGE_FORMAT_RG:
			format = GL_RG;
			break;

		case IMAGE_FORMAT_RGB:
			format = GL_RGB;
			break;

		case IMAGE_FORMAT_RGBA:
			format = GL_RGBA;
			break;

		default:
			ASSERT(false, "undefined texture resource format");
			break;
		}

		GL_FAILED(glGenTextures(1, &textureID_));
		GL_FAILED(glBindTexture(GL_TEXTURE_2D, textureID_));
		GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_FAILED(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer.data()));
		GL_FAILED(glGenerateMipmap(GL_TEXTURE_2D));
		GL_FAILED(glBindTexture(GL_TEXTURE_2D, 0));
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

	GL_FAILED(glDeleteTextures(1, &textureID_));

	bIsInitialized_ = false;
}

void Texture2D::Active(uint32_t unit) const
{
	GL_FAILED(glActiveTexture(GL_TEXTURE0 + unit));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D, textureID_));
}

void Texture2D::ReadPixelBufferFromFile(const std::string& path, int32_t& outWidth, int32_t& outHeight, int32_t& outChannels, std::vector<uint8_t>& outPixels, bool bIsVerticallyFlip)
{
	stbi_set_flip_vertically_on_load(static_cast<int32_t>(bIsVerticallyFlip));

	uint8_t* bufferPtr = stbi_load(path.c_str(), &outWidth, &outHeight, &outChannels, 0);
	ASSERT(bufferPtr != nullptr, "failed to load %s file", path.c_str());

	std::size_t bufferSize = static_cast<std::size_t>(outWidth * outHeight * outChannels);
	outPixels.resize(bufferSize);

	std::copy(bufferPtr, bufferPtr + bufferSize, outPixels.data());

	stbi_image_free(bufferPtr);
	bufferPtr = nullptr;
}

#pragma warning(pop)