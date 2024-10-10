#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)
#pragma warning(disable: 28020)

#include <glad/glad.h>

#include "Assertion.h"
#include "Camera3D.h"
#include "DebugDrawManager3D.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "VertexBuffer.h"

DebugDrawManager3D& DebugDrawManager3D::Get()
{
	static DebugDrawManager3D instance;
	return instance;
}

void DebugDrawManager3D::Startup()
{
	app_ = IApp::Get();

	uint32_t byteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	VertexBuffer::Usage usage = VertexBuffer::Usage::DYNAMIC;
	vertexBuffer_ = ResourceManager::Get().Create<VertexBuffer>(byteSize, usage);

	GL_CHECK(glGenVertexArrays(1, &vertexArrayObject_));
	GL_CHECK(glBindVertexArray(vertexArrayObject_));
	{
		vertexBuffer_->Bind();

		GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, position))));
		GL_CHECK(glEnableVertexAttribArray(0));

		GL_CHECK(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, color))));
		GL_CHECK(glEnableVertexAttribArray(1));

		vertexBuffer_->Unbind();
	}
	GL_CHECK(glBindVertexArray(0));

	shader_ = ResourceManager::Get().Create<Shader>("MiniGame/Shader/DebugDraw3D.vert", "MiniGame/Shader/DebugDraw3D.frag");
}

void DebugDrawManager3D::Shutdown()
{
	ResourceManager::Get().Destroy(vertexBuffer_);
	vertexBuffer_ = nullptr;

	ResourceManager::Get().Destroy(shader_);
	shader_ = nullptr;

	GL_CHECK(glDeleteVertexArrays(1, &vertexArrayObject_));

	app_ = nullptr;
}

void DebugDrawManager3D::Begin(const Camera3D* camera3D)
{
	CHECK(!bIsBegin_ && camera3D != nullptr);

	shader_->Bind();
	{
		shader_->SetUniform("view", camera3D->GetView());
		shader_->SetUniform("projection", camera3D->GetProjection());
	}
	shader_->Unbind();

	GLboolean originEnableDepth;
	GL_CHECK(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	GLboolean originEnableCull;
	GL_CHECK(glGetBooleanv(GL_CULL_FACE, &originEnableCull));

	originEnableDepth_ = static_cast<bool>(originEnableDepth);
	originEnableCull_ = static_cast<bool>(originEnableCull);

	app_->SetDepthMode(true);
	app_->SetCullFaceMode(false);

	bIsBegin_ = true;
}

void DebugDrawManager3D::End()
{
	CHECK(bIsBegin_);

	Flush();

	app_->SetCullFaceMode(originEnableCull_);
	app_->SetDepthMode(originEnableDepth_);

	bIsBegin_ = false;
}

void DebugDrawManager3D::DrawLine(const Vec3f& startPos, const Vec3f& endPos, const Vec4f& color)
{
	static const uint32_t MAX_VERTEX_SIZE = 2;
	if (IsFullCommandQueue(MAX_VERTEX_SIZE))
	{
		Flush();
	}

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == DrawMode::LINES)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += MAX_VERTEX_SIZE;

			vertices_[startVertexIndex + 0].position = Vec4f(startPos.x, startPos.y, startPos.z, 1.0f);
			vertices_[startVertexIndex + 0].color = color;
			vertices_[startVertexIndex + 1].position = Vec4f(endPos.x, endPos.y, endPos.z, 1.0f);
			vertices_[startVertexIndex + 1].color = color;
			return;
		}
	}

	uint32_t startVertexIndex = 0;
	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();
		startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
	}

	RenderCommand command;
	command.drawMode = DrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = MAX_VERTEX_SIZE;

	vertices_[command.startVertexIndex + 0].position = Vec4f(startPos.x, startPos.y, startPos.z, 1.0f);
	vertices_[command.startVertexIndex + 0].color = color;
	vertices_[command.startVertexIndex + 1].position = Vec4f(endPos.x, endPos.y, endPos.z, 1.0f);
	vertices_[command.startVertexIndex + 1].color = color;

	commandQueue_.push(command);
}

