#include <glad/glad.h>

#include "GLAssertion.h"
#include "RenderModule.h"
#include "ShadowMap.h"

ShadowMap::ShadowMap(const ESize& size)
	: size_(static_cast<uint32_t>(size))
{
	float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GL_FAILED(glGenFramebuffers(1, &shadowMapFBO_));
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO_));

	GL_FAILED(glGenTextures(1, &shadowMapID_));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D, shadowMapID_));
	GL_FAILED(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size_, size_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	GL_FAILED(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border));

	GL_FAILED(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapID_, 0));
	GL_FAILED(glDrawBuffer(GL_NONE));
	GL_FAILED(glReadBuffer(GL_NONE));

	GLenum state = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	CHECK(state == GL_FRAMEBUFFER_COMPLETE);

	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	bIsInitialized_ = true;
}

ShadowMap::~ShadowMap()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void ShadowMap::Release()
{
	CHECK(bIsInitialized_);

	GL_FAILED(glDeleteTextures(1, &shadowMapID_));
	GL_FAILED(glDeleteFramebuffers(1, &shadowMapFBO_));

	bIsInitialized_ = false;
}

void ShadowMap::Active(uint32_t unit) const
{
	GL_FAILED(glActiveTexture(GL_TEXTURE0 + unit));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D, shadowMapID_));
}

void ShadowMap::Clear()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::Bind()
{
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO_));
}

void ShadowMap::Unbind()
{
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}