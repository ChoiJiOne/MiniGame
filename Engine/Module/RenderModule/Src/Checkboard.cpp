#include <glad/glad.h>

#include "Checkboard.h"
#include "GLAssertion.h"
#include "RenderModule.h"
#include "Shader.h"

Checkboard::Checkboard(const ESize& boardSize, const ESize& size, const Vec4f& color0, const Vec4f& color1)
{
	CHECK(boardSize >= size);
	static Shader* checkboardCreator = nullptr;
	if (!checkboardCreator)
	{
		checkboardCreator = RenderModule::CreateResource<Shader>("Resource/Shader/Checkboard.comp");
	}

	width_ = static_cast<int32_t>(boardSize);
	height_ = static_cast<int32_t>(boardSize);

	GL_FAILED(glGenTextures(1, &textureID_));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D, textureID_));
	GL_FAILED(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_FLOAT, nullptr));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	checkboardCreator->Bind();
	{
		checkboardCreator->SetUniform("size", static_cast<int32_t>(size));
		checkboardCreator->SetUniform("color0", color0);
		checkboardCreator->SetUniform("color1", color1);

		uint32_t workGroupX = static_cast<uint32_t>(width_) / 32;
		uint32_t workGroupY = static_cast<uint32_t>(height_) / 32;
		uint32_t workGroupZ = 1;

		GL_FAILED(glBindImageTexture(0, textureID_, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8));
		GL_FAILED(glDispatchCompute(workGroupX, workGroupY, workGroupZ));
	}
	checkboardCreator->Unbind();

	GL_FAILED(glBindTexture(GL_TEXTURE_2D, 0));

	bIsInitialized_ = true;
}

Checkboard::~Checkboard()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Checkboard::Release()
{
	CHECK(bIsInitialized_);

	GL_FAILED(glDeleteTextures(1, &textureID_));

	bIsInitialized_ = false;
}

void Checkboard::Active(uint32_t unit) const
{
	GL_FAILED(glActiveTexture(GL_TEXTURE0 + unit));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D, textureID_));
}