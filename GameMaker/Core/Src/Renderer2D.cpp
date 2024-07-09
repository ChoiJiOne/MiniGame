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

static RenderManager* renderManager = nullptr;

Renderer2D::Renderer2D()
{
	if (!renderManager)
	{
		renderManager = &RenderManager::Get();
	}

	ResourceManager& resourceManager = ResourceManager::Get();

	std::string vsPath = "GameMaker/Shader/Renderer2D.vert";
	std::string fsPath = "GameMaker/Shader/Renderer2D.frag";
	shader_ = resourceManager.Create<Shader>(vsPath, fsPath);

	uint32_t byteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	VertexBuffer::EUsage usage = VertexBuffer::EUsage::DYNAMIC;
	vertexBuffer_ = resourceManager.Create<VertexBuffer>(byteSize, usage);

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

	ResourceManager& resourceManager = ResourceManager::Get();

	if (shader_)
	{
		resourceManager.Destroy(shader_);
		shader_ = nullptr;
	}

	if (vertexBuffer_)
	{
		resourceManager.Destroy(vertexBuffer_);
		vertexBuffer_ = nullptr;
	}

	GL_FAILED(glDeleteVertexArrays(1, &vertexArrayObject_));

	bIsInitialized_ = false;
}

void GameMaker::Renderer2D::Begin(const Camera2D* camera2D)
{
	CHECK(!bIsBegin_);

	Mat4x4 ortho;
	if (!camera2D)
	{
		float w = 0.0f;
		float h = 0.0f;
		static const float nearZ = -1.0f;
		static const float farZ = 1.0f;
		renderManager->GetScreenSize<float>(w, h);

		ortho = Mat4x4::Ortho(-w * 0.5f, +w * 0.5f, -h * 0.5f, +h * 0.5f, nearZ, farZ);
	}
	else
	{
		ortho = camera2D->GetOrtho();
	}

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	GLboolean originEnableCull;
	GL_FAILED(glGetBooleanv(GL_CULL_FACE, &originEnableCull));

	bIsBeforeEnableDepth_ = static_cast<bool>(originEnableDepth);
	bIsBeforeEnableCull_ = static_cast<bool>(originEnableCull);

	renderManager->SetDepthMode(false);
	renderManager->SetCullFaceMode(false);

	shader_->Bind();
	{
		shader_->SetUniform("ortho", ortho);
	}
	shader_->Unbind();

	bIsBegin_ = true;
}

void Renderer2D::End()
{
	CHECK(bIsBegin_);

	renderManager->SetCullFaceMode(bIsBeforeEnableCull_);
	renderManager->SetDepthMode(bIsBeforeEnableDepth_);

	bIsBegin_ = false;
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
	Draw(Mat4x4::Identity(), EDrawMode::POINTS, size, EMode::GEOMETRY);
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
	Draw(Mat4x4::Identity(), EDrawMode::POINTS, size, EMode::GEOMETRY);
}

void Renderer2D::DrawLine(const Vec2f* positions, const Vec4f* colors, uint32_t size)
{
	CHECK(size <= MAX_VERTEX_SIZE);

	for (uint32_t index = 0; index < size; ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.375f, positions[index].y + 0.375f);
		vertices_[index].color = colors[index];
	}

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, size, EMode::GEOMETRY);
}

void Renderer2D::DrawLine(const Vec2f& startPos, const Vec2f& endPos, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = Vec2f(startPos.x + 0.375f, startPos.y + 0.375f);
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = Vec2f(endPos.x + 0.375f, endPos.y + 0.375f);
	vertices_[vertexCount++].color = color;

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount, EMode::GEOMETRY);
}

void Renderer2D::DrawLine(const Vec2f& startPos, const Vec4f& startColor, const Vec2f& endPos, const Vec4f& endColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = Vec2f(startPos.x + 0.375f, startPos.y + 0.375f);
	vertices_[vertexCount++].color = startColor;

	vertices_[vertexCount].position = Vec2f(endPos.x + 0.375f, endPos.y + 0.375f);
	vertices_[vertexCount++].color = endColor;

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount, EMode::GEOMETRY);
}

void Renderer2D::DrawLines(const Vec2f* positions, uint32_t size, const Vec4f& color)
{
	CHECK(size <= MAX_VERTEX_SIZE);

	for (uint32_t index = 0; index < size; ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.375f, positions[index].y + 0.375f);
		vertices_[index].color = color;
	}

	Draw(Mat4x4::Identity(), EDrawMode::LINES, size, EMode::GEOMETRY);
}

