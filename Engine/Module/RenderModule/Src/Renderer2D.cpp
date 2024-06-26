#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)

#include <glad/glad.h>

#include "GLAssertion.h"
#include "Renderer2D.h"
#include "Shader.h"

Renderer2D::Renderer2D()
{
	std::string vsPath = "Resource/Shader/Renderer2D.vert";
	std::string fsPath = "Resource/Shader/Renderer2D.frag";
	shader_ = RenderModule::CreateResource<Shader>(vsPath, fsPath);

	uint32_t byteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	VertexBuffer::EUsage usage = VertexBuffer::EUsage::DYNAMIC;
	vertexBuffer_ = RenderModule::CreateResource<VertexBuffer>(byteSize, usage);

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

	if (shader_)
	{
		RenderModule::DestroyResource(shader_);
		shader_ = nullptr;
	}

	if (vertexBuffer_)
	{
		RenderModule::DestroyResource(vertexBuffer_);
		vertexBuffer_ = nullptr;
	}

	GL_FAILED(glDeleteVertexArrays(1, &vertexArrayObject_));

	bIsInitialized_ = false;
}

void Renderer2D::DrawPoint(const Vec2f* positions, const Vec4f* colors, uint32_t size, float pointSize)
{
	CHECK(size <= MAX_VERTEX_SIZE);
	CHECK(pointSize >= 0.0f);

	for (uint32_t index = 0; index < size; ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.375f, positions[index].y + 0.375f);
		vertices_[index].color = colors[index];
	}

	pointSize_ = pointSize;
	Draw(Mat4x4::Identity(), EDrawMode::POINTS, size);
}

void Renderer2D::DrawLine(const Vec2f* positions, const Vec4f* colors, uint32_t size)
{
	CHECK(size <= MAX_VERTEX_SIZE);

	for (uint32_t index = 0; index < size; ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.375f, positions[index].y + 0.375f);
		vertices_[index].color = colors[index];
	}

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, size);
}

void Renderer2D::DrawLine(const Vec2f& startPosition, const Vec2f& endPosition, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = Vec2f(startPosition.x + 0.375f, startPosition.y + 0.375f);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = Vec2f(endPosition.x + 0.375f, endPosition.y + 0.375f);
	vertices_[vertexCount++].color = color;
	
	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer2D::DrawLine(const Vec2f& startPosition, const Vec4f& startColor, const Vec2f& endPosition, const Vec4f& endColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = Vec2f(startPosition.x + 0.375f, startPosition.y + 0.375f);
	vertices_[vertexCount++].color = startColor;

	vertices_[vertexCount].position = Vec2f(endPosition.x + 0.375f, endPosition.y + 0.375f);
	vertices_[vertexCount++].color = endColor;

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer2D::Draw(const Mat4x4& transform, const EDrawMode& drawMode, uint32_t vertexCount)
{
	CHECK(drawMode != EDrawMode::NONE);

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	RenderModule::SetDepthMode(false);
	{
		shader_->Bind();

		const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
		uint32_t bufferByteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
		vertexBuffer_->SetBufferData(vertexPtr, bufferByteSize);

		shader_->SetUniform("transform", transform);
		shader_->SetUniform("ortho", ortho_);

		if (drawMode == EDrawMode::POINTS)
		{
			shader_->SetUniform("pointSize", pointSize_);
		}

		GL_FAILED(glBindVertexArray(vertexArrayObject_));
		RenderModule::ExecuteDrawVertex(vertexCount, drawMode);
		GL_FAILED(glBindVertexArray(0));

		shader_->Unbind();
	}
	RenderModule::SetDepthMode(originEnableDepth);
}

#pragma warning(pop)