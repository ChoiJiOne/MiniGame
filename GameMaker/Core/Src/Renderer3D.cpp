#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)
#pragma warning(disable: 28020)

#include <glad/glad.h>

#include "Assertion.h"
#include "Camera3D.h"
#include "Renderer3D.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "VertexBuffer.h"

using namespace GameMaker;

static RenderManager* renderManagerPtr = nullptr;
static ResourceManager* resourceManagerPtr = nullptr;

Renderer3D::Renderer3D()
{
	if (!renderManagerPtr)
	{
		renderManagerPtr = &RenderManager::Get();
	}

	if (!resourceManagerPtr)
	{
		resourceManagerPtr = &ResourceManager::Get();
	}

	ResourceManager& resourceManager = ResourceManager::Get();

	std::string vsPath = "GameMaker/Shader/Renderer3D.vert";
	std::string fsPath = "GameMaker/Shader/Renderer3D.frag";
	shader_ = resourceManager.Create<Shader>(vsPath, fsPath);

	uint32_t byteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	VertexBuffer::EUsage usage = VertexBuffer::EUsage::DYNAMIC;
	vertexBuffer_ = resourceManager.Create<VertexBuffer>(byteSize, usage);

	GL_FAILED(glGenVertexArrays(1, &vertexArrayObject_));
	GL_FAILED(glBindVertexArray(vertexArrayObject_));
	{
		vertexBuffer_->Bind();

		GL_FAILED(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, position))));
		GL_FAILED(glEnableVertexAttribArray(0));

		GL_FAILED(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, color))));
		GL_FAILED(glEnableVertexAttribArray(1));

		vertexBuffer_->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));

	bIsInitialized_ = true;
}

Renderer3D::~Renderer3D()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Renderer3D::Release()
{
	CHECK(bIsInitialized_);
	
	if (shader_)
	{
		resourceManagerPtr->Destroy(shader_);
		shader_ = nullptr;
	}

	if (vertexBuffer_)
	{
		resourceManagerPtr->Destroy(vertexBuffer_);
		vertexBuffer_ = nullptr;
	}

	GL_FAILED(glDeleteVertexArrays(1, &vertexArrayObject_));

	bIsInitialized_ = false;
}

void Renderer3D::Begin(const Camera3D* camera3D)
{
	CHECK(!bIsBegin_ && camera3D != nullptr);
	
	shader_->Bind();
	{
		shader_->SetUniform("view", camera3D->GetView());
		shader_->SetUniform("projection", camera3D->GetProjection());
	}
	shader_->Unbind();

	bIsBegin_ = true;
}

void Renderer3D::End()
{
	CHECK(bIsBegin_);
	
	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	GLboolean originEnableCull;
	GL_FAILED(glGetBooleanv(GL_CULL_FACE, &originEnableCull));

	renderManagerPtr->SetCullFaceMode(false);
	renderManagerPtr->SetDepthMode(true);
	{
		shader_->Bind();

		const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
		uint32_t bufferByteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
		vertexBuffer_->SetBufferData(vertexPtr, bufferByteSize);

		GL_FAILED(glBindVertexArray(vertexArrayObject_));

		while (!commandQueue_.empty())
		{
			RenderCommand command = commandQueue_.front();
			commandQueue_.pop();

			GL_FAILED(glDrawArrays(static_cast<GLenum>(command.drawMode), command.startVertexIndex, command.vertexCount));
		}
		
		GL_FAILED(glBindVertexArray(0));

		shader_->Unbind();
	}
	renderManagerPtr->SetDepthMode(static_cast<bool>(originEnableDepth));
	renderManagerPtr->SetCullFaceMode(static_cast<bool>(originEnableCull));

	bIsBegin_ = false;
}

