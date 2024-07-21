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
				command.texture->Active(0);
				break;

			case Renderer2D::EType::STRING:
				GL_FAILED(glActiveTexture(GL_TEXTURE0));
				GL_FAILED(glBindTexture(GL_TEXTURE_2D, command.font->GetGlyphAtlasID()));
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
	renderManagerPtr->SetCullFaceMode(static_cast<bool>(originEnableCull));
	renderManagerPtr->SetDepthMode(static_cast<bool>(originEnableDepth));

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

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::TRIANGLES;
		command.startVertexIndex = 0;
		command.vertexCount = static_cast<uint32_t>(vertices.size());
		command.type = EType::GEOMETRY;
		command.texture = nullptr;
		command.font = nullptr;

		for (uint32_t index = 0; index < command.vertexCount; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices[index];
			vertices_[command.startVertexIndex + index].color = color;
		}

		commandQueue_.push(command);
	}
	else
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
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::TRIANGLES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = static_cast<uint32_t>(vertices.size());
			command.type = EType::GEOMETRY;
			command.texture = nullptr;
			command.font = nullptr;

			for (uint32_t index = 0; index < command.vertexCount; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices[index];
				vertices_[command.startVertexIndex + index].color = color;
			}

			commandQueue_.push(command);
		}
	}
}

void Renderer2D::DrawLine(const Vec2f& startPos, const Vec2f& endPos, const Vec4f& color)
{
	std::array<Vec2f, 2> vertices = 
	{ 
		startPos + Vec2f(0.375f, 0.375f), 
		endPos + Vec2f(0.375f, 0.375f), 
	};

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::LINES;
		command.startVertexIndex = 0;
		command.vertexCount = static_cast<uint32_t>(vertices.size());
		command.type = EType::GEOMETRY;
		command.texture = nullptr;
		command.font = nullptr;

		for (uint32_t index = 0; index < command.vertexCount; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices[index];
			vertices_[command.startVertexIndex + index].color = color;
		}

		commandQueue_.push(command);
	}
	else
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
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::LINES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = static_cast<uint32_t>(vertices.size());
			command.type = EType::GEOMETRY;
			command.texture = nullptr;
			command.font = nullptr;

			for (uint32_t index = 0; index < command.vertexCount; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices[index];
				vertices_[command.startVertexIndex + index].color = color;
			}

			commandQueue_.push(command);
		}
	}
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

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::LINES;
		command.startVertexIndex = 0;
		command.vertexCount = static_cast<uint32_t>(vertices.size());
		command.type = EType::GEOMETRY;
		command.texture = nullptr;
		command.font = nullptr;

		for (uint32_t index = 0; index < command.vertexCount; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices[index];
			vertices_[command.startVertexIndex + index].color = colors[index];
		}

		commandQueue_.push(command);
	}
	else
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
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::LINES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = static_cast<uint32_t>(vertices.size());
			command.type = EType::GEOMETRY;
			command.texture = nullptr;
			command.font = nullptr;

			for (uint32_t index = 0; index < command.vertexCount; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices[index];
				vertices_[command.startVertexIndex + index].color = colors[index];
			}

			commandQueue_.push(command);
		}
	}
}

void Renderer2D::DrawTriangle(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color)
{
	std::array<Vec2f, 3> vertices =
	{
		fromPos + Vec2f(0.375f, 0.375f),
		  byPos + Vec2f(0.375f, 0.375f),
		  toPos + Vec2f(0.375f, 0.375f),
	};

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::TRIANGLES;
		command.startVertexIndex = 0;
		command.vertexCount = static_cast<uint32_t>(vertices.size());
		command.type = EType::GEOMETRY;
		command.texture = nullptr;
		command.font = nullptr;

		for (uint32_t index = 0; index < command.vertexCount; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices[index];
			vertices_[command.startVertexIndex + index].color = color;
		}

		commandQueue_.push(command);
	}
	else
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
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::TRIANGLES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = static_cast<uint32_t>(vertices.size());
			command.type = EType::GEOMETRY;
			command.texture = nullptr;
			command.font = nullptr;

			for (uint32_t index = 0; index < command.vertexCount; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices[index];
				vertices_[command.startVertexIndex + index].color = color;
			}

			commandQueue_.push(command);
		}
	}
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

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::TRIANGLES;
		command.startVertexIndex = 0;
		command.vertexCount = static_cast<uint32_t>(vertices.size());
		command.type = EType::GEOMETRY;
		command.texture = nullptr;
		command.font = nullptr;

		for (uint32_t index = 0; index < command.vertexCount; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices[index];
			vertices_[command.startVertexIndex + index].color = colors[index];
		}

		commandQueue_.push(command);
	}
	else
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
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::TRIANGLES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = static_cast<uint32_t>(vertices.size());
			command.type = EType::GEOMETRY;
			command.texture = nullptr;
			command.font = nullptr;

			for (uint32_t index = 0; index < command.vertexCount; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices[index];
				vertices_[command.startVertexIndex + index].color = colors[index];
			}

			commandQueue_.push(command);
		}
	}
}