void DebugDrawManager3D::DrawLine(const Vec3f& startPos, const Vec4f& startColor, const Vec3f& endPos, const Vec4f& endColor)
{
	static const uint32_t MAX_VERTEX_SIZE = 2;
	if (IsFullCommandQueue(MAX_VERTEX_SIZE))
	{
		Flush();
	}

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == DrawMode::LINES)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += MAX_VERTEX_SIZE;

			vertices_[startVertexIndex + 0].position = Vec4f(startPos.x, startPos.y, startPos.z, 1.0f);
			vertices_[startVertexIndex + 0].color = startColor;
			vertices_[startVertexIndex + 1].position = Vec4f(endPos.x, endPos.y, endPos.z, 1.0f);
			vertices_[startVertexIndex + 1].color = endColor;
			return;
		}
	}

	uint32_t startVertexIndex = 0;
	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();
		startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
	}

	RenderCommand command;
	command.drawMode = DrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = MAX_VERTEX_SIZE;

	vertices_[command.startVertexIndex + 0].position = Vec4f(startPos.x, startPos.y, startPos.z, 1.0f);
	vertices_[command.startVertexIndex + 0].color = startColor;
	vertices_[command.startVertexIndex + 1].position = Vec4f(endPos.x, endPos.y, endPos.z, 1.0f);
	vertices_[command.startVertexIndex + 1].color = endColor;

	commandQueue_.push(command);
}

void DebugDrawManager3D::DrawCube(const Mat4x4& world, const Vec3f& extents, const Vec4f& color)
{
	static const uint32_t MAX_VERTEX_SIZE = 24;
	if (IsFullCommandQueue(MAX_VERTEX_SIZE))
	{
		Flush();
	}

	Vec3f minPos = -extents * 0.5f;
	Vec3f maxPos = extents * 0.5f;

	auto composeVertexData = [&](uint32_t startVertexIndex)
		{
			vertices_[startVertexIndex + 0] = Vertex{ Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color };
			vertices_[startVertexIndex + 1] = Vertex{ Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color };

			vertices_[startVertexIndex + 2] = Vertex{ Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color };
			vertices_[startVertexIndex + 3] = Vertex{ Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color };

			vertices_[startVertexIndex + 4] = Vertex{ Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color };
			vertices_[startVertexIndex + 5] = Vertex{ Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color };

			vertices_[startVertexIndex + 6] = Vertex{ Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color };
			vertices_[startVertexIndex + 7] = Vertex{ Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color };

			vertices_[startVertexIndex + 8] = Vertex{ Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color };
			vertices_[startVertexIndex + 9] = Vertex{ Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color };

			vertices_[startVertexIndex + 10] = Vertex{ Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color };
			vertices_[startVertexIndex + 11] = Vertex{ Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color };

			vertices_[startVertexIndex + 12] = Vertex{ Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color };
			vertices_[startVertexIndex + 13] = Vertex{ Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color };

			vertices_[startVertexIndex + 14] = Vertex{ Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color };
			vertices_[startVertexIndex + 15] = Vertex{ Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color };

			vertices_[startVertexIndex + 16] = Vertex{ Vec4f(maxPos.x, maxPos.y, maxPos.z, 1.0f), color };
			vertices_[startVertexIndex + 17] = Vertex{ Vec4f(maxPos.x, minPos.y, maxPos.z, 1.0f), color };

			vertices_[startVertexIndex + 18] = Vertex{ Vec4f(minPos.x, maxPos.y, maxPos.z, 1.0f), color };
			vertices_[startVertexIndex + 19] = Vertex{ Vec4f(minPos.x, minPos.y, maxPos.z, 1.0f), color };

			vertices_[startVertexIndex + 20] = Vertex{ Vec4f(minPos.x, maxPos.y, minPos.z, 1.0f), color };
			vertices_[startVertexIndex + 21] = Vertex{ Vec4f(minPos.x, minPos.y, minPos.z, 1.0f), color };

			vertices_[startVertexIndex + 22] = Vertex{ Vec4f(maxPos.x, maxPos.y, minPos.z, 1.0f), color };
			vertices_[startVertexIndex + 23] = Vertex{ Vec4f(maxPos.x, minPos.y, minPos.z, 1.0f), color };
		};

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == DrawMode::LINES)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += MAX_VERTEX_SIZE;

			composeVertexData(startVertexIndex);
			return;
		}
	}

	uint32_t startVertexIndex = 0;
	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();
		startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
	}

	RenderCommand command;
	command.drawMode = DrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = MAX_VERTEX_SIZE;

	composeVertexData(startVertexIndex);
	
	commandQueue_.push(command);
}

