#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)

#include <glad/glad.h>

#include "Assertion.h"
#include "Camera2D.h"
#include "ITexture.h"
#include "Renderer2D.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "TTFont.h"
#include "VertexBuffer.h"

using namespace GameMaker;

static RenderManager* renderManagerPtr = nullptr;
static ResourceManager* resourceManagerPtr = nullptr;

Renderer2D::Renderer2D()
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

	Shader* geometry2D = resourceManagerPtr->Create<Shader>("GameMaker/Shader/Geometry2D.vert", "GameMaker/Shader/Geometry2D.frag");

	shader_.insert({ EType::GEOMETRY, geometry2D });

	bIsInitialized_ = true;
}

Renderer2D::~Renderer2D()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Renderer2D::Release()
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

void Renderer2D::Begin(const Camera2D* camera2D)
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

void Renderer2D::End()
{
	CHECK(bIsBegin_);

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	GLboolean originEnableCull;
	GL_FAILED(glGetBooleanv(GL_CULL_FACE, &originEnableCull));

	renderManagerPtr->SetDepthMode(false);
	renderManagerPtr->SetCullFaceMode(false);
	{
		//const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
		//uint32_t bufferByteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
		//vertexBuffer_->SetBufferData(vertexPtr, bufferByteSize);

		//GL_FAILED(glBindVertexArray(vertexArrayObject_));

		//while (commandQueue_.empty())
		//{
		//	RenderCommand command = commandQueue_.front();
		//	commandQueue_.pop();

		//	Shader* shader = shader_.at(command.type);
		//	shader->Bind();

		//	GL_FAILED(glDrawArrays(static_cast<GLenum>(command.drawMode), command.startVertexIndex, command.vertexCount));

		//	shader->Unbind();
		//}

		//GL_FAILED(glBindVertexArray(0));
	}
	renderManagerPtr->SetCullFaceMode(static_cast<bool>(originEnableCull));
	renderManagerPtr->SetDepthMode(static_cast<bool>(originEnableDepth));

	bIsBegin_ = false;
}
//
//void GameMaker::Renderer::DrawRect(const Vec2f& center, float w, float h, const Vec4f& color, float rotate)
//{
//	//if (commandQueue_.empty())
//	//{
//	//	RenderCommand command;
//
//	//	command.drawMode = EDrawMode::TRIANGLES;
//	//	command.startVertexIndex = 0;
//	//	command.vertexCount = 6;
//	//	command.type = EType::GEOMETRY;
//	//	command.texture = nullptr;
//	//	command.font = nullptr;
//	//}
//	//else
//	//{
//	//	RenderCommand& beforeCommand = commandQueue_.back();
//
//	//}
//
//
//}

#pragma warning(pop)