void Renderer2D::DrawTriangleWireframe(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color)
{
	std::array<Vec2f, 6> vertices =
	{
		fromPos + Vec2f(0.375f, 0.375f),   byPos + Vec2f(0.375f, 0.375f),
		byPos   + Vec2f(0.375f, 0.375f),   toPos + Vec2f(0.375f, 0.375f),
		toPos   + Vec2f(0.375f, 0.375f), fromPos + Vec2f(0.375f, 0.375f),
	};

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::LINES;
		command.startVertexIndex = 0;
		command.vertexCount = static_cast<uint32_t>(vertices.size());
		command.type = EType::GEOMETRY;
		command.texture = nullptr;
		command.font = nullptr;

		for (uint32_t index = 0; index < command.vertexCount; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices[index];
			vertices_[command.startVertexIndex + index].color = color;
		}

		commandQueue_.push(command);
	}
	else
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
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::LINES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = static_cast<uint32_t>(vertices.size());
			command.type = EType::GEOMETRY;
			command.texture = nullptr;
			command.font = nullptr;

			for (uint32_t index = 0; index < command.vertexCount; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices[index];
				vertices_[command.startVertexIndex + index].color = color;
			}

			commandQueue_.push(command);
		}
	}
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

	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::LINES;
		command.startVertexIndex = 0;
		command.vertexCount = static_cast<uint32_t>(vertices.size());
		command.type = EType::GEOMETRY;
		command.texture = nullptr;
		command.font = nullptr;

		for (uint32_t index = 0; index < command.vertexCount; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices[index];
			vertices_[command.startVertexIndex + index].color = colors[index];
		}

		commandQueue_.push(command);
	}
	else
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
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::LINES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = static_cast<uint32_t>(vertices.size());
			command.type = EType::GEOMETRY;
			command.texture = nullptr;
			command.font = nullptr;

			for (uint32_t index = 0; index < command.vertexCount; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices[index];
				vertices_[command.startVertexIndex + index].color = colors[index];
			}

			commandQueue_.push(command);
		}
	}
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
	
	if (commandQueue_.empty())
	{
		RenderCommand command;
		command.drawMode = EDrawMode::TRIANGLES;
		command.startVertexIndex = 0;
		command.vertexCount = static_cast<uint32_t>(vertices.size());
		command.type = EType::GEOMETRY;
		command.texture = nullptr;
		command.font = nullptr;

		for (uint32_t index = 0; index < command.vertexCount; ++index)
		{
			vertices_[command.startVertexIndex + index].position = vertices[index];
			vertices_[command.startVertexIndex + index].color = color;
		}

		commandQueue_.push(command);
	}
	else
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
		}
		else
		{
			RenderCommand command;
			command.drawMode = EDrawMode::TRIANGLES;
			command.startVertexIndex = prevCommand.startVertexIndex + prevCommand.vertexCount;
			command.vertexCount = static_cast<uint32_t>(vertices.size());
			command.type = EType::GEOMETRY;
			command.texture = nullptr;
			command.font = nullptr;

			for (uint32_t index = 0; index < command.vertexCount; ++index)
			{
				vertices_[command.startVertexIndex + index].position = vertices[index];
				vertices_[command.startVertexIndex + index].color = color;
			}

			commandQueue_.push(command);
		}
	}
}

#pragma warning(pop)