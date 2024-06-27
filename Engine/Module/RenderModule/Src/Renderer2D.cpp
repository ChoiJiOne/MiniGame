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

void Renderer2D::DrawPoint(const Vec2f* positions, uint32_t size, const Vec4f& color, float pointSize)
{
	CHECK(size <= MAX_VERTEX_SIZE);
	CHECK(pointSize >= 0.0f);

	for (uint32_t index = 0; index < size; ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.375f, positions[index].y + 0.375f);
		vertices_[index].color = color;
	}

	pointSize_ = pointSize;
	Draw(Mat4x4::Identity(), EDrawMode::POINTS, size);
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

void Renderer2D::DrawLine(const Vec2f& startPos, const Vec2f& endPos, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = Vec2f(startPos.x + 0.375f, startPos.y + 0.375f);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = Vec2f(endPos.x + 0.375f, endPos.y + 0.375f);
	vertices_[vertexCount++].color = color;
	
	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer2D::DrawLine(const Vec2f& startPos, const Vec4f& startColor, const Vec2f& endPos, const Vec4f& endColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = Vec2f(startPos.x + 0.375f, startPos.y + 0.375f);
	vertices_[vertexCount++].color = startColor;

	vertices_[vertexCount].position = Vec2f(endPos.x + 0.375f, endPos.y + 0.375f);
	vertices_[vertexCount++].color = endColor;

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer2D::DrawLines(const Vec2f* positions, uint32_t size, const Vec4f& color)
{
	CHECK(size <= MAX_VERTEX_SIZE);

	for (uint32_t index = 0; index < size; ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.375f, positions[index].y + 0.375f);
		vertices_[index].color = color;
	}

	Draw(Mat4x4::Identity(), EDrawMode::LINES, size);
}

void Renderer2D::DrawLines(const Vec2f* positions, const Vec4f* colors, uint32_t size)
{
	CHECK(size <= MAX_VERTEX_SIZE);

	for (uint32_t index = 0; index < size; ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.375f, positions[index].y + 0.375f);
		vertices_[index].color = colors[index];
	}

	Draw(Mat4x4::Identity(), EDrawMode::LINES, size);
}

void Renderer2D::DrawTriangle(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = Vec2f(fromPos.x + 0.375f, fromPos.y + 0.375f);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = Vec2f(byPos.x + 0.375f, byPos.y + 0.375f);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = Vec2f(toPos.x + 0.375f, toPos.y + 0.375f);
	vertices_[vertexCount++].color = color;

	Draw(Mat4x4::Identity(), EDrawMode::TRIANGLES, vertexCount);
}

void Renderer2D::DrawTriangle(const Vec2f& fromPos, const Vec4f& fromColor, const Vec2f& byPos, const Vec4f& byColor, const Vec2f& toPos, const Vec4f& toColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = Vec2f(fromPos.x + 0.375f, fromPos.y + 0.375f);
	vertices_[vertexCount++].color = fromColor;

	vertices_[vertexCount].position = Vec2f(byPos.x + 0.375f, byPos.y + 0.375f);
	vertices_[vertexCount++].color = byColor;

	vertices_[vertexCount].position = Vec2f(toPos.x + 0.375f, toPos.y + 0.375f);
	vertices_[vertexCount++].color = toColor;

	Draw(Mat4x4::Identity(), EDrawMode::TRIANGLES, vertexCount);
}

