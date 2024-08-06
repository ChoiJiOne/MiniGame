#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)
#pragma warning(disable: 6031)

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

		GL_FAILED(glVertexAttribIPointer(3, 1, GL_INT, Vertex::GetStride(), (void*)(offsetof(Vertex, unit))));
		GL_FAILED(glEnableVertexAttribArray(3));

		vertexBuffer_->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));

	Shader* geometry2D = resourceManagerPtr->Create<Shader>("GameMaker/Shader/Geometry2D.vert", "GameMaker/Shader/Geometry2D.frag");
	Shader* sprite2D = resourceManagerPtr->Create<Shader>("GameMaker/Shader/Sprite2D.vert", "GameMaker/Shader/Sprite2D.frag");
	Shader* string2D = resourceManagerPtr->Create<Shader>("GameMaker/Shader/String2D.vert", "GameMaker/Shader/String2D.frag");

	shaders_.insert({ EType::GEOMETRY, geometry2D });
	shaders_.insert({ EType::SPRITE,   sprite2D   });
	shaders_.insert({ EType::STRING,   string2D   });

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

	for (auto& shader : shaders_)
	{
		resourceManagerPtr->Destroy(shader.second);
	}

	GL_FAILED(glDeleteVertexArrays(1, &vertexArrayObject_));

	bIsInitialized_ = false;
}

void Renderer2D::Begin(const Camera2D* camera2D)
{
	CHECK(!bIsBegin_);

	Mat4x4 ortho;
	if (!camera2D)
	{
		float w = 0.0f;
		float h = 0.0f;
		static const float nearZ = -1.0f;
		static const float farZ = 1.0f;
		renderManagerPtr->GetScreenSize<float>(w, h);

		ortho = Mat4x4::Ortho(-w * 0.5f, +w * 0.5f, -h * 0.5f, +h * 0.5f, nearZ, farZ);
	}
	else
	{
		ortho = camera2D->GetOrtho();
	}

	for (auto& shader : shaders_)
	{
		Shader* shaderPtr = shader.second;

		shaderPtr->Bind();
		{
			shaderPtr->SetUniform("ortho", ortho);
		}
		shaderPtr->Unbind();
	}

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	GLboolean originEnableCull;
	GL_FAILED(glGetBooleanv(GL_CULL_FACE, &originEnableCull));

	originEnableDepth_ = static_cast<bool>(originEnableDepth);
	originEnableCull_ = static_cast<bool>(originEnableCull);

	renderManagerPtr->SetDepthMode(false);
	renderManagerPtr->SetCullFaceMode(false);

	bIsBegin_ = true;
}