void Renderer3D::DrawLine(const Vec3f& startPos, const Vec3f& endPos, const Vec4f& color)
{
	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::LINES;
		command.startVertexIndex = 0;
		command.vertexCount = 2;
		
		vertices_[command.startVertexIndex + 0].position = Vec4f(startPos.x, startPos.y, startPos.z, 1.0f);
		vertices_[command.startVertexIndex + 0].color = color;

		vertices_[command.startVertexIndex + 1].position = Vec4f(endPos.x, endPos.y, endPos.z, 1.0f);
		vertices_[command.startVertexIndex + 1].color = color;

		commandQueue_.push(command);
	}
	else
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += 2;

			vertices_[startVertexIndex + 0].position = Vec4f(startPos.x, startPos.y, startPos.z, 1.0f);
			vertices_[startVertexIndex + 0].color = color;

			vertices_[startVertexIndex + 1].position = Vec4f(endPos.x, endPos.y, endPos.z, 1.0f);
			vertices_[startVertexIndex + 1].color = color;
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::LINES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = 2;

			vertices_[command.startVertexIndex + 0].position = Vec4f(startPos.x, startPos.y, startPos.z, 1.0f);
			vertices_[command.startVertexIndex + 0].color = color;

			vertices_[command.startVertexIndex + 1].position = Vec4f(endPos.x, endPos.y, endPos.z, 1.0f);
			vertices_[command.startVertexIndex + 1].color = color;
		}
	}
}
void Renderer3D::DrawLine(const Vec3f& startPos, const Vec4f& startColor, const Vec3f& endPos, const Vec4f& endColor)
{
	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::LINES;
		command.startVertexIndex = 0;
		command.vertexCount = 2;

		vertices_[command.startVertexIndex + 0].position = Vec4f(startPos.x, startPos.y, startPos.z, 1.0f);
		vertices_[command.startVertexIndex + 0].color = startColor;

		vertices_[command.startVertexIndex + 1].position = Vec4f(endPos.x, endPos.y, endPos.z, 1.0f);
		vertices_[command.startVertexIndex + 1].color = endColor;

		commandQueue_.push(command);
	}
	else
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += 2;

			vertices_[startVertexIndex + 0].position = Vec4f(startPos.x, startPos.y, startPos.z, 1.0f);
			vertices_[startVertexIndex + 0].color = startColor;

			vertices_[startVertexIndex + 1].position = Vec4f(endPos.x, endPos.y, endPos.z, 1.0f);
			vertices_[startVertexIndex + 1].color = endColor;
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::LINES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = 2;

			vertices_[command.startVertexIndex + 0].position = Vec4f(startPos.x, startPos.y, startPos.z, 1.0f);
			vertices_[command.startVertexIndex + 0].color = startColor;

			vertices_[command.startVertexIndex + 1].position = Vec4f(endPos.x, endPos.y, endPos.z, 1.0f);
			vertices_[command.startVertexIndex + 1].color = endColor;
		}
	}
}

void Renderer3D::DrawQuad(const Mat4x4& world, float width, float height, const Vec4f& color)
{
	float w2 = width * 0.5f;
	float h2 = height * 0.5f;

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::TRIANGLES;
		command.startVertexIndex = 0;
		command.vertexCount = 6;

		vertices_[command.startVertexIndex + 0] = Vertex(Vec4f(-w2, -h2, 0.0f, 1.0f), color);
		vertices_[command.startVertexIndex + 1] = Vertex(Vec4f(+w2, -h2, 0.0f, 1.0f), color);
		vertices_[command.startVertexIndex + 2] = Vertex(Vec4f(+w2, +h2, 0.0f, 1.0f), color);

		vertices_[command.startVertexIndex + 3] = Vertex(Vec4f(-w2, -h2, 0.0f, 1.0f), color);
		vertices_[command.startVertexIndex + 4] = Vertex(Vec4f(+w2, +h2, 0.0f, 1.0f), color);
		vertices_[command.startVertexIndex + 5] = Vertex(Vec4f(-w2, +h2, 0.0f, 1.0f), color);

		for (uint32_t index = 0; index < 6; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices_[command.startVertexIndex + index].position * world;
		}

		commandQueue_.push(command);
	}
	else
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::TRIANGLES)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += 6;

			vertices_[startVertexIndex + 0] = Vertex(Vec4f(-w2, -h2, 0.0f, 1.0f), color);
			vertices_[startVertexIndex + 1] = Vertex(Vec4f(+w2, -h2, 0.0f, 1.0f), color);
			vertices_[startVertexIndex + 2] = Vertex(Vec4f(+w2, +h2, 0.0f, 1.0f), color);

			vertices_[startVertexIndex + 3] = Vertex(Vec4f(-w2, -h2, 0.0f, 1.0f), color);
			vertices_[startVertexIndex + 4] = Vertex(Vec4f(+w2, +h2, 0.0f, 1.0f), color);
			vertices_[startVertexIndex + 5] = Vertex(Vec4f(-w2, +h2, 0.0f, 1.0f), color);

			for (uint32_t index = 0; index < 6; ++index)
			{
				vertices_[startVertexIndex + index].position = vertices_[startVertexIndex + index].position * world;
			}
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::TRIANGLES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = 6;

			vertices_[command.startVertexIndex + 0] = Vertex(Vec4f(-w2, -h2, 0.0f, 1.0f), color);
			vertices_[command.startVertexIndex + 1] = Vertex(Vec4f(+w2, -h2, 0.0f, 1.0f), color);
			vertices_[command.startVertexIndex + 2] = Vertex(Vec4f(+w2, +h2, 0.0f, 1.0f), color);

			vertices_[command.startVertexIndex + 3] = Vertex(Vec4f(-w2, -h2, 0.0f, 1.0f), color);
			vertices_[command.startVertexIndex + 4] = Vertex(Vec4f(+w2, +h2, 0.0f, 1.0f), color);
			vertices_[command.startVertexIndex + 5] = Vertex(Vec4f(-w2, +h2, 0.0f, 1.0f), color);

			for (uint32_t index = 0; index < 6; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices_[command.startVertexIndex + index].position * world;
			}

			commandQueue_.push(command);
		}
	}
}