void Renderer2D::DrawLines(const Vec2f* positions, const Vec4f* colors, uint32_t size)
{
	CHECK(size <= MAX_VERTEX_SIZE);

	for (uint32_t index = 0; index < size; ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.375f, positions[index].y + 0.375f);
		vertices_[index].color = colors[index];
	}

	Draw(Mat4x4::Identity(), EDrawMode::LINES, size, EMode::GEOMETRY);
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

	Draw(Mat4x4::Identity(), EDrawMode::TRIANGLES, vertexCount, EMode::GEOMETRY);
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

	Draw(Mat4x4::Identity(), EDrawMode::TRIANGLES, vertexCount, EMode::GEOMETRY);
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

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount, EMode::GEOMETRY);
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

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount, EMode::GEOMETRY);
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

	Draw(transform, EDrawMode::TRIANGLE_FAN, vertexCount, EMode::GEOMETRY);
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

	Draw(transform, EDrawMode::LINE_STRIP, vertexCount, EMode::GEOMETRY);
}

void Renderer2D::DrawRoundRect(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate)
{
	uint32_t vertexCount = 0;

	Vec2f c = center + Vec2f(0.375f, 0.375f);
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;
	side = Min<float>(side, Min<float>(w2, h2));

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

	Draw(transform, EDrawMode::TRIANGLE_FAN, vertexCount, EMode::GEOMETRY);
}

void Renderer2D::DrawRoundRectWireframe(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate)
{
	uint32_t vertexCount = 0;

	Vec2f c = center + Vec2f(0.375f, 0.375f);
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;
	side = Min<float>(side, Min<float>(w2, h2));

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

	Draw(transform, EDrawMode::LINE_STRIP, vertexCount, EMode::GEOMETRY);
}

void Renderer2D::DrawCircle(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount)
{
	CHECK(radius >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	for (int32_t slice = 1; slice <= sliceCount; ++slice)
	{
		float radian = (static_cast<float>(slice - 1) * TWO_PI) / static_cast<float>(sliceCount);
		float x = radius * Cos(radian);
		float y = radius * Sin(radian);

		vertices_[slice].position = Vec2f(center.x + x, center.y + y) + Vec2f(0.375f, 0.375f);
		vertices_[slice].color = color;
	}

	vertices_[0].position = Vec2f(center.x, center.y) + Vec2f(0.375f, 0.375f);
	vertices_[0].color = color;

	vertices_[sliceCount + 1] = vertices_[1];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 2);

	Draw(Mat4x4::Identity(), EDrawMode::TRIANGLE_FAN, vertexCount, EMode::GEOMETRY);
}

void Renderer2D::DrawCircleWireframe(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount)
{
	CHECK(radius >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	for (int32_t index = 0; index < sliceCount; ++index)
	{
		float radian = (static_cast<float>(index) * TWO_PI) / static_cast<float>(sliceCount);
		float x = radius * Cos(radian);
		float y = radius * Sin(radian);

		vertices_[index].position = Vec2f(center.x + x, center.y + y) + Vec2f(0.375f, 0.375f);
		vertices_[index].color = color;
	}

	vertices_[sliceCount] = vertices_[0];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 1);

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount, EMode::GEOMETRY);
}

void Renderer2D::DrawEllipse(const Vec2f& center, float xAxis, float yAxis, const Vec4f& color, float rotate, int32_t sliceCount)
{
	CHECK(xAxis >= 0.0f && yAxis >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	float x2 = xAxis * 0.5f;
	float y2 = yAxis * 0.5f;

	for (int32_t slice = 1; slice <= sliceCount; ++slice)
	{
		float radian = (static_cast<float>(slice - 1) * TWO_PI) / static_cast<float>(sliceCount);
		float x = x2 * Cos(radian);
		float y = y2 * Sin(radian);

		vertices_[slice].position = Vec2f(center.x + x, center.y + y) + Vec2f(0.375f, 0.375f);
		vertices_[slice].color = color;
	}

	vertices_[0].position = Vec2f(center.x, center.y) + Vec2f(0.375f, 0.375f);
	vertices_[0].color = color;

	vertices_[sliceCount + 1] = vertices_[1];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 2);

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);

	Draw(transform, EDrawMode::TRIANGLE_FAN, vertexCount, EMode::GEOMETRY);
}