void Renderer2D::End()
{
	CHECK(bIsBegin_);

	Flush();
	
	renderManagerPtr->SetCullFaceMode(originEnableCull_);
	renderManagerPtr->SetDepthMode(originEnableDepth_);

	bIsBegin_ = false;
}
void Renderer2D::DrawPoint(const Vec2f& point, const Vec4f& color, float pointSize)
{
	float w = pointSize * 0.5f;
	std::array<Vec2f, 6> vertices =
	{
		Vec2f(-w, -w),
		Vec2f(+w, +w),
		Vec2f(-w, +w),
		Vec2f(-w, -w),
		Vec2f(+w, -w),
		Vec2f(+w, +w),
	};

	for (auto& vertex : vertices)
	{
		vertex += (point + Vec2f(0.375f, 0.375f));
	}

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();
		if (prevCommand.drawMode == EDrawMode::TRIANGLES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = color;
			}

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
	command.drawMode = EDrawMode::TRIANGLES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = color;
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawLine(const Vec2f& startPos, const Vec2f& endPos, const Vec4f& color)
{
	std::array<Vec2f, 2> vertices = 
	{ 
		startPos + Vec2f(0.375f, 0.375f), 
		endPos + Vec2f(0.375f, 0.375f), 
	};

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = color;
			}

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
	command.drawMode = EDrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = color;
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawLine(const Vec2f& startPos, const Vec4f& startColor, const Vec2f& endPos, const Vec4f& endColor)
{
	std::array<Vec2f, 2> vertices = 
	{ 
		startPos + Vec2f(0.375f, 0.375f), 
		endPos + Vec2f(0.375f, 0.375f), 
	};

	std::array<Vec4f, 2> colors =
	{
		startColor,
		endColor,
	};

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = colors[index];
			}

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
	command.drawMode = EDrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = colors[index];
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawTriangle(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color)
{
	std::array<Vec2f, 3> vertices =
	{
		fromPos + Vec2f(0.375f, 0.375f),
		  byPos + Vec2f(0.375f, 0.375f),
		  toPos + Vec2f(0.375f, 0.375f),
	};

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::TRIANGLES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = color;
			}

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
	command.drawMode = EDrawMode::TRIANGLES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = color;
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawTriangle(const Vec2f& fromPos, const Vec4f& fromColor, const Vec2f& byPos, const Vec4f& byColor, const Vec2f& toPos, const Vec4f& toColor)
{
	std::array<Vec2f, 3> vertices =
	{
		fromPos + Vec2f(0.375f, 0.375f),
		  byPos + Vec2f(0.375f, 0.375f),
		  toPos + Vec2f(0.375f, 0.375f),
	};

	std::array<Vec4f, 3> colors =
	{
		fromColor,
		byColor,
		toColor,
	};

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::TRIANGLES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = colors[index];
			}

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
	command.drawMode = EDrawMode::TRIANGLES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = colors[index];
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawTriangleWireframe(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color)
{
	std::array<Vec2f, 6> vertices =
	{
		fromPos + Vec2f(0.375f, 0.375f),   byPos + Vec2f(0.375f, 0.375f),
		byPos   + Vec2f(0.375f, 0.375f),   toPos + Vec2f(0.375f, 0.375f),
		toPos   + Vec2f(0.375f, 0.375f), fromPos + Vec2f(0.375f, 0.375f),
	};

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = color;
			}

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
	command.drawMode = EDrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = color;
	}

	commandQueue_.push(command);
}

