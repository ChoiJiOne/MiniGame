#include <array>
#include <thread>

#include <glad/glad.h>
#include <stb_image.h>

#include "Assertion.h"

#include "GLAssertion.h"
#include "Skybox.h"

#define INTERNAL_FORMAT_R    1
#define INTERNAL_FORMAT_RG   2
#define INTERNAL_FORMAT_RGB  3
#define INTERNAL_FORMAT_RGBA 4

Skybox::Skybox(const std::string& rightPath, const std::string& leftPath, const std::string& topPath, const std::string& bottomPath, const std::string& frontPath, const std::string& backPath)
{
	std::array<std::tuple<std::string, GLenum, PixelBuffer>, 6> resourceInfos =
	{
		std::tuple<std::string, GLenum, PixelBuffer>{ rightPath,  GL_TEXTURE_CUBE_MAP_POSITIVE_X, PixelBuffer{} },
		std::tuple<std::string, GLenum, PixelBuffer>{ leftPath,   GL_TEXTURE_CUBE_MAP_NEGATIVE_X, PixelBuffer{} },
		std::tuple<std::string, GLenum, PixelBuffer>{ topPath,    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, PixelBuffer{} },
		std::tuple<std::string, GLenum, PixelBuffer>{ bottomPath, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, PixelBuffer{} },
		std::tuple<std::string, GLenum, PixelBuffer>{ frontPath,  GL_TEXTURE_CUBE_MAP_POSITIVE_Z, PixelBuffer{} },
		std::tuple<std::string, GLenum, PixelBuffer>{ backPath,   GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, PixelBuffer{} },
	};

	std::vector<std::thread> workers;
	for (auto& resourceInfo : resourceInfos)
	{
		workers.push_back(std::thread(ReadPixelBufferFromFile, std::get<0>(resourceInfo), std::ref(std::get<2>(resourceInfo)), false));
	}

	for (auto& worker : workers)
	{
		worker.join();
	}

	GL_FAILED(glGenTextures(1, &cubeMapID_));
	GL_FAILED(glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID_));

	for (const auto& resourceInfo : resourceInfos)
	{
		const PixelBuffer& bufferData = std::get<2>(resourceInfo);

		int32_t width = bufferData.width;
		int32_t height = bufferData.height;
		int32_t channels = bufferData.channels;
		const std::vector<uint8_t>& buffer = bufferData.buffer;

		GLenum target = std::get<1>(resourceInfo);
		GLenum format = 0xFFFF;

		switch (channels)
		{
		case INTERNAL_FORMAT_R:
			format = GL_RED;
			break;

		case INTERNAL_FORMAT_RG:
			format = GL_RG;
			break;

		case INTERNAL_FORMAT_RGB:
			format = GL_RGB;
			break;

		case INTERNAL_FORMAT_RGBA:
			format = GL_RGBA;
			break;

		default:
			ASSERT(false, "undefined format");
			break;
		}

		GL_FAILED(glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer.data()));
	}

	GL_FAILED(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_FAILED(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_FAILED(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_FAILED(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_FAILED(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	GL_FAILED(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

	bIsInitialized_ = true;
}

Skybox::~Skybox()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Skybox::Release()
{
	CHECK(bIsInitialized_);

	GL_FAILED(glDeleteTextures(1, &cubeMapID_));

	bIsInitialized_ = false;
}

void Skybox::Active(uint32_t unit) const
{
	GL_FAILED(glActiveTexture(GL_TEXTURE0 + unit));
	GL_FAILED(glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID_));
}

void Skybox::ReadPixelBufferFromFile(const std::string& path, PixelBuffer& outPixelBuffer, bool bIsVerticallyFlip)
{
	stbi_set_flip_vertically_on_load(static_cast<int32_t>(bIsVerticallyFlip));

	uint8_t* bufferPtr = stbi_load(path.c_str(), &outPixelBuffer.width, &outPixelBuffer.height, &outPixelBuffer.channels, 0);
	ASSERT(bufferPtr != nullptr, "failed to load %s file", path.c_str());

	std::size_t bufferSize = static_cast<std::size_t>(outPixelBuffer.width * outPixelBuffer.height * outPixelBuffer.channels);
	outPixelBuffer.buffer.resize(bufferSize);

	std::copy(bufferPtr, bufferPtr + bufferSize, outPixelBuffer.buffer.data());

	stbi_image_free(bufferPtr);
	bufferPtr = nullptr;
}