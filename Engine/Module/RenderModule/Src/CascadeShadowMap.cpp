#include <glad/glad.h>

#include "Assertion.h"

#include "CascadeShadowMap.h"
#include "GLAssertion.h"
#include "RenderModule.h"

CascadeShadowMap::CascadeShadowMap(const ESize& size, uint32_t level)
	: size_(static_cast<uint32_t>(size))
{
	float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GL_FAILED(glGenFramebuffers(1, &shadowMapFBO_));
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO_));

	GL_FAILED(glGenTextures(1, &shadowMapID_));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapID_));

	GL_FAILED(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, size_, size_, static_cast<int32_t>(level) + 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GL_FAILED(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	GL_FAILED(glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, border));

	GL_FAILED(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapID_, 0));
	GL_FAILED(glDrawBuffer(GL_NONE));
	GL_FAILED(glReadBuffer(GL_NONE));

	GLenum state = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	CHECK(state == GL_FRAMEBUFFER_COMPLETE);

	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	bIsInitialized_ = true;
}

CascadeShadowMap::~CascadeShadowMap()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void CascadeShadowMap::Release()
{
	CHECK(bIsInitialized_);

	GL_FAILED(glDeleteTextures(1, &shadowMapID_));
	GL_FAILED(glDeleteFramebuffers(1, &shadowMapFBO_));

	bIsInitialized_ = false;
}

void CascadeShadowMap::Active(uint32_t unit) const
{
	GL_FAILED(glActiveTexture(GL_TEXTURE0 + unit));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapID_));
}

void CascadeShadowMap::Clear()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void CascadeShadowMap::Bind()
{
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO_));
}

void CascadeShadowMap::Unbind()
{
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}