void Renderer3D::DrawQuadWireframe(const Mat4x4& world, float width, float height, const Vec4f& color)
{
	float w2 = width * 0.5f;
	float h2 = height * 0.5f;

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::LINES;
		command.startVertexIndex = 0;
		command.vertexCount = 8;

		vertices_[command.startVertexIndex + 0].position = Vec4f(-w2, -h2, 0.0f, 1.0f);
		vertices_[command.startVertexIndex + 0].color = color;

		vertices_[command.startVertexIndex + 1].position = Vec4f(+w2, -h2, 0.0f, 1.0f);
		vertices_[command.startVertexIndex + 1].color = color;

		vertices_[command.startVertexIndex + 2].position = Vec4f(+w2, -h2, 0.0f, 1.0f);
		vertices_[command.startVertexIndex + 2].color = color;

		vertices_[command.startVertexIndex + 3].position = Vec4f(+w2, +h2, 0.0f, 1.0f);
		vertices_[command.startVertexIndex + 3].color = color;

		vertices_[command.startVertexIndex + 4].position = Vec4f(+w2, +h2, 0.0f, 1.0f);
		vertices_[command.startVertexIndex + 4].color = color;

		vertices_[command.startVertexIndex + 5].position = Vec4f(-w2, +h2, 0.0f, 1.0f);
		vertices_[command.startVertexIndex + 5].color = color;

		vertices_[command.startVertexIndex + 6].position = Vec4f(-w2, +h2, 0.0f, 1.0f);
		vertices_[command.startVertexIndex + 6].color = color;

		vertices_[command.startVertexIndex + 7].position = Vec4f(-w2, -h2, 0.0f, 1.0f);
		vertices_[command.startVertexIndex + 7].color = color;

		for (uint32_t index = 0; index < 8; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices_[command.startVertexIndex + index].position * world;
		}

		commandQueue_.push(command);
	}
	else
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += 8;

			vertices_[startVertexIndex + 0].position = Vec4f(-w2, -h2, 0.0f, 1.0f);
			vertices_[startVertexIndex + 0].color = color;

			vertices_[startVertexIndex + 1].position = Vec4f(+w2, -h2, 0.0f, 1.0f);
			vertices_[startVertexIndex + 1].color = color;

			vertices_[startVertexIndex + 2].position = Vec4f(+w2, -h2, 0.0f, 1.0f);
			vertices_[startVertexIndex + 2].color = color;

			vertices_[startVertexIndex + 3].position = Vec4f(+w2, +h2, 0.0f, 1.0f);
			vertices_[startVertexIndex + 3].color = color;

			vertices_[startVertexIndex + 4].position = Vec4f(+w2, +h2, 0.0f, 1.0f);
			vertices_[startVertexIndex + 4].color = color;

			vertices_[startVertexIndex + 5].position = Vec4f(-w2, +h2, 0.0f, 1.0f);
			vertices_[startVertexIndex + 5].color = color;

			vertices_[startVertexIndex + 6].position = Vec4f(-w2, +h2, 0.0f, 1.0f);
			vertices_[startVertexIndex + 6].color = color;

			vertices_[startVertexIndex + 7].position = Vec4f(-w2, -h2, 0.0f, 1.0f);
			vertices_[startVertexIndex + 7].color = color;

			for (uint32_t index = 0; index < 8; ++index)
			{
				vertices_[startVertexIndex + index].position = vertices_[startVertexIndex + index].position * world;
			}
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::LINES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = 8;

			vertices_[command.startVertexIndex + 0].position = Vec4f(-w2, -h2, 0.0f, 1.0f);
			vertices_[command.startVertexIndex + 0].color = color;

			vertices_[command.startVertexIndex + 1].position = Vec4f(+w2, -h2, 0.0f, 1.0f);
			vertices_[command.startVertexIndex + 1].color = color;

			vertices_[command.startVertexIndex + 2].position = Vec4f(+w2, -h2, 0.0f, 1.0f);
			vertices_[command.startVertexIndex + 2].color = color;

			vertices_[command.startVertexIndex + 3].position = Vec4f(+w2, +h2, 0.0f, 1.0f);
			vertices_[command.startVertexIndex + 3].color = color;

			vertices_[command.startVertexIndex + 4].position = Vec4f(+w2, +h2, 0.0f, 1.0f);
			vertices_[command.startVertexIndex + 4].color = color;

			vertices_[command.startVertexIndex + 5].position = Vec4f(-w2, +h2, 0.0f, 1.0f);
			vertices_[command.startVertexIndex + 5].color = color;

			vertices_[command.startVertexIndex + 6].position = Vec4f(-w2, +h2, 0.0f, 1.0f);
			vertices_[command.startVertexIndex + 6].color = color;

			vertices_[command.startVertexIndex + 7].position = Vec4f(-w2, -h2, 0.0f, 1.0f);
			vertices_[command.startVertexIndex + 7].color = color;

			for (uint32_t index = 0; index < 8; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices_[command.startVertexIndex + index].position * world;
			}

			commandQueue_.push(command);
		}
	}
}

void Renderer3D::DrawCube(const Mat4x4& world, const Vec3f& extents, const Vec4f& color)
{
	Vec3f minPos = -extents * 0.5f;
	Vec3f maxPos = extents * 0.5f;

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::LINES;
		command.startVertexIndex = 0;
		command.vertexCount = 24;
		
		vertices_[command.startVertexIndex + 0] = Vertex(Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 1] = Vertex(Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 2] = Vertex(Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 3] = Vertex(Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 4] = Vertex(Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 5] = Vertex(Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 6] = Vertex(Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 7] = Vertex(Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 8] = Vertex(Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 9] = Vertex(Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 10] = Vertex(Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 11] = Vertex(Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 12] = Vertex(Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 13] = Vertex(Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 14] = Vertex(Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 15] = Vertex(Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 16] = Vertex(Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 17] = Vertex(Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 18] = Vertex(Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 19] = Vertex(Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 20] = Vertex(Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 21] = Vertex(Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color);

		vertices_[command.startVertexIndex + 22] = Vertex(Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color);
		vertices_[command.startVertexIndex + 23] = Vertex(Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color);

		for (uint32_t index = 0; index < 24; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices_[command.startVertexIndex + index].position * world;
		}

		commandQueue_.push(command);
	}
	else
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += 24;

			vertices_[startVertexIndex + 0] = Vertex(Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color);
			vertices_[startVertexIndex + 1] = Vertex(Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color);

			vertices_[startVertexIndex + 2] = Vertex(Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color);
			vertices_[startVertexIndex + 3] = Vertex(Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color);

			vertices_[startVertexIndex + 4] = Vertex(Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color);
			vertices_[startVertexIndex + 5] = Vertex(Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color);

			vertices_[startVertexIndex + 6] = Vertex(Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color);
			vertices_[startVertexIndex + 7] = Vertex(Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color);

			vertices_[startVertexIndex + 8] = Vertex(Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color);
			vertices_[startVertexIndex + 9] = Vertex(Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color);

			vertices_[startVertexIndex + 10] = Vertex(Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color);
			vertices_[startVertexIndex + 11] = Vertex(Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color);

			vertices_[startVertexIndex + 12] = Vertex(Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color);
			vertices_[startVertexIndex + 13] = Vertex(Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color);

			vertices_[startVertexIndex + 14] = Vertex(Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color);
			vertices_[startVertexIndex + 15] = Vertex(Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color);

			vertices_[startVertexIndex + 16] = Vertex(Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color);
			vertices_[startVertexIndex + 17] = Vertex(Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color);

			vertices_[startVertexIndex + 18] = Vertex(Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color);
			vertices_[startVertexIndex + 19] = Vertex(Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color);

			vertices_[startVertexIndex + 20] = Vertex(Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color);
			vertices_[startVertexIndex + 21] = Vertex(Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color);

			vertices_[startVertexIndex + 22] = Vertex(Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color);
			vertices_[startVertexIndex + 23] = Vertex(Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color);

			for (uint32_t index = 0; index < 24; ++index)
			{
				vertices_[startVertexIndex + index].position = vertices_[startVertexIndex + index].position * world;
			}
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::LINES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = 24;

			vertices_[command.startVertexIndex + 0] = Vertex(Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 1] = Vertex(Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 2] = Vertex(Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 3] = Vertex(Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 4] = Vertex(Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 5] = Vertex(Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 6] = Vertex(Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 7] = Vertex(Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 8] = Vertex(Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 9] = Vertex(Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 10] = Vertex(Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 11] = Vertex(Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 12] = Vertex(Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 13] = Vertex(Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 14] = Vertex(Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 15] = Vertex(Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 16] = Vertex(Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 17] = Vertex(Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 18] = Vertex(Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 19] = Vertex(Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 20] = Vertex(Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 21] = Vertex(Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color);

			vertices_[command.startVertexIndex + 22] = Vertex(Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color);
			vertices_[command.startVertexIndex + 23] = Vertex(Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color);

			for (uint32_t index = 0; index < 24; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices_[command.startVertexIndex + index].position * world;
			}

			commandQueue_.push(command);
		}
	}
}