void DebugDrawManager3D::DrawViewfrustum(const Mat4x4& view, const Mat4x4& projection, const Vec4f& color)
{
	static const uint32_t MAX_VERTEX_SIZE = 24;
	if (IsFullCommandQueue(MAX_VERTEX_SIZE))
	{
		Flush();
	}

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

	auto composeVertexData = [&](uint32_t startVertexIndex)
		{
			vertices_[startVertexIndex + 0] = Vertex{ Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color };
			vertices_[startVertexIndex + 1] = Vertex{ Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color };

			vertices_[startVertexIndex + 2] = Vertex{ Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color };
			vertices_[startVertexIndex + 3] = Vertex{ Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color };

			vertices_[startVertexIndex + 4] = Vertex{ Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color };
			vertices_[startVertexIndex + 5] = Vertex{ Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color };

			vertices_[startVertexIndex + 4] = Vertex{ Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color };
			vertices_[startVertexIndex + 5] = Vertex{ Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color };

			vertices_[startVertexIndex + 6] = Vertex{ Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color };
			vertices_[startVertexIndex + 7] = Vertex{ Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color };

			vertices_[startVertexIndex + 8] = Vertex{ Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color };
			vertices_[startVertexIndex + 9] = Vertex{ Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color };

			vertices_[startVertexIndex + 10] = Vertex{ Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color };
			vertices_[startVertexIndex + 11] = Vertex{ Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color };

			vertices_[startVertexIndex + 12] = Vertex{ Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color };
			vertices_[startVertexIndex + 13] = Vertex{ Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color };

			vertices_[startVertexIndex + 14] = Vertex{ Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color };
			vertices_[startVertexIndex + 15] = Vertex{ Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color };

			vertices_[startVertexIndex + 16] = Vertex{ Vec4f(frustumCorners[0].x, frustumCorners[0].y, frustumCorners[0].z, 1.0f), color };
			vertices_[startVertexIndex + 17] = Vertex{ Vec4f(frustumCorners[4].x, frustumCorners[4].y, frustumCorners[4].z, 1.0f), color };

			vertices_[startVertexIndex + 18] = Vertex{ Vec4f(frustumCorners[1].x, frustumCorners[1].y, frustumCorners[1].z, 1.0f), color };
			vertices_[startVertexIndex + 19] = Vertex{ Vec4f(frustumCorners[5].x, frustumCorners[5].y, frustumCorners[5].z, 1.0f), color };

			vertices_[startVertexIndex + 20] = Vertex{ Vec4f(frustumCorners[2].x, frustumCorners[2].y, frustumCorners[2].z, 1.0f), color };
			vertices_[startVertexIndex + 21] = Vertex{ Vec4f(frustumCorners[6].x, frustumCorners[6].y, frustumCorners[6].z, 1.0f), color };

			vertices_[startVertexIndex + 22] = Vertex{ Vec4f(frustumCorners[3].x, frustumCorners[3].y, frustumCorners[3].z, 1.0f), color };
			vertices_[startVertexIndex + 23] = Vertex{ Vec4f(frustumCorners[7].x, frustumCorners[7].y, frustumCorners[7].z, 1.0f), color };
		};

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == DrawMode::LINES)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += MAX_VERTEX_SIZE;

			composeVertexData(startVertexIndex);
			return;
		}
	}

	uint32_t startVertexIndex = 0;
	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();
		startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
	}

	RenderCommand command;
	command.drawMode = DrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = MAX_VERTEX_SIZE;

	composeVertexData(startVertexIndex);

	commandQueue_.push(command);
}

void DebugDrawManager3D::Flush()
{
	if (commandQueue_.empty()) /** Command Queue가 비어있으면 동작X */
	{
		return;
	}

	const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
	uint32_t bufferByteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	vertexBuffer_->SetBufferData(vertexPtr, bufferByteSize);
	
	GL_CHECK(glBindVertexArray(vertexArrayObject_));
	{
		shader_->Bind();
		while (!commandQueue_.empty())
		{
			RenderCommand command = commandQueue_.front();
			commandQueue_.pop();

			GL_CHECK(glDrawArrays(static_cast<GLenum>(command.drawMode), command.startVertexIndex, command.vertexCount));
		}
		shader_->Unbind();
	}
	GL_CHECK(glBindVertexArray(0));
}

bool DebugDrawManager3D::IsFullCommandQueue(uint32_t vertexCount)
{
	if (commandQueue_.empty())
	{
		return false;
	}

	const RenderCommand& command = commandQueue_.back();
	uint32_t index = command.startVertexIndex + command.vertexCount + vertexCount;

	return index >= MAX_VERTEX_BUFFER_SIZE;
}

#pragma warning(pop)