void Renderer2D::DrawEllipseWireframe(const Vec2f& center, float xAxis, float yAxis, const Vec4f& color, float rotate, int32_t sliceCount)
{
	CHECK(xAxis >= 0.0f && yAxis >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	float x2 = xAxis * 0.5f;
	float y2 = yAxis * 0.5f;

	for (int32_t index = 0; index < sliceCount; ++index)
	{
		float radian = (static_cast<float>(index) * TWO_PI) / static_cast<float>(sliceCount);
		float x = x2 * Cos(radian);
		float y = y2 * Sin(radian);

		vertices_[index].position = Vec2f(center.x + x, center.y + y) + Vec2f(0.375f, 0.375f);
		vertices_[index].color = color;
	}

	vertices_[sliceCount] = vertices_[0];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 1);

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	Draw(transform, EDrawMode::LINE_STRIP, vertexCount, EMode::GEOMETRY);
}

void Renderer2D::DrawString(const TTFont* font, const std::wstring& text, const Vec2f& pos, const Vec4f& color)
{
	uint32_t vertexCount = SetGlyphVertexBuffer(font, text, pos, color);

	GL_FAILED(glActiveTexture(GL_TEXTURE0 + GLYPH_ATLAS_BIND_SLOT));
	GL_FAILED(glBindTexture(GL_TEXTURE_2D, font->GetGlyphAtlasID()));

	Draw(Mat4x4::Identity(), EDrawMode::TRIANGLES, vertexCount, EMode::STRING);
}

void Renderer2D::DrawSprite(const ITexture* texture, const Vec2f& center, float w, float h, float rotate, bool bFlipH, bool bFlipV)
{
	CHECK(texture);

	uint32_t vertexCount = 0;

	Vec2f c = center + Vec2f(0.375f, 0.375f);
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;

	vertices_[vertexCount].position = c + Vec2f(-w2, -h2);
	vertices_[vertexCount++].uv = Vec2f(0.0f, 0.0f);

	vertices_[vertexCount].position = c + Vec2f(+w2, -h2);
	vertices_[vertexCount++].uv = Vec2f(1.0f, 0.0f);

	vertices_[vertexCount].position = c + Vec2f(+w2, +h2);
	vertices_[vertexCount++].uv = Vec2f(1.0f, 1.0f);

	vertices_[vertexCount].position = c + Vec2f(-w2, +h2);
	vertices_[vertexCount++].uv = Vec2f(0.0f, 1.0f);

	Mat4x4 transform = Mat4x4::Translation(-c.x, -c.y, 0.0f);
	if (bFlipH)
	{
		transform = transform * Mat4x4::RotateY(PI);
	}
	if (bFlipV)
	{
		transform = transform * Mat4x4::RotateX(PI);
	}
	transform = transform * Mat4x4::RotateZ(rotate);
	transform = transform * Mat4x4::Translation(+c.x, +c.y, 0.0f);
	
	factor_ = 0.0f;

	texture->Active(SPRITE_BIND_SLOT);
	Draw(transform, EDrawMode::TRIANGLE_FAN, vertexCount, EMode::SPRITE);
}

void Renderer2D::DrawSprite(const ITexture* texture, const Vec2f& center, float w, float h, const Vec4f& blend, float factor, float rotate, bool bFlipH, bool bFlipV)
{
	CHECK(texture);

	uint32_t vertexCount = 0;

	Vec2f c = center + Vec2f(0.375f, 0.375f);
	float w2 = w * 0.5f;
	float h2 = h * 0.5f;

	vertices_[vertexCount].position = c + Vec2f(-w2, -h2);
	vertices_[vertexCount].uv = Vec2f(0.0f, 0.0f);
	vertices_[vertexCount++].color = blend;

	vertices_[vertexCount].position = c + Vec2f(+w2, -h2);
	vertices_[vertexCount].uv = Vec2f(1.0f, 0.0f);
	vertices_[vertexCount++].color = blend;

	vertices_[vertexCount].position = c + Vec2f(+w2, +h2);
	vertices_[vertexCount].uv = Vec2f(1.0f, 1.0f);
	vertices_[vertexCount++].color = blend;

	vertices_[vertexCount].position = c + Vec2f(-w2, +h2);
	vertices_[vertexCount].uv = Vec2f(0.0f, 1.0f);
	vertices_[vertexCount++].color = blend;
	
	Mat4x4 transform = Mat4x4::Translation(-c.x, -c.y, 0.0f);
	if (bFlipH)
	{
		transform = transform * Mat4x4::RotateY(PI);
	}
	if (bFlipV)
	{
		transform = transform * Mat4x4::RotateX(PI);
	}
	transform = transform * Mat4x4::RotateZ(rotate);
	transform = transform * Mat4x4::Translation(+c.x, +c.y, 0.0f);
	
	factor_ = Clamp<float>(factor, 0.0f, 1.0f);

	texture->Active(SPRITE_BIND_SLOT);
	Draw(transform, EDrawMode::TRIANGLE_FAN, vertexCount, EMode::SPRITE);
}

