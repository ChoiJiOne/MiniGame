#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)

#include <glad/glad.h>

#include "Assertion.h"
#include "Camera2D.h"
#include "ITexture.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "TTFont.h"
#include "VertexBuffer.h"

using namespace GameMaker;

static RenderManager* renderManagerPtr = nullptr;
static ResourceManager* resourceManagerPtr = nullptr;

Renderer::Renderer()
{
	if (!renderManagerPtr)
	{
		renderManagerPtr = &RenderManager::Get();
	}

	if (!resourceManagerPtr)
	{
		resourceManagerPtr = &ResourceManager::Get();
	}

	uint32_t byteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	VertexBuffer::EUsage usage = VertexBuffer::EUsage::DYNAMIC;
	vertexBuffer_ = resourceManagerPtr->Create<VertexBuffer>(byteSize, usage);

	GL_FAILED(glGenVertexArrays(1, &vertexArrayObject_));
	GL_FAILED(glBindVertexArray(vertexArrayObject_));
	{
		vertexBuffer_->Bind();

		GL_FAILED(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, position))));
		GL_FAILED(glEnableVertexAttribArray(0));

		GL_FAILED(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, uv))));
		GL_FAILED(glEnableVertexAttribArray(1));

		GL_FAILED(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, color))));
		GL_FAILED(glEnableVertexAttribArray(2));

		vertexBuffer_->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));

	bIsInitialized_ = true;
}

Renderer::~Renderer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Renderer::Release()
{
	CHECK(bIsInitialized_);

	if (vertexBuffer_)
	{
		resourceManagerPtr->Destroy(vertexBuffer_);
		vertexBuffer_ = nullptr;
	}

	GL_FAILED(glDeleteVertexArrays(1, &vertexArrayObject_));

	bIsInitialized_ = false;
}

void Renderer::Begin(const Camera2D* camera2D)
{
	CHECK(!bIsBegin_);

	if (!camera2D)
	{
		float w = 0.0f;
		float h = 0.0f;
		static const float nearZ = -1.0f;
		static const float farZ = 1.0f;
		renderManagerPtr->GetScreenSize<float>(w, h);

		ortho_ = Mat4x4::Ortho(-w * 0.5f, +w * 0.5f, -h * 0.5f, +h * 0.5f, nearZ, farZ);
	}
	else
	{
		ortho_ = camera2D->GetOrtho();
	}

	bIsBegin_ = true;
}

void Renderer::End()
{
	CHECK(bIsBegin_);

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	GLboolean originEnableCull;
	GL_FAILED(glGetBooleanv(GL_CULL_FACE, &originEnableCull));

	renderManagerPtr->SetDepthMode(false);
	renderManagerPtr->SetCullFaceMode(false);
	{
		// Execute Render Command...
	}
	renderManagerPtr->SetCullFaceMode(static_cast<bool>(originEnableCull));
	renderManagerPtr->SetDepthMode(static_cast<bool>(originEnableDepth));

	bIsBegin_ = false;
}

#pragma warning(pop)