void GameMaker::Renderer2D::DrawTriangleWireframe(const Vec2f& fromPos, const Vec4f& fromColor, const Vec2f& byPos, const Vec4f& byColor, const Vec2f& toPos, const Vec4f& toColor)
{
	std::array<Vec2f, 6> vertices =
	{
		fromPos + Vec2f(0.375f, 0.375f),   byPos + Vec2f(0.375f, 0.375f),
		byPos + Vec2f(0.375f, 0.375f),   toPos + Vec2f(0.375f, 0.375f),
		toPos + Vec2f(0.375f, 0.375f), fromPos + Vec2f(0.375f, 0.375f),
	};

	std::array<Vec4f, 6> colors =
	{
		fromColor, byColor,
		byColor,   toColor,
		toColor,   fromColor,
	};

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = colors[index];
			}

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
	command.drawMode = EDrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = colors[index];
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawRect(const Vec2f& center, float w, float h, const Vec4f& color, float rotate)
{
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;
	
	std::array<Vec2f, 6> vertices =
	{
		Vec2f(-w2, -h2),
		Vec2f(+w2, +h2),
		Vec2f(-w2, +h2),
		Vec2f(-w2, -h2),
		Vec2f(+w2, -h2),
		Vec2f(+w2, +h2),
	};
	
	Mat2x2 rotateMat = Mat2x2(Cos(rotate), -Sin(rotate), Sin(rotate), Cos(rotate));
	for (auto& vertex : vertices)
	{
		vertex = rotateMat * vertex;
		vertex += (center + Vec2f(0.375f, 0.375f));
	}

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::TRIANGLES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = color;
			}

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
	command.drawMode = EDrawMode::TRIANGLES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = color;
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawRectWireframe(const Vec2f& center, float w, float h, const Vec4f& color, float rotate)
{
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;

	std::array<Vec2f, 8> vertices =
	{
		Vec2f(-w2, -h2), Vec2f(+w2, -h2),
		Vec2f(+w2, -h2), Vec2f(+w2, +h2),
		Vec2f(+w2, +h2), Vec2f(-w2, +h2),
		Vec2f(-w2, +h2), Vec2f(-w2, -h2),
	};

	Mat2x2 rotateMat = Mat2x2(Cos(rotate), -Sin(rotate), Sin(rotate), Cos(rotate));
	for (auto& vertex : vertices)
	{
		vertex = rotateMat * vertex;
		vertex += (center + Vec2f(0.375f, 0.375f));
	}
	
	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = color;
			}

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
	command.drawMode = EDrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = color;
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawRoundRect(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate)
{
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;
	side = Min<float>(side, Min<float>(h2, h2));

	uint32_t vertexCount = 0;
	
	static const uint32_t MAX_VERTEX_SIZE = 252;
	static const uint32_t MAX_SLICE_SIZE = 20;
	std::array<Vec2f, MAX_VERTEX_SIZE> vertices;

	auto calculateBezierCurve = [&](const Vec2f& startPos, const Vec2f& endPos, const Vec2f& controlPos, uint32_t sliceCount)
		{
			for (int32_t slice = 0; slice < sliceCount; ++slice)
			{
				float t0 = static_cast<float>(slice + 0) / static_cast<float>(sliceCount);
				float t1 = static_cast<float>(slice + 1) / static_cast<float>(sliceCount);
				
				vertices[vertexCount + 0] = Vec2f();
				vertices[vertexCount + 1] = Vec2f::Bezier(startPos, endPos, controlPos, t0);
				vertices[vertexCount + 2] = Vec2f::Bezier(startPos, endPos, controlPos, t1);

				vertexCount += 3;
			}
		};

	Vec2f control = Vec2f(-w2, +h2);
	Vec2f start = control + Vec2f(+side, 0.0f);
	Vec2f end = control + Vec2f(0.0f, -side);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	vertices[vertexCount + 0] = Vec2f();
	vertices[vertexCount + 1] = Vec2f(-w2, +h2) + Vec2f(0.0f, -side);
	vertices[vertexCount + 2] = Vec2f(-w2, -h2) + Vec2f(0.0f, +side);
	vertexCount += 3;

	control = Vec2f(-w2, -h2);
	start = control + Vec2f(0.0f, +side);
	end = control + Vec2f(+side, 0.0f);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	vertices[vertexCount + 0] = Vec2f();
	vertices[vertexCount + 1] = Vec2f(-w2, -h2) + Vec2f(+side, 0.0f);
	vertices[vertexCount + 2] = Vec2f(+w2, -h2) + Vec2f(-side, 0.0f);
	vertexCount += 3;

	control = Vec2f(+w2, -h2);
	start = control + Vec2f(-side, 0.0f);
	end = control + Vec2f(0.0f, +side);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	vertices[vertexCount + 0] = Vec2f();
	vertices[vertexCount + 1] = Vec2f(+w2, -h2) + Vec2f(0.0f, +side);
	vertices[vertexCount + 2] = Vec2f(+w2, +h2) + Vec2f(0.0f, -side);
	vertexCount += 3;

	control = Vec2f(+w2, +h2);
	start = control + Vec2f(0.0f, -side);
	end = control + Vec2f(-side, 0.0f);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	vertices[vertexCount + 0] = Vec2f();
	vertices[vertexCount + 1] = Vec2f(+w2, +h2) + Vec2f(-side, 0.0f);
	vertices[vertexCount + 2] = Vec2f(-w2, +h2) + Vec2f(+side, 0.0f);
	vertexCount += 3;

	Mat2x2 rotateMat = Mat2x2(Cos(rotate), -Sin(rotate), Sin(rotate), Cos(rotate));
	for (uint32_t index = 0; index < vertexCount; index += 3)
	{
		vertices[index + 1] = rotateMat * vertices[index + 1];
		vertices[index + 2] = rotateMat * vertices[index + 2];

		vertices[index + 0] += (center + Vec2f(0.375f, 0.375f));
		vertices[index + 1] += (center + Vec2f(0.375f, 0.375f));
		vertices[index + 2] += (center + Vec2f(0.375f, 0.375f));
	}

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::TRIANGLES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = color;
			}

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
	command.drawMode = EDrawMode::TRIANGLES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = color;
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawRoundRectWireframe(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate)
{
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;
	side = Min<float>(side, Min<float>(h2, h2));

	uint32_t vertexCount = 0;

	static const uint32_t MAX_VERTEX_SIZE = 168;
	static const uint32_t MAX_SLICE_SIZE = 20;
	std::array<Vec2f, MAX_VERTEX_SIZE> vertices;

	auto calculateBezierCurve = [&](const Vec2f& startPos, const Vec2f& endPos, const Vec2f& controlPos, uint32_t sliceCount)
		{
			for (int32_t slice = 0; slice < sliceCount; ++slice)
			{
				float t0 = static_cast<float>(slice + 0) / static_cast<float>(sliceCount);
				float t1 = static_cast<float>(slice + 1) / static_cast<float>(sliceCount);

				vertices[vertexCount + 0] = Vec2f::Bezier(startPos, endPos, controlPos, t0);
				vertices[vertexCount + 1] = Vec2f::Bezier(startPos, endPos, controlPos, t1);
				vertexCount += 2;
			}
		};

	Vec2f control = Vec2f(-w2, +h2);
	Vec2f start = control + Vec2f(+side, 0.0f);
	Vec2f end = control + Vec2f(0.0f, -side);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	vertices[vertexCount + 0] = Vec2f(-w2, +h2) + Vec2f(0.0f, -side);
	vertices[vertexCount + 1] = Vec2f(-w2, -h2) + Vec2f(0.0f, +side);
	vertexCount += 2;

	control = Vec2f(-w2, -h2);
	start = control + Vec2f(0.0f, +side);
	end = control + Vec2f(+side, 0.0f);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	vertices[vertexCount + 0] = Vec2f(-w2, -h2) + Vec2f(+side, 0.0f);
	vertices[vertexCount + 1] = Vec2f(+w2, -h2) + Vec2f(-side, 0.0f);
	vertexCount += 2;

	control = Vec2f(+w2, -h2);
	start = control + Vec2f(-side, 0.0f);
	end = control + Vec2f(0.0f, +side);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	vertices[vertexCount + 0] = Vec2f(+w2, -h2) + Vec2f(0.0f, +side);
	vertices[vertexCount + 1] = Vec2f(+w2, +h2) + Vec2f(0.0f, -side);
	vertexCount += 2;

	control = Vec2f(+w2, +h2);
	start = control + Vec2f(0.0f, -side);
	end = control + Vec2f(-side, 0.0f);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	vertices[vertexCount + 0] = Vec2f(+w2, +h2) + Vec2f(-side, 0.0f);
	vertices[vertexCount + 1] = Vec2f(-w2, +h2) + Vec2f(+side, 0.0f);
	vertexCount += 2;

	Mat2x2 rotateMat = Mat2x2(Cos(rotate), -Sin(rotate), Sin(rotate), Cos(rotate));
	for (uint32_t index = 0; index < vertexCount; index += 2)
	{
		vertices[index + 0] = rotateMat * vertices[index + 0];
		vertices[index + 1] = rotateMat * vertices[index + 1];

		vertices[index + 0] += (center + Vec2f(0.375f, 0.375f));
		vertices[index + 1] += (center + Vec2f(0.375f, 0.375f));
	}

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = color;
			}

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
	command.drawMode = EDrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = color;
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawCircle(const Vec2f& center, float radius, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	static const uint32_t MAX_VERTEX_SIZE = 300;
	static const uint32_t MAX_SLICE_SIZE = 100;
	std::array<Vec2f, MAX_VERTEX_SIZE> vertices;

	for (int32_t slice = 0; slice < MAX_SLICE_SIZE; ++slice)
	{
		float radian0 = (static_cast<float>(slice + 0) * TWO_PI) / static_cast<float>(MAX_SLICE_SIZE);
		float radian1 = (static_cast<float>(slice + 1) * TWO_PI) / static_cast<float>(MAX_SLICE_SIZE);

		vertices[vertexCount + 0] = center + Vec2f(0.375f, 0.375f);
		vertices[vertexCount + 1] = center + Vec2f(radius * Cos(radian0), radius * Sin(radian0)) + Vec2f(0.375f, 0.375f);
		vertices[vertexCount + 2] = center + Vec2f(radius * Cos(radian1), radius * Sin(radian1)) + Vec2f(0.375f, 0.375f);
		
		vertexCount += 3;
	}

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::TRIANGLES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = color;
			}

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
	command.drawMode = EDrawMode::TRIANGLES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = color;
	}

	commandQueue_.push(command);
}