void Renderer2D::DrawTriangleWireframe(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = Vec2f(fromPos.x + 0.375f, fromPos.y + 0.375f);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = Vec2f(byPos.x + 0.375f, byPos.y + 0.375f);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = Vec2f(toPos.x + 0.375f, toPos.y + 0.375f);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = Vec2f(fromPos.x + 0.375f, fromPos.y + 0.375f);
	vertices_[vertexCount++].color = color;

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer2D::DrawTriangleWireframe(const Vec2f& fromPos, const Vec4f& fromColor, const Vec2f& byPos, const Vec4f& byColor, const Vec2f& toPos, const Vec4f& toColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = Vec2f(fromPos.x + 0.375f, fromPos.y + 0.375f);
	vertices_[vertexCount++].color = fromColor;

	vertices_[vertexCount].position = Vec2f(byPos.x + 0.375f, byPos.y + 0.375f);
	vertices_[vertexCount++].color = byColor;

	vertices_[vertexCount].position = Vec2f(toPos.x + 0.375f, toPos.y + 0.375f);
	vertices_[vertexCount++].color = toColor;

	vertices_[vertexCount].position = Vec2f(fromPos.x + 0.375f, fromPos.y + 0.375f);
	vertices_[vertexCount++].color = fromColor;

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer2D::DrawRect(const Vec2f& center, float w, float h, const Vec4f& color, float rotate)
{
	uint32_t vertexCount = 0;

	Vec2f c = center + Vec2f(0.375f, 0.375f);
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;

	vertices_[vertexCount].position = c + Vec2f(-w2, +h2);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = c + Vec2f(+w2, +h2);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = c + Vec2f(+w2, -h2);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = c + Vec2f(-w2, -h2);
	vertices_[vertexCount++].color = color;

	Mat4x4 transform = Mat4x4::Translation(-c.x, -c.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+c.x, +c.y, 0.0f);

	Draw(transform, EDrawMode::TRIANGLE_FAN, vertexCount);
}

void Renderer2D::DrawRectWireframe(const Vec2f& center, float w, float h, const Vec4f& color, float rotate)
{
	uint32_t vertexCount = 0;

	Vec2f c = center + Vec2f(0.375f, 0.375f);
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;

	vertices_[vertexCount].position = c + Vec2f(-w2, +h2);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = c + Vec2f(+w2, +h2);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = c + Vec2f(+w2, -h2);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = c + Vec2f(-w2, -h2);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = c + Vec2f(-w2, +h2);
	vertices_[vertexCount++].color = color;

	Mat4x4 transform = Mat4x4::Translation(-c.x, -c.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+c.x, +c.y, 0.0f);

	Draw(transform, EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer2D::DrawRoundRect(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate)
{
	uint32_t vertexCount = 0;

	Vec2f c = center + Vec2f(0.375f, 0.375f);
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;
	side = MathModule::Min<float>(side, MathModule::Min<float>(w2, h2));

	vertices_[vertexCount].position = c;
	vertices_[vertexCount++].color = color;

	auto calculateBezierCurve = [&](const Vec2f& startPos, const Vec2f& endPos, const Vec2f& controlPos, uint32_t sliceCount)
		{
			for (int32_t slice = 0; slice <= sliceCount; ++slice)
			{
				float t = static_cast<float>(slice) / static_cast<float>(sliceCount);
				Vec2f p = startPos * (1.0f - t) + controlPos * t;
				Vec2f q = controlPos * (1.0f - t) + endPos * t;
				Vec2f r = p * (1.0f - t) + q * t;

				vertices_[vertexCount].position = Vec2f(r.x, r.y) + Vec2f(0.375f, 0.375f);
				vertices_[vertexCount++].color = color;
			}
		};
	
	Vec2f control = c + Vec2f(-w2, -h2);
	Vec2f start = control + Vec2f(side, 0.0f);
	Vec2f end = control + Vec2f(0.0f, side);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	control = c + Vec2f(-w2, +h2);
	start = control + Vec2f(0.0f, -side);
	end = control + Vec2f(side, 0.0f);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	control = c + Vec2f(+w2, +h2);
	start = control + Vec2f(-side, 0.0f);
	end = control + Vec2f(0.0f, -side);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	control = c + Vec2f(+w2, -h2);
	start = control + Vec2f(0.0f, +side);
	end = control + Vec2f(-side, 0.0f);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);
	
	control = c + Vec2f(-w2, -h2);
	vertices_[vertexCount].position = Vec2f(control.x + side, control.y) + Vec2f(0.375f, 0.375f);
	vertices_[vertexCount++].color = color;
	
	Mat4x4 transform = Mat4x4::Translation(-c.x, -c.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+c.x, +c.y, 0.0f);

	Draw(transform, EDrawMode::TRIANGLE_FAN, vertexCount);
}

void Renderer2D::DrawRoundRectWireframe(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate)
{
	uint32_t vertexCount = 0;

	Vec2f c = center + Vec2f(0.375f, 0.375f);
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;
	side = MathModule::Min<float>(side, MathModule::Min<float>(w2, h2));
	
	auto calculateBezierCurve = [&](const Vec2f& startPos, const Vec2f& endPos, const Vec2f& controlPos, uint32_t sliceCount)
		{
			for (int32_t slice = 0; slice <= sliceCount; ++slice)
			{
				float t = static_cast<float>(slice) / static_cast<float>(sliceCount);
				Vec2f p = startPos * (1.0f - t) + controlPos * t;
				Vec2f q = controlPos * (1.0f - t) + endPos * t;
				Vec2f r = p * (1.0f - t) + q * t;

				vertices_[vertexCount].position = Vec2f(r.x, r.y) + Vec2f(0.375f, 0.375f);
				vertices_[vertexCount++].color = color;
			}
		};

	Vec2f control = c + Vec2f(-w2, -h2);
	Vec2f start = control + Vec2f(side, 0.0f);
	Vec2f end = control + Vec2f(0.0f, side);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	control = c + Vec2f(-w2, +h2);
	start = control + Vec2f(0.0f, -side);
	end = control + Vec2f(side, 0.0f);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	control = c + Vec2f(+w2, +h2);
	start = control + Vec2f(-side, 0.0f);
	end = control + Vec2f(0.0f, -side);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	control = c + Vec2f(+w2, -h2);
	start = control + Vec2f(0.0f, +side);
	end = control + Vec2f(-side, 0.0f);
	calculateBezierCurve(start, end, control, MAX_SLICE_SIZE);

	control = c + Vec2f(-w2, -h2);
	vertices_[vertexCount].position = Vec2f(control.x + side, control.y) + Vec2f(0.375f, 0.375f);
	vertices_[vertexCount++].color = color;

	Mat4x4 transform = Mat4x4::Translation(-c.x, -c.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+c.x, +c.y, 0.0f);

	Draw(transform, EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer2D::DrawCircle(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount)
{
	CHECK(radius >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	for (int32_t slice = 1; slice <= sliceCount; ++slice)
	{
		float radian = (static_cast<float>(slice - 1) * TwoPi) / static_cast<float>(sliceCount);
		float x = radius * MathModule::Cos(radian);
		float y = radius * MathModule::Sin(radian);

		vertices_[slice].position = Vec2f(center.x + x, center.y + y) + Vec2f(0.375f, 0.375f);
		vertices_[slice].color = color;
	}

	vertices_[0].position = Vec2f(center.x, center.y) + Vec2f(0.375f, 0.375f);
	vertices_[0].color = color;

	vertices_[sliceCount + 1] = vertices_[1];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 2);

	Draw(Mat4x4::Identity(), EDrawMode::TRIANGLE_FAN, vertexCount);
}

void Renderer2D::DrawCircleWireframe(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount)
{
	CHECK(radius >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	for (int32_t index = 0; index < sliceCount; ++index)
	{
		float radian = (static_cast<float>(index) * TwoPi) / static_cast<float>(sliceCount);
		float x = radius * MathModule::Cos(radian);
		float y = radius * MathModule::Sin(radian);

		vertices_[index].position = Vec2f(center.x + x, center.y + y) + Vec2f(0.375f, 0.375f);
		vertices_[index].color = color;
	}

	vertices_[sliceCount] = vertices_[0];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 1);

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer2D::Draw(const Mat4x4& transform, const EDrawMode& drawMode, uint32_t vertexCount)
{
	CHECK(drawMode != EDrawMode::NONE);

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	GLboolean originEnableCull;
	GL_FAILED(glGetBooleanv(GL_CULL_FACE, &originEnableCull));

	RenderModule::SetDepthMode(false);
	RenderModule::SetCullFaceMode(false);
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
	RenderModule::SetCullFaceMode(originEnableCull);
	RenderModule::SetDepthMode(originEnableDepth);
}

#pragma warning(pop)