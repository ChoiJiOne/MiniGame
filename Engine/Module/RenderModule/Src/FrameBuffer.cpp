#include <glad/glad.h>

#include "Assertion.h"

#include "FrameBuffer.h"
#include "GLAssertion.h"

FrameBuffer::FrameBuffer(int32_t bufferWidth, int32_t bufferHeight)
	: width_(bufferWidth)
	, height_(bufferHeight)
{
	GL_FAILED(glGenFramebuffers(1, &framebufferID_));
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_));

	GL_FAILED(glGenTextures(MAX_COLOR_BUFFER_COUNT, colorBufferID_));

	uint32_t attachments[MAX_COLOR_BUFFER_COUNT];
	for (std::size_t index = 0; index < MAX_COLOR_BUFFER_COUNT; ++index)
	{
		attachments[index] = GL_COLOR_ATTACHMENT0 + index;

		GL_FAILED(glBindTexture(GL_TEXTURE_2D, colorBufferID_[index]));
		GL_FAILED(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bufferWidth, bufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_FAILED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_FAILED(glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[index], GL_TEXTURE_2D, colorBufferID_[index], 0));
	}

	GL_FAILED(glDrawBuffers(MAX_COLOR_BUFFER_COUNT, attachments));

	GL_FAILED(glGenRenderbuffers(1, &depthStencilBufferID_));
	GL_FAILED(glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBufferID_));
	GL_FAILED(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight));
	GL_FAILED(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBufferID_));

	GLenum state = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	CHECK(state == GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	bIsInitialized_ = true;
}

FrameBuffer::~FrameBuffer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void FrameBuffer::Release()
{
	CHECK(bIsInitialized_);

	GL_FAILED(glDeleteRenderbuffers(1, &depthStencilBufferID_));
	GL_FAILED(glDeleteTextures(MAX_COLOR_BUFFER_COUNT, colorBufferID_));
	GL_FAILED(glDeleteFramebuffers(1, &framebufferID_));

	bIsInitialized_ = false;
}

void FrameBuffer::Clear(float red, float green, float blue, float alpha, float depth, uint8_t stencil)
{
	glClearColor(red, green, blue, alpha);
	glClearDepth(depth);
	glClearStencil(stencil);

	GL_FAILED(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void FrameBuffer::Bind()
{
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_));
}

void FrameBuffer::Unbind()
{
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::Active(uint32_t unit) const
{
	CHECK(0 <= targetColorBuffer_ && targetColorBuffer_ < MAX_COLOR_BUFFER_COUNT);

	GL_FAILED(glActiveTexture(GL_TEXTURE0 + unit));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D, colorBufferID_[targetColorBuffer_]));
}

void FrameBuffer::SetTargetColorBuffer(uint32_t index)
{
	CHECK(0 <= index && index < MAX_COLOR_BUFFER_COUNT);
	targetColorBuffer_ = index;
}

// https://stackoverflow.com/questions/32213369/opengl-mutlisampling-multiple-render-target-fbos
void FrameBuffer::Blit(FrameBuffer* src, FrameBuffer* dst)
{
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	GL_FAILED(glBindFramebuffer(GL_READ_FRAMEBUFFER, src->framebufferID_));
	GL_FAILED(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->framebufferID_));

	uint32_t attachments[MAX_COLOR_BUFFER_COUNT] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	for (uint32_t index = 0; index < MAX_COLOR_BUFFER_COUNT; ++index)
	{
		GL_FAILED(glReadBuffer(attachments[index]));
		GL_FAILED(glDrawBuffer(attachments[index]));
		GL_FAILED(glBlitFramebuffer(0, 0, src->width_, src->height_, 0, 0, dst->width_, dst->height_, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	}

	GL_FAILED(glBlitFramebuffer(0, 0, src->width_, src->height_, 0, 0, dst->width_, dst->height_, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST));
	GL_FAILED(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}