void GameMaker::Renderer2D::DrawCircleWireframe(const Vec2f& center, float radius, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	static const uint32_t MAX_VERTEX_SIZE = 200;
	static const uint32_t MAX_SLICE_SIZE = 100;
	std::array<Vec2f, MAX_VERTEX_SIZE> vertices;

	for (int32_t slice = 0; slice < MAX_SLICE_SIZE; ++slice)
	{
		float radian0 = (static_cast<float>(slice + 0) * TWO_PI) / static_cast<float>(MAX_SLICE_SIZE);
		float radian1 = (static_cast<float>(slice + 1) * TWO_PI) / static_cast<float>(MAX_SLICE_SIZE);

		vertices[vertexCount + 0] = center + Vec2f(radius * Cos(radian0), radius * Sin(radian0)) + Vec2f(0.375f, 0.375f);
		vertices[vertexCount + 1] = center + Vec2f(radius * Cos(radian1), radius * Sin(radian1)) + Vec2f(0.375f, 0.375f);

		vertexCount += 2;
	}

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::LINES && prevCommand.type == EType::GEOMETRY)
		{
			uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices_[startVertexIndex + index].position = vertices[index];
				vertices_[startVertexIndex + index].color = color;
			}

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
	command.drawMode = EDrawMode::LINES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::GEOMETRY;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].color = color;
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawString(TTFont* font, const std::wstring& text, const Vec2f& pos, const Vec4f& color)
{
	/** 문자 하나당 정점 6개. */
	uint32_t vertexCount = 6 * static_cast<uint32_t>(text.size());

	float w = 0.0f;
	float h = 0.0f;
	font->MeasureText(text, w, h);
	
	float atlasSize = static_cast<float>(font->GetAtlasSize());
	Vec2f currPos = Vec2f(pos.x, pos.y - h);

	auto composeVertexData = [&](uint32_t vertexIndex, uint32_t unit)
		{
			for (const auto& unicode : text)
			{
				const Glyph& glyph = font->GetGlyph(static_cast<int32_t>(unicode));

				float uw = static_cast<float>(glyph.pos1.x - glyph.pos0.x);
				float uh = static_cast<float>(glyph.pos1.y - glyph.pos0.y);

				vertices_[vertexIndex + 0].position = Vec2f(currPos.x, currPos.y - glyph.yoff);
				vertices_[vertexIndex + 0].uv = Vec2f(static_cast<float>(glyph.pos0.x) / atlasSize, static_cast<float>(glyph.pos0.y) / atlasSize);
				vertices_[vertexIndex + 0].color = color;
				vertices_[vertexIndex + 0].unit = unit;
				
				vertices_[vertexIndex + 1].position = Vec2f(currPos.x, currPos.y - uh - glyph.yoff);
				vertices_[vertexIndex + 1].uv = Vec2f(static_cast<float>(glyph.pos0.x) / atlasSize, static_cast<float>(glyph.pos1.y) / atlasSize);
				vertices_[vertexIndex + 1].color = color;
				vertices_[vertexIndex + 1].unit = unit;

				vertices_[vertexIndex + 2].position = Vec2f(currPos.x + uw, currPos.y - glyph.yoff);
				vertices_[vertexIndex + 2].uv = Vec2f(static_cast<float>(glyph.pos1.x) / atlasSize, static_cast<float>(glyph.pos0.y) / atlasSize);
				vertices_[vertexIndex + 2].color = color;
				vertices_[vertexIndex + 2].unit = unit;

				vertices_[vertexIndex + 3].position = Vec2f(currPos.x + uw, currPos.y - glyph.yoff);
				vertices_[vertexIndex + 3].uv = Vec2f(static_cast<float>(glyph.pos1.x) / atlasSize, static_cast<float>(glyph.pos0.y) / atlasSize);
				vertices_[vertexIndex + 3].color = color;
				vertices_[vertexIndex + 3].unit = unit;

				vertices_[vertexIndex + 4].position = Vec2f(currPos.x, currPos.y - uh - glyph.yoff);
				vertices_[vertexIndex + 4].uv = Vec2f(static_cast<float>(glyph.pos0.x) / atlasSize, static_cast<float>(glyph.pos1.y) / atlasSize);
				vertices_[vertexIndex + 4].color = color;
				vertices_[vertexIndex + 4].unit = unit;

				vertices_[vertexIndex + 5].position = Vec2f(currPos.x + uw, currPos.y - uh - glyph.yoff);
				vertices_[vertexIndex + 5].uv = Vec2f(static_cast<float>(glyph.pos1.x) / atlasSize, static_cast<float>(glyph.pos1.y) / atlasSize);
				vertices_[vertexIndex + 5].color = color;
				vertices_[vertexIndex + 5].unit = unit;

				currPos.x += glyph.xadvance;
				vertexIndex += 6;
			}
		};


	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();

		if (prevCommand.drawMode == EDrawMode::TRIANGLES && prevCommand.type == EType::STRING)
		{
			int32_t atlasUnit = -1;
			for (uint32_t unit = 0; unit < MAX_TEXTURE_UNIT; ++unit)
			{
				if (prevCommand.font[unit] == font)
				{
					atlasUnit = unit;
					break;
				}
			}

			if (atlasUnit != -1)
			{
				uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
				prevCommand.vertexCount += vertexCount;

				composeVertexData(startVertexIndex, atlasUnit);
				return;
			}

			for (uint32_t unit = 0; unit < MAX_TEXTURE_UNIT; ++unit)
			{
				if (prevCommand.font[unit] == nullptr)
				{
					atlasUnit = unit;
					break;
				}
			}

			if (atlasUnit != -1)
			{
				uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
				prevCommand.vertexCount += vertexCount;
				prevCommand.font[atlasUnit] = font;

				composeVertexData(startVertexIndex, atlasUnit);
				return;
			}
		}
	}
	
	uint32_t startVertexIndex = 0;
	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();
		startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
	}

	uint32_t atlasUnit = 0;

	RenderCommand command;
	command.drawMode = EDrawMode::TRIANGLES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = vertexCount;
	command.type = EType::STRING;
	command.font[atlasUnit] = font;

	composeVertexData(command.startVertexIndex, atlasUnit);

	commandQueue_.push(command);
}