void Renderer3D::DrawViewfrustum(const Mat4x4& view, const Mat4x4& projection, const Vec4f& color)
{
	static const uint32_t MAX_FRUSTUM_CORNER = 8;
	static const std::array<Vec3f, MAX_FRUSTUM_CORNER> corners =
	{
		Vec3f(0.0f, 0.0f, 0.0f),
		Vec3f(0.0f, 0.0f, 1.0f),
		Vec3f(0.0f, 1.0f, 0.0f),
		Vec3f(0.0f, 1.0f, 1.0f),
		Vec3f(1.0f, 0.0f, 0.0f),
		Vec3f(1.0f, 0.0f, 1.0f),
		Vec3f(1.0f, 1.0f, 0.0f),
		Vec3f(1.0f, 1.0f, 1.0f),
	};

	Mat4x4 inv = Mat4x4::Inverse(view * projection);
	std::array<Vec3f, MAX_FRUSTUM_CORNER> frustumCorners;
	for (int32_t index = 0; index < MAX_FRUSTUM_CORNER; ++index)
	{
		Vec4f transform = Vec4f(2.0f * corners[index].x - 1.0f, 2.0f * corners[index].y - 1.0f, 2.0f * corners[index].z - 1.0f, 1.0f) * inv;
		frustumCorners[index].x = transform.x / transform.w;
		frustumCorners[index].y = transform.y / transform.w;
		frustumCorners[index].z = transform.z / transform.w;
	}

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::LINES;
		command.startVertexIndex = 0;
		command.vertexCount = 24;

		vertices_[command.startVertexIndex + 0] = Vertex(Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color);
		vertices_[command.startVertexIndex + 1] = Vertex(Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color);

		vertices_[command.startVertexIndex + 2] = Vertex(Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color);
		vertices_[command.startVertexIndex + 3] = Vertex(Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color);

		vertices_[command.startVertexIndex + 4] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);
		vertices_[command.startVertexIndex + 5] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);

		vertices_[command.startVertexIndex + 4] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);
		vertices_[command.startVertexIndex + 5] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);

		vertices_[command.startVertexIndex + 6] = Vertex(Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color);
		vertices_[command.startVertexIndex + 7] = Vertex(Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color);

		vertices_[command.startVertexIndex + 8] = Vertex(Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color);
		vertices_[command.startVertexIndex + 9] = Vertex(Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color);

		vertices_[command.startVertexIndex + 10] = Vertex(Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color);
		vertices_[command.startVertexIndex + 11] = Vertex(Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color);

		vertices_[command.startVertexIndex + 12] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);
		vertices_[command.startVertexIndex + 13] = Vertex(Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color);

		vertices_[command.startVertexIndex + 14] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);
		vertices_[command.startVertexIndex + 15] = Vertex(Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color);

		vertices_[command.startVertexIndex + 16] = Vertex(Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color);
		vertices_[command.startVertexIndex + 17] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);

		vertices_[command.startVertexIndex + 18] = Vertex(Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color);
		vertices_[command.startVertexIndex + 19] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);

		vertices_[command.startVertexIndex + 20] = Vertex(Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color);
		vertices_[command.startVertexIndex + 21] = Vertex(Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color);

		vertices_[command.startVertexIndex + 22] = Vertex(Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color);
		vertices_[command.startVertexIndex + 23] = Vertex(Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color);

		commandQueue_.push(command);
	}
	else
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += 24;

			vertices_[startVertexIndex + 0] = Vertex(Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color);
			vertices_[startVertexIndex + 1] = Vertex(Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color);

			vertices_[startVertexIndex + 2] = Vertex(Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color);
			vertices_[startVertexIndex + 3] = Vertex(Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color);

			vertices_[startVertexIndex + 4] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);
			vertices_[startVertexIndex + 5] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);

			vertices_[startVertexIndex + 4] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);
			vertices_[startVertexIndex + 5] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);

			vertices_[startVertexIndex + 6] = Vertex(Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color);
			vertices_[startVertexIndex + 7] = Vertex(Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color);

			vertices_[startVertexIndex + 8] = Vertex(Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color);
			vertices_[startVertexIndex + 9] = Vertex(Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color);

			vertices_[startVertexIndex + 10] = Vertex(Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color);
			vertices_[startVertexIndex + 11] = Vertex(Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color);

			vertices_[startVertexIndex + 12] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);
			vertices_[startVertexIndex + 13] = Vertex(Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color);

			vertices_[startVertexIndex + 14] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);
			vertices_[startVertexIndex + 15] = Vertex(Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color);

			vertices_[startVertexIndex + 16] = Vertex(Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color);
			vertices_[startVertexIndex + 17] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);

			vertices_[startVertexIndex + 18] = Vertex(Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color);
			vertices_[startVertexIndex + 19] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);

			vertices_[startVertexIndex + 20] = Vertex(Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color);
			vertices_[startVertexIndex + 21] = Vertex(Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color);

			vertices_[startVertexIndex + 22] = Vertex(Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color);
			vertices_[startVertexIndex + 23] = Vertex(Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color);

		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::LINES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = 24;

			vertices_[command.startVertexIndex + 0] = Vertex(Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color);
			vertices_[command.startVertexIndex + 1] = Vertex(Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color);

			vertices_[command.startVertexIndex + 2] = Vertex(Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color);
			vertices_[command.startVertexIndex + 3] = Vertex(Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color);

			vertices_[command.startVertexIndex + 4] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);
			vertices_[command.startVertexIndex + 5] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);

			vertices_[command.startVertexIndex + 4] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);
			vertices_[command.startVertexIndex + 5] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);

			vertices_[command.startVertexIndex + 6] = Vertex(Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color);
			vertices_[command.startVertexIndex + 7] = Vertex(Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color);

			vertices_[command.startVertexIndex + 8] = Vertex(Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color);
			vertices_[command.startVertexIndex + 9] = Vertex(Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color);

			vertices_[command.startVertexIndex + 10] = Vertex(Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color);
			vertices_[command.startVertexIndex + 11] = Vertex(Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color);

			vertices_[command.startVertexIndex + 12] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);
			vertices_[command.startVertexIndex + 13] = Vertex(Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color);

			vertices_[command.startVertexIndex + 14] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);
			vertices_[command.startVertexIndex + 15] = Vertex(Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color);

			vertices_[command.startVertexIndex + 16] = Vertex(Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color);
			vertices_[command.startVertexIndex + 17] = Vertex(Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color);

			vertices_[command.startVertexIndex + 18] = Vertex(Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color);
			vertices_[command.startVertexIndex + 19] = Vertex(Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color);

			vertices_[command.startVertexIndex + 20] = Vertex(Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color);
			vertices_[command.startVertexIndex + 21] = Vertex(Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color);

			vertices_[command.startVertexIndex + 22] = Vertex(Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color);
			vertices_[command.startVertexIndex + 23] = Vertex(Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color);

			commandQueue_.push(command);
		}
	}
}

#pragma warning(pop)