#include <glad/glad.h>

#include "GLAssertion.h"
#include "FrameBuffer.h"
#include "RenderModule.h"
#include "Shader.h"
#include "PostEffectComposer.h"

PostEffectComposer::PostEffectComposer()
{
	std::array<Vertex, MAX_VERTEX_SIZE> vertices =
	{
		Vertex(-1.0, -1.0),
		Vertex(+1.0, -1.0),
		Vertex(-1.0, +1.0),
		Vertex(+1.0, +1.0),
		Vertex(-1.0, +1.0),
		Vertex(+1.0, -1.0),
	};

	uint32_t byteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices.size());
	VertexBuffer::EUsage usage = VertexBuffer::EUsage::STATIC;
	vertexBuffer_ = RenderModule::CreateResource<VertexBuffer>(vertices.data(), byteSize, usage);

	GL_FAILED(glGenVertexArrays(1, &vertexArrayObject_));
	GL_FAILED(glBindVertexArray(vertexArrayObject_));
	{
		vertexBuffer_->Bind();

		GL_FAILED(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, position))));
		GL_FAILED(glEnableVertexAttribArray(0));

		vertexBuffer_->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));

	blit_ = RenderModule::CreateResource<Shader>("Resource/Shader/Blit.vert", "Resource/Shader/Blit.frag");
	fade_ = RenderModule::CreateResource<Shader>("Resource/Shader/Blit.vert", "Resource/Shader/Fade.frag");
	grayscale_ = RenderModule::CreateResource<Shader>("Resource/Shader/Blit.vert", "Resource/Shader/Grayscale.frag");
	blur_ = RenderModule::CreateResource<Shader>("Resource/Shader/Blit.vert", "Resource/Shader/Blur.frag");

	bIsInitialized_ = true;
}

PostEffectComposer::~PostEffectComposer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void PostEffectComposer::Release()
{
	if (blur_)
	{
		RenderModule::DestroyResource(blur_);
		blur_ = nullptr;
	}

	if (grayscale_)
	{
		RenderModule::DestroyResource(grayscale_);
		grayscale_ = nullptr;
	}

	if (fade_)
	{
		RenderModule::DestroyResource(fade_);
		fade_ = nullptr;
	}

	if (blit_)
	{
		RenderModule::DestroyResource(blit_);
		blit_ = nullptr;
	}

	if (vertexBuffer_)
	{
		RenderModule::DestroyResource(vertexBuffer_);
		vertexBuffer_ = nullptr;
	}

	GL_FAILED(glDeleteVertexArrays(1, &vertexArrayObject_));

	bIsInitialized_ = false;
}

void PostEffectComposer::Blit(FrameBuffer* framebuffer, uint32_t index)
{
	blit_->Bind();
	{
		framebuffer->SetTargetColorBuffer(index);
		framebuffer->Active(FRAME_BUFFER_BIND_SLOT);

		GL_FAILED(glBindVertexArray(vertexArrayObject_));
		RenderModule::ExecuteDrawVertex(MAX_VERTEX_SIZE, EDrawMode::TRIANGLES);
		GL_FAILED(glBindVertexArray(0));
	}
	blit_->Unbind();
}

void PostEffectComposer::Fade(FrameBuffer* framebuffer, uint32_t index, float bias)
{
	fade_->Bind();
	{
		framebuffer->SetTargetColorBuffer(index);
		framebuffer->Active(FRAME_BUFFER_BIND_SLOT);

		fade_->SetUniform("fadeBias", bias);
		
		GL_FAILED(glBindVertexArray(vertexArrayObject_));
		RenderModule::ExecuteDrawVertex(MAX_VERTEX_SIZE, EDrawMode::TRIANGLES);
		GL_FAILED(glBindVertexArray(0));
	}
	fade_->Unbind();
}

void PostEffectComposer::Grayscale(FrameBuffer* framebuffer, uint32_t index)
{
	grayscale_->Bind();
	{
		framebuffer->SetTargetColorBuffer(index);
		framebuffer->Active(FRAME_BUFFER_BIND_SLOT);

		GL_FAILED(glBindVertexArray(vertexArrayObject_));
		RenderModule::ExecuteDrawVertex(MAX_VERTEX_SIZE, EDrawMode::TRIANGLES);
		GL_FAILED(glBindVertexArray(0));
	}
	grayscale_->Unbind();
}

void PostEffectComposer::Blur(FrameBuffer* framebuffer, uint32_t index)
{
	blur_->Bind();
	{
		framebuffer->SetTargetColorBuffer(index);
		framebuffer->Active(FRAME_BUFFER_BIND_SLOT);

		GL_FAILED(glBindVertexArray(vertexArrayObject_));
		RenderModule::ExecuteDrawVertex(MAX_VERTEX_SIZE, EDrawMode::TRIANGLES);
		GL_FAILED(glBindVertexArray(0));
	}
	blur_->Unbind();
}