void Renderer2D::DrawSprite(ITexture* texture, const Vec2f& center, float w, float h, float rotate, bool bFlipH, bool bFlipV)
{
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;

	std::array<Vec2f, 6> vertices =
	{
		Vec2f(-w2, -h2),
		Vec2f(+w2, +h2),
		Vec2f(-w2, +h2),
		Vec2f(-w2, -h2),
		Vec2f(+w2, -h2),
		Vec2f(+w2, +h2),
	};

	std::array<Vec2f, 6> uvs =
	{
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
	};

	Mat2x2 rotateMat = Mat2x2(Cos(rotate), -Sin(rotate), Sin(rotate), Cos(rotate));
	for (auto& vertex : vertices)
	{
		vertex = rotateMat * vertex;
		vertex += (center + Vec2f(0.375f, 0.375f));
	}

	for (auto& uv : uvs)
	{
		uv.x = bFlipV ? (1.0f - uv.x) : uv.x;
		uv.y = bFlipH ? (1.0f - uv.y) : uv.y;
	}

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();
		if (prevCommand.drawMode == EDrawMode::TRIANGLES && prevCommand.type == EType::SPRITE)
		{
			int32_t textureUnit = -1;
			for (uint32_t unit = 0; unit < MAX_TEXTURE_UNIT; ++unit)
			{
				if (prevCommand.texture[unit] == texture)
				{
					textureUnit = unit;
					break;
				}
			}

			if (textureUnit != -1)
			{
				uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
				prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

				for (uint32_t index = 0; index < vertices.size(); ++index)
				{
					vertices_[startVertexIndex + index].position = vertices[index];
					vertices_[startVertexIndex + index].uv = uvs[index];
					vertices_[startVertexIndex + index].color = Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
					vertices_[startVertexIndex + index].unit = textureUnit;
				}

				return;
			}

			for (uint32_t unit = 0; unit < MAX_TEXTURE_UNIT; ++unit)
			{
				if (prevCommand.texture[unit] == nullptr)
				{
					textureUnit = unit;
					break;
				}
			}

			if (textureUnit != -1)
			{
				uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
				prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());
				prevCommand.texture[textureUnit] = texture;

				for (uint32_t index = 0; index < vertices.size(); ++index)
				{
					vertices_[startVertexIndex + index].position = vertices[index];
					vertices_[startVertexIndex + index].uv = uvs[index];
					vertices_[startVertexIndex + index].color = Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
					vertices_[startVertexIndex + index].unit = textureUnit;
				}

				return;
			}
		}
	}
	
	uint32_t startVertexIndex = 0;
	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();
		startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
	}
	
	uint32_t textureUnit = 0;

	RenderCommand command;
	command.drawMode = EDrawMode::TRIANGLES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::SPRITE;
	command.texture[textureUnit] = texture;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].uv = uvs[index];
		vertices_[command.startVertexIndex + index].color = Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
		vertices_[command.startVertexIndex + index].unit = textureUnit;
	}

	commandQueue_.push(command);
}