uint32_t Renderer2D::SetGlyphVertexBuffer(const TTFont* font, const std::wstring& text, const Vec2f& pos, const Vec4f& color)
{
	float w = 0.0f;
	float h = 0.0f;
	font->MeasureText(text, w, h);

	float atlasSize = static_cast<float>(font->GetGlyphAtlasSize());

	Vec2f currPos = Vec2f(pos.x, pos.y - h);
	int32_t vertexCount = 0;

	for (const auto& unicode : text)
	{
		const Glyph& glyph = font->GetGlyph(static_cast<int32_t>(unicode));

		float unicodeWidth = static_cast<float>(glyph.position1.x - glyph.position0.x);
		float unicodeHeight = static_cast<float>(glyph.position1.y - glyph.position0.y);

		vertices_[vertexCount + 0].position = Vec2f(currPos.x + glyph.xoffset, currPos.y - glyph.yoffset);
		vertices_[vertexCount + 0].uv = Vec2f(static_cast<float>(glyph.position0.x) / atlasSize, static_cast<float>(glyph.position0.y) / atlasSize);
		vertices_[vertexCount + 0].color = color;

		vertices_[vertexCount + 1].position = Vec2f(currPos.x + glyph.xoffset, currPos.y - unicodeHeight - glyph.yoffset);
		vertices_[vertexCount + 1].uv = Vec2f(static_cast<float>(glyph.position0.x) / atlasSize, static_cast<float>(glyph.position1.y) / atlasSize);
		vertices_[vertexCount + 1].color = color;

		vertices_[vertexCount + 2].position = Vec2f(currPos.x + glyph.xoffset + unicodeWidth, currPos.y - glyph.yoffset);
		vertices_[vertexCount + 2].uv = Vec2f(static_cast<float>(glyph.position1.x) / atlasSize, static_cast<float>(glyph.position0.y) / atlasSize);
		vertices_[vertexCount + 2].color = color;

		vertices_[vertexCount + 3].position = Vec2f(currPos.x + glyph.xoffset + unicodeWidth, currPos.y - glyph.yoffset);
		vertices_[vertexCount + 3].uv = Vec2f(static_cast<float>(glyph.position1.x) / atlasSize, static_cast<float>(glyph.position0.y) / atlasSize);
		vertices_[vertexCount + 3].color = color;

		vertices_[vertexCount + 4].position = Vec2f(currPos.x + glyph.xoffset, currPos.y - unicodeHeight - glyph.yoffset);
		vertices_[vertexCount + 4].uv = Vec2f(static_cast<float>(glyph.position0.x) / atlasSize, static_cast<float>(glyph.position1.y) / atlasSize);
		vertices_[vertexCount + 4].color = color;

		vertices_[vertexCount + 5].position = Vec2f(currPos.x + glyph.xoffset + unicodeWidth, currPos.y - unicodeHeight - glyph.yoffset);
		vertices_[vertexCount + 5].uv = Vec2f(static_cast<float>(glyph.position1.x) / atlasSize, static_cast<float>(glyph.position1.y) / atlasSize);
		vertices_[vertexCount + 5].color = color;

		currPos.x += glyph.xadvance;
		vertexCount += 6;
	}

	return vertexCount;
}

void Renderer2D::Draw(const Mat4x4& transform, const EDrawMode& drawMode, uint32_t vertexCount, const EMode& mode)
{
	CHECK(drawMode != EDrawMode::NONE);

	const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
	uint32_t bufferByteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	vertexBuffer_->SetBufferData(vertexPtr, bufferByteSize);

	shader_->Bind();
	{
		/** 공통적으로 설정. */
		shader_->SetUniform("transform", transform);
		shader_->SetUniform("mode", static_cast<int32_t>(mode));

		if (drawMode == EDrawMode::POINTS)
		{
			shader_->SetUniform("pointSize", pointSize_);
		}

		if (mode == EMode::SPRITE)
		{
			shader_->SetUniform("factor", factor_);
		}

		GL_FAILED(glBindVertexArray(vertexArrayObject_));
		renderManager->ExecuteDrawVertex(vertexCount, drawMode);
		GL_FAILED(glBindVertexArray(0));
	}
	shader_->Unbind();
}

#pragma warning(pop)