void Renderer2D::DrawSprite(ITexture* texture, const Vec2f& center, float w, float h, const Vec3f& blend, float factor, float rotate, bool bFlipH, bool bFlipV)
{
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;

	std::array<Vec2f, 6> vertices =
	{
		Vec2f(-w2, -h2),
		Vec2f(+w2, +h2),
		Vec2f(-w2, +h2),
		Vec2f(-w2, -h2),
		Vec2f(+w2, -h2),
		Vec2f(+w2, +h2),
	};

	std::array<Vec2f, 6> uvs =
	{
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
	};

	Mat2x2 rotateMat = Mat2x2(Cos(rotate), -Sin(rotate), Sin(rotate), Cos(rotate));
	for (auto& vertex : vertices)
	{
		vertex = rotateMat * vertex;
		vertex += (center + Vec2f(0.375f, 0.375f));
	}

	for (auto& uv : uvs)
	{
		uv.x = bFlipV ? (1.0f - uv.x) : uv.x;
		uv.y = bFlipH ? (1.0f - uv.y) : uv.y;
	}

	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();
		if (prevCommand.drawMode == EDrawMode::TRIANGLES && prevCommand.type == EType::SPRITE)
		{
			int32_t textureUnit = -1;
			for (uint32_t unit = 0; unit < MAX_TEXTURE_UNIT; ++unit)
			{
				if (prevCommand.texture[unit] == texture)
				{
					textureUnit = unit;
					break;
				}
			}

			if (textureUnit != -1)
			{
				uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
				prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());

				for (uint32_t index = 0; index < vertices.size(); ++index)
				{
					vertices_[startVertexIndex + index].position = vertices[index];
					vertices_[startVertexIndex + index].uv = uvs[index];
					vertices_[startVertexIndex + index].color = Vec4f(blend.x, blend.y, blend.z, factor);
					vertices_[startVertexIndex + index].unit = textureUnit;
				}

				return;
			}

			for (uint32_t unit = 0; unit < MAX_TEXTURE_UNIT; ++unit)
			{
				if (prevCommand.texture[unit] == nullptr)
				{
					textureUnit = unit;
					break;
				}
			}

			if (textureUnit != -1)
			{
				uint32_t startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
				prevCommand.vertexCount += static_cast<uint32_t>(vertices.size());
				prevCommand.texture[textureUnit] = texture;

				for (uint32_t index = 0; index < vertices.size(); ++index)
				{
					vertices_[startVertexIndex + index].position = vertices[index];
					vertices_[startVertexIndex + index].uv = uvs[index];
					vertices_[startVertexIndex + index].color = Vec4f(blend.x, blend.y, blend.z, factor);
					vertices_[startVertexIndex + index].unit = textureUnit;
				}

				return;
			}
		}
	}

	uint32_t startVertexIndex = 0;
	if (!commandQueue_.empty())
	{
		RenderCommand& prevCommand = commandQueue_.back();
		startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
	}

	uint32_t textureUnit = 0;

	RenderCommand command;
	command.drawMode = EDrawMode::TRIANGLES;
	command.startVertexIndex = startVertexIndex;
	command.vertexCount = static_cast<uint32_t>(vertices.size());
	command.type = EType::SPRITE;
	command.texture[textureUnit] = texture;

	for (uint32_t index = 0; index < command.vertexCount; ++index)
	{
		vertices_[command.startVertexIndex + index].position = vertices[index];
		vertices_[command.startVertexIndex + index].uv = uvs[index];
		vertices_[command.startVertexIndex + index].color = Vec4f(blend.x, blend.y, blend.z, factor);
		vertices_[command.startVertexIndex + index].unit = textureUnit;
	}

	commandQueue_.push(command);
}

void Renderer2D::Flush()
{
	if (commandQueue_.empty()) /** Command Queue가 비어있으면 동작X */
	{
		return;
	}

	const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
	uint32_t bufferByteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	vertexBuffer_->SetBufferData(vertexPtr, bufferByteSize);

	GL_FAILED(glBindVertexArray(vertexArrayObject_));

	while (!commandQueue_.empty())
	{
		RenderCommand command = commandQueue_.front();
		commandQueue_.pop();

		switch (command.type)
		{
		case Renderer2D::EType::SPRITE:
			for (uint32_t unit = 0; unit < MAX_TEXTURE_UNIT; ++unit)
			{
				if (command.texture[unit])
				{
					command.texture[unit]->Active(unit);
				}
			}
			break;

		case Renderer2D::EType::STRING:
			for (uint32_t unit = 0; unit < MAX_TEXTURE_UNIT; ++unit)
			{
				if (command.font[unit])
				{
					command.font[unit]->Active(unit);
				}
			}
			break;
		}

		Shader* shader = shaders_.at(command.type);
		shader->Bind();
		{
			GL_FAILED(glDrawArrays(static_cast<GLenum>(command.drawMode), command.startVertexIndex, command.vertexCount));
		}
		shader->Unbind();
	}

	GL_FAILED(glBindVertexArray(0));
}

#pragma warning(pop)