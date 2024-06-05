#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)

#include <glad/glad.h>

#include "GLAssertion.h"
#include "GeometryRenderer.h"
#include "RenderModule.h"
#include "Shader.h"

GeometryRenderer::GeometryRenderer()
{
	shader_ = RenderModule::CreateResource<Shader>("Resource/Shader/Geometry.vert", "Resource/Shader/Geometry.frag");

	uint32_t byteSize = static_cast<uint32_t>(VertexPositionColor3D::GetStride() * vertices_.size());
	VertexBuffer::EUsage usage = VertexBuffer::EUsage::DYNAMIC;
	vertexBuffer_ = RenderModule::CreateResource<VertexBuffer>(byteSize, usage);

	GL_FAILED(glGenVertexArrays(1, &vertexArrayObject_));
	GL_FAILED(glBindVertexArray(vertexArrayObject_));
	{
		vertexBuffer_->Bind();

		GL_FAILED(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexPositionColor3D::GetStride(), (void*)(offsetof(VertexPositionColor3D, position))));
		GL_FAILED(glEnableVertexAttribArray(0));

		GL_FAILED(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexPositionColor3D::GetStride(), (void*)(offsetof(VertexPositionColor3D, color))));
		GL_FAILED(glEnableVertexAttribArray(1));

		vertexBuffer_->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));
}

GeometryRenderer::~GeometryRenderer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void GeometryRenderer::Release()
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
}

void GeometryRenderer::DrawPoints2D(const std::vector<Vec2f>& positions, const Vec4f& color, float pointSize)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);
	CHECK(pointSize >= 0.0f);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index].position = Vec3f(positions[index].x + 0.5f, positions[index].y + 0.5f, 0.0f);
		vertices_[index].color = color;
	}

	pointSize_ = pointSize;
	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::POINTS, static_cast<uint32_t>(positions.size()), false);
}

void GeometryRenderer::DrawConnectPoints2D(const std::vector<Vec2f>& positions, const Vec4f& color)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index].position = Vec3f(positions[index].x + 0.5f, positions[index].y + 0.5f, 0.0f);
		vertices_[index].color = color;
	}

	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::LINE_STRIP, static_cast<uint32_t>(positions.size()), false);
}

void GeometryRenderer::DrawLine2D(const Vec2f& fromPosition, const Vec2f& toPosition, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(fromPosition.x + 0.5f, fromPosition.y + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(toPosition.x + 0.5f, toPosition.y + 0.5f, 0.0f), color);

	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::LINE_STRIP, vertexCount, false);
}

void GeometryRenderer::DrawLine2D(const Vec2f& fromPosition, const Vec4f& fromColor, const Vec2f& toPosition, const Vec4f& toColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(fromPosition.x + 0.5f, fromPosition.y + 0.5f, 0.0f), fromColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(toPosition.x + 0.5f, toPosition.y + 0.5f, 0.0f), toColor);

	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::LINE_STRIP, vertexCount, false);
}

void GeometryRenderer::DrawLines2D(const std::vector<Vec2f>& positions, const Vec4f& color)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index].position = Vec3f(positions[index].x + 0.5f, positions[index].y + 0.5f, 0.0f);
		vertices_[index].color = color;
	}

	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::LINES, static_cast<uint32_t>(positions.size()), false);
}

void GeometryRenderer::DrawTriangle2D(const Vec2f& fromPosition, const Vec2f& byPosition, const Vec2f& toPosition, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(fromPosition.x + 0.5f, fromPosition.y + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(byPosition.x + 0.5f, byPosition.y + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(toPosition.x + 0.5f, toPosition.y + 0.5f, 0.0f), color);

	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::TRIANGLES, vertexCount, false);
}

void GeometryRenderer::DrawTriangle2D(const Vec2f& fromPosition, const Vec4f& fromColor, const Vec2f& byPosition, const Vec4f& byColor, const Vec2f& toPosition, const Vec4f& toColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(fromPosition.x + 0.5f, fromPosition.y + 0.5f, 0.0f), fromColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(byPosition.x + 0.5f, byPosition.y + 0.5f, 0.0f), byColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(toPosition.x + 0.5f, toPosition.y + 0.5f, 0.0f), toColor);

	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::TRIANGLES, vertexCount, false);
}

void GeometryRenderer::DrawWireframeTriangle2D(const Vec2f& fromPosition, const Vec2f& byPosition, const Vec2f& toPosition, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(fromPosition.x + 0.5f, fromPosition.y + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(byPosition.x + 0.5f, byPosition.y + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(toPosition.x + 0.5f, toPosition.y + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(fromPosition.x + 0.5f, fromPosition.y + 0.5f, 0.0f), color);

	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::LINE_STRIP, vertexCount, false);
}

void GeometryRenderer::DrawWireframeTriangle2D(const Vec2f& fromPosition, const Vec4f& fromColor, const Vec2f& byPosition, const Vec4f& byColor, const Vec2f& toPosition, const Vec4f& toColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(fromPosition.x + 0.5f, fromPosition.y + 0.5f, 0.0f), fromColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(byPosition.x + 0.5f, byPosition.y + 0.5f, 0.0f), byColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(toPosition.x + 0.5f, toPosition.y + 0.5f, 0.0f), toColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(fromPosition.x + 0.5f, fromPosition.y + 0.5f, 0.0f), fromColor);

	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::LINE_STRIP, vertexCount, false);
}

void GeometryRenderer::DrawRectangle2D(const Vec2f& center, float width, float height, float rotate, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(center.x - width / 2.0f + 0.5f, center.y + height / 2.0f + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(center.x + width / 2.0f + 0.5f, center.y + height / 2.0f + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(center.x + width / 2.0f + 0.5f, center.y - height / 2.0f + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(center.x - width / 2.0f + 0.5f, center.y - height / 2.0f + 0.5f, 0.0f), color);

	Mat4x4 world = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry(world, Mat4x4::Identity(), ortho_, EDrawMode::TRIANGLE_FAN, vertexCount, false);
}

void GeometryRenderer::DrawWireframeRectangle2D(const Vec2f& center, float width, float height, float rotate, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(center.x - width / 2.0f + 0.5f, center.y - height / 2.0f + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(center.x - width / 2.0f + 0.5f, center.y + height / 2.0f + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(center.x + width / 2.0f + 0.5f, center.y + height / 2.0f + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(center.x + width / 2.0f + 0.5f, center.y - height / 2.0f + 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(center.x - width / 2.0f + 0.5f, center.y - height / 2.0f + 0.5f, 0.0f), color);

	Mat4x4 world = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry(world, Mat4x4::Identity(), ortho_, EDrawMode::LINE_STRIP, vertexCount, false);
}

void GeometryRenderer::DrawRoundRectangle2D(const Vec2f& center, float width, float height, float side, float rotate, const Vec4f& color)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	side = MathModule::Min<float>(side, MathModule::Min<float>(halfWidth, halfHeight));

	uint32_t vertexCount = 0;
	vertices_[vertexCount++].position = Vec3f(center.x, center.y, 0.0f);

	auto computeSpline = [&](const Vec2f& startPosition, const Vec2f& endPosition, const Vec2f& controlPosition)
	{
		for (int32_t slice = 0; slice <= MAX_SLICE_SIZE; ++slice)
		{
			float t = static_cast<float>(slice) / static_cast<float>(MAX_SLICE_SIZE);
			Vec2f p = startPosition * (1.0f - t) + controlPosition * t;
			Vec2f q = controlPosition * (1.0f - t) + endPosition * t;
			Vec2f r = p * (1.0f - t) + q * t;
			vertices_[vertexCount++].position = Vec3f(r.x, r.y, 0.0f);
		}
	};

	Vec2f control, start, end;

	control = Vec2f(center.x - halfWidth, center.y - halfHeight);
	start = Vec2f(control.x + side, control.y);
	end = Vec2f(control.x, control.y + side);
	computeSpline(start, end, control);

	control = Vec2f(center.x - halfWidth, center.y + halfHeight);
	start = Vec2f(control.x, control.y - side);
	end = Vec2f(control.x + side, control.y);
	computeSpline(start, end, control);

	control = Vec2f(center.x + halfWidth, center.y + halfHeight);
	start = Vec2f(control.x - side, control.y);
	end = Vec2f(control.x, control.y - side);
	computeSpline(start, end, control);

	control = Vec2f(center.x + halfWidth, center.y - halfHeight);
	start = Vec2f(control.x, control.y + side);
	end = Vec2f(control.x - side, control.y);
	computeSpline(start, end, control);

	control = Vec2f(center.x - halfWidth, center.y - halfHeight);
	vertices_[vertexCount++].position = Vec3f(control.x + side, control.y, 0.0f);

	for (int32_t index = 0; index < vertexCount; ++index)
	{
		vertices_[index].color = color;
	}

	Mat4x4 world = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry(world, Mat4x4::Identity(), ortho_, EDrawMode::TRIANGLE_FAN, vertexCount, false);
}

void GeometryRenderer::DrawWireframeRoundRectangle2D(const Vec2f& center, float width, float height, float side, float rotate, const Vec4f& color)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	side = MathModule::Min<float>(side, MathModule::Min<float>(halfWidth, halfHeight));

	uint32_t vertexCount = 0;

	auto computeSpline = [&](const Vec2f& startPosition, const Vec2f& endPosition, const Vec2f& controlPosition)
	{
		for (int32_t slice = 0; slice <= MAX_SLICE_SIZE; ++slice)
		{
			float t = static_cast<float>(slice) / static_cast<float>(MAX_SLICE_SIZE);
			Vec2f p = startPosition * (1.0f - t) + controlPosition * t;
			Vec2f q = controlPosition * (1.0f - t) + endPosition * t;
			Vec2f r = p * (1.0f - t) + q * t;
			vertices_[vertexCount++].position = Vec3f(r.x, r.y, 0.0f);
		}
	};

	Vec2f control, start, end;

	control = Vec2f(center.x - halfWidth, center.y - halfHeight);
	start = Vec2f(control.x + side, control.y);
	end = Vec2f(control.x, control.y + side);
	computeSpline(start, end, control);

	control = Vec2f(center.x - halfWidth, center.y + halfHeight);
	start = Vec2f(control.x, control.y - side);
	end = Vec2f(control.x + side, control.y);
	computeSpline(start, end, control);

	control = Vec2f(center.x + halfWidth, center.y + halfHeight);
	start = Vec2f(control.x - side, control.y);
	end = Vec2f(control.x, control.y - side);
	computeSpline(start, end, control);

	control = Vec2f(center.x + halfWidth, center.y - halfHeight);
	start = Vec2f(control.x, control.y + side);
	end = Vec2f(control.x - side, control.y);
	computeSpline(start, end, control);

	control = Vec2f(center.x - halfWidth, center.y - halfHeight);
	vertices_[vertexCount++].position = Vec3f(control.x + side, control.y, 0.0f);

	for (int32_t index = 0; index < vertexCount; ++index)
	{
		vertices_[index].color = color;
	}

	Mat4x4 world = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry(world, Mat4x4::Identity(), ortho_, EDrawMode::LINE_STRIP, vertexCount, false);
}

void GeometryRenderer::DrawCircle2D(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount)
{
	CHECK(radius >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	for (int32_t slice = 1; slice <= sliceCount; ++slice)
	{
		float radian = (static_cast<float>(slice - 1) * TwoPi) / static_cast<float>(sliceCount);
		float x = radius * MathModule::Cos(radian);
		float y = radius * MathModule::Sin(radian);

		vertices_[slice] = VertexPositionColor3D(Vec3f(center.x + x + 0.5f, center.y + y + 0.5f, 0.0f), color);
	}

	vertices_[0] = VertexPositionColor3D(Vec3f(center.x + 0.5f, center.y + 0.5f, 0.0f), color);
	vertices_[sliceCount + 1] = vertices_[1];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 2);

	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::TRIANGLE_FAN, vertexCount, false);
}

void GeometryRenderer::DrawWireframeCircle2D(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount)
{
	CHECK(radius >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE);

	for (int32_t index = 0; index < sliceCount; ++index)
	{
		float radian = (static_cast<float>(index) * TwoPi) / static_cast<float>(sliceCount);
		float x = radius * MathModule::Cos(radian);
		float y = radius * MathModule::Sin(radian);

		vertices_[index] = VertexPositionColor3D(Vec3f(center.x + x + 0.5f, center.y + y + 0.5f, 0.0f), color);
	}

	vertices_[sliceCount] = vertices_[0];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 1);

	DrawGeometry(Mat4x4::Identity(), Mat4x4::Identity(), ortho_, EDrawMode::LINE_STRIP, vertexCount, false);
}

void GeometryRenderer::DrawEllipse2D(const Vec2f& center, float xAxis, float yAxis, float rotate, const Vec4f& color, int32_t sliceCount)
{
	CHECK(xAxis >= 0.0f && yAxis >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	float halfXAxis = xAxis / 2.0f;
	float haflYAxis = yAxis / 2.0f;

	for (int32_t slice = 1; slice <= sliceCount; ++slice)
	{
		float radian = (static_cast<float>(slice - 1) * TwoPi) / static_cast<float>(sliceCount);
		float x = halfXAxis * MathModule::Cos(radian);
		float y = haflYAxis * MathModule::Sin(radian);

		vertices_[slice] = VertexPositionColor3D(Vec3f(center.x + x + 0.5f, center.y + y + 0.5f, 0.0f), color);
	}

	vertices_[0] = VertexPositionColor3D(Vec3f(center.x + 0.5f, center.y + 0.5f, 0.0f), color);
	vertices_[sliceCount + 1] = vertices_[1];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 2);

	Mat4x4 world = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry(world, Mat4x4::Identity(), ortho_, EDrawMode::TRIANGLE_FAN, vertexCount, false);
}

void GeometryRenderer::DrawWireframeEllipse2D(const Vec2f& center, float xAxis, float yAxis, float rotate, const Vec4f& color, int32_t sliceCount)
{
	CHECK(xAxis >= 0.0f && yAxis >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	float halfXAxis = xAxis / 2.0f;
	float haflYAxis = yAxis / 2.0f;

	for (int32_t index = 0; index < sliceCount; ++index)
	{
		float radian = (static_cast<float>(index) * TwoPi) / static_cast<float>(sliceCount);
		float x = halfXAxis * MathModule::Cos(radian);
		float y = haflYAxis * MathModule::Sin(radian);

		vertices_[index] = VertexPositionColor3D(Vec3f(center.x + x + 0.5f, center.y + y + 0.5f, 0.0f), color);
	}

	vertices_[sliceCount] = vertices_[0];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 1);

	Mat4x4 world = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry(world, Mat4x4::Identity(), ortho_, EDrawMode::LINE_STRIP, vertexCount, false);
}

void GeometryRenderer::DrawHorizonProgressBar2D(const Vec2f& center, float width, float height, float rate, const Vec4f& color, const Vec4f& bgColor)
{
	rate = MathModule::Clamp<float>(rate, 0.0f, 1.0f);

	float x0 = -width * 0.5f;
	float x1 = +width * 0.5f;
	float y0 = -height * 0.5f;
	float y1 = +height * 0.5f;
	float x = x0 + rate * width;

	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y0, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y0, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y1, 0.0f), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y0, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y1, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y1, 0.0f), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y1, 0.0f), bgColor);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y1, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y1, 0.0f), bgColor);

	DrawGeometry(Mat4x4::Translation(center.x, center.y, 0.0f), Mat4x4::Identity(), ortho_, EDrawMode::TRIANGLES, vertexCount, false);
}

void GeometryRenderer::DrawVerticalProgressBar2D(const Vec2f& center, float width, float height, float rate, const Vec4f& color, const Vec4f& bgColor)
{
	rate = MathModule::Clamp<float>(rate, 0.0f, 1.0f);

	float x0 = -width * 0.5f;
	float x1 = +width * 0.5f;
	float y0 = -height * 0.5f;
	float y1 = +height * 0.5f;
	float y = y1 - rate * height;

	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y, 0.0f), bgColor);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y, 0.0f), bgColor);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y1, 0.0f), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y1, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y1, 0.0f), color);

	DrawGeometry(Mat4x4::Translation(center.x, center.y, 0.0f), Mat4x4::Identity(), ortho_, EDrawMode::TRIANGLES, vertexCount, false);
}

void GeometryRenderer::DrawPoints3D(const std::vector<Vec3f>& positions, const Vec4f& color, float pointSize)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index] = VertexPositionColor3D(positions[index], color);
	}

	pointSize_ = pointSize;
	DrawGeometry(Mat4x4::Identity(), view_, projection_, EDrawMode::POINTS, static_cast<uint32_t>(positions.size()), true);
}

void GeometryRenderer::DrawConnectPoints3D(const std::vector<Vec3f>& positions, const Vec4f& color)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index] = VertexPositionColor3D(positions[index], color);
	}

	DrawGeometry(Mat4x4::Identity(), view_, projection_, EDrawMode::LINE_STRIP, static_cast<uint32_t>(positions.size()), true);
}

void GeometryRenderer::DrawLine3D(const Vec3f& fromPosition, const Vec3f& toPosition, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(fromPosition, color);
	vertices_[vertexCount++] = VertexPositionColor3D(toPosition, color);

	DrawGeometry(Mat4x4::Identity(), view_, projection_, EDrawMode::LINE_STRIP, vertexCount, true);
}

void GeometryRenderer::DrawLine3D(const Vec3f& fromPosition, const Vec4f& fromColor, const Vec3f& toPosition, const Vec4f& toColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(fromPosition, fromColor);
	vertices_[vertexCount++] = VertexPositionColor3D(toPosition, toColor);

	DrawGeometry(Mat4x4::Identity(), view_, projection_, EDrawMode::LINE_STRIP, vertexCount, true);
}

void GeometryRenderer::DrawLines3D(const std::vector<Vec3f>& positions, const Vec4f& color)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index] = VertexPositionColor3D(Vec3f(positions[index].x, positions[index].y, positions[index].z), color);
	}

	DrawGeometry(Mat4x4::Identity(), view_, projection_, EDrawMode::LINES, static_cast<uint32_t>(positions.size()), true);
}

void GeometryRenderer::DrawQuad3D(const Mat4x4& world, float width, float height, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(-width * 0.5f, -height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(+width * 0.5f, -height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(+width * 0.5f, +height * 0.5f, 0.0f), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(-width * 0.5f, -height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(+width * 0.5f, +height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(-width * 0.5f, +height * 0.5f, 0.0f), color);

	DrawGeometry(world, view_, projection_, EDrawMode::TRIANGLES, vertexCount, true);
}

void GeometryRenderer::DrawHorizonProgressBar3D(const Mat4x4& world, float width, float height, float rate, const Vec4f& color, const Vec4f& bgColor)
{
	rate = MathModule::Clamp<float>(rate, 0.0f, 1.0f);

	float x0 = -width * 0.5f;
	float x1 = +width * 0.5f;
	float y0 = -height * 0.5f;
	float y1 = +height * 0.5f;
	float x = x0 + rate * width;

	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y0, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y0, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y1, 0.0f), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y0, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y1, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y1, 0.0f), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y1, 0.0f), bgColor);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y1, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, y1, 0.0f), bgColor);

	DrawGeometry(world, view_, projection_, EDrawMode::TRIANGLES, vertexCount, true);
}

void GeometryRenderer::DrawVerticalProgressBar3D(const Mat4x4& world, float width, float height, float rate, const Vec4f& color, const Vec4f& bgColor)
{
	rate = MathModule::Clamp<float>(rate, 0.0f, 1.0f);

	float x0 = -width * 0.5f;
	float x1 = +width * 0.5f;
	float y0 = -height * 0.5f;
	float y1 = +height * 0.5f;
	float y = y0 + rate * height;

	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y0, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y0, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y, 0.0f), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y0, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y, 0.0f), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y, 0.0f), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y1, 0.0f), bgColor);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x1, y1, 0.0f), bgColor);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x0, y1, 0.0f), bgColor);

	DrawGeometry(world, view_, projection_, EDrawMode::TRIANGLES, vertexCount, true);
}

void GeometryRenderer::DrawCube3D(const Mat4x4& world, const Vec3f& extents, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	Vec3f minPosition = -extents * 0.5f;
	Vec3f maxPosition = extents * 0.5f;

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, maxPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, maxPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, maxPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, maxPosition.y, minPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, maxPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, maxPosition.y, minPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, maxPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, maxPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, minPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, minPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, minPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, minPosition.y, minPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, minPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, minPosition.y, minPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, minPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, minPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, maxPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, minPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, maxPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, minPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, maxPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minPosition.x, minPosition.y, minPosition.z), color);

	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, maxPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxPosition.x, minPosition.y, minPosition.z), color);

	DrawGeometry(world, view_, projection_, EDrawMode::LINES, vertexCount, true);
}

void GeometryRenderer::DrawSphere3D(const Mat4x4& world, float radius, const Vec4f& color)
{
	static const uint32_t sliceCount = 20;

	uint32_t vertexCount = 0;
	float stackStep = Pi / static_cast<float>(sliceCount);
	float sliceStep = TwoPi / static_cast<float>(sliceCount);

	for (uint32_t stack = 0; stack <= sliceCount; ++stack)
	{
		float phi = static_cast<float>(stack) * stackStep;

		for (uint32_t slice = 0; slice < sliceCount; ++slice)
		{
			float theta0 = static_cast<float>(slice + 0) * sliceStep;
			float theta1 = static_cast<float>(slice + 1) * sliceStep;

			CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
			vertices_[vertexCount++] = VertexPositionColor3D(
				Vec3f(
					radius * MathModule::Sin(phi) * MathModule::Cos(theta0),
					radius * MathModule::Cos(phi),
					radius * MathModule::Sin(phi) * MathModule::Sin(theta0)
				),
				color
			);

			CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
			vertices_[vertexCount++] = VertexPositionColor3D(
				Vec3f(
					radius * MathModule::Sin(phi) * MathModule::Cos(theta1),
					radius * MathModule::Cos(phi),
					radius * MathModule::Sin(phi) * MathModule::Sin(theta1)
				),
				color
			);
		}
	}

	stackStep = TwoPi / static_cast<float>(sliceCount);
	for (uint32_t slice = 0; slice < sliceCount; ++slice)
	{
		float theta = static_cast<float>(slice) * sliceStep;

		for (uint32_t stack = 0; stack <= sliceCount; ++stack)
		{
			float phi0 = static_cast<float>(stack + 0) * stackStep;
			float phi1 = static_cast<float>(stack + 1) * stackStep;

			CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
			vertices_[vertexCount++] = VertexPositionColor3D(
				Vec3f(
					radius * MathModule::Cos(phi0) * MathModule::Cos(theta),
					radius * MathModule::Sin(phi0),
					radius * MathModule::Cos(phi0) * MathModule::Sin(theta)
				),
				color
			);

			CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
			vertices_[vertexCount++] = VertexPositionColor3D(
				Vec3f(
					radius * MathModule::Cos(phi1) * MathModule::Cos(theta),
					radius * MathModule::Sin(phi1),
					radius * MathModule::Cos(phi1) * MathModule::Sin(theta)
				),
				color
			);
		}
	}

	DrawGeometry(world, view_, projection_, EDrawMode::LINES, vertexCount, true);
}

void GeometryRenderer::DrawGrid3D(const Vec3f& extensions, float stride)
{
	CHECK(stride >= 1.0f);

	float minXPosition = -extensions.x * 0.5f;
	float maxXPosition = +extensions.x * 0.5f;
	float minYPosition = -extensions.y * 0.5f;
	float maxYPosition = +extensions.y * 0.5f;
	float minZPosition = -extensions.z * 0.5f;
	float maxZPosition = +extensions.z * 0.5f;

	int32_t vertexCount = 0;
	for (float x = minXPosition; x <= maxXPosition; x += stride)
	{
		Vec4f color = MathModule::NearZero(x) ? Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : Vec4f(1.0f, 1.0f, 1.0f, 0.4f);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, 0.0f, minZPosition), color);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(x, 0.0f, maxZPosition), color);
	}

	for (float z = minZPosition; z <= maxZPosition; z += stride)
	{
		Vec4f color = MathModule::NearZero(z) ? Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : Vec4f(1.0f, 1.0f, 1.0f, 0.4f);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(minXPosition, 0.0f, z), color);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(maxXPosition, 0.0f, z), color);
	}

	CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(0.0f, minYPosition, 0.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f));

	CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
	vertices_[vertexCount++] = VertexPositionColor3D(Vec3f(0.0f, maxYPosition, 0.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f));

	DrawGeometry(Mat4x4::Identity(), view_, projection_, EDrawMode::LINES, static_cast<uint32_t>(vertexCount), true);
}

void GeometryRenderer::DrawGeometry(const Mat4x4& world, const Mat4x4& view, const Mat4x4& projection, const EDrawMode& drawMode, uint32_t vertexCount, bool bEnableDepth)
{
	CHECK(drawMode != EDrawMode::NONE);

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	RenderModule::SetDepthMode(bEnableDepth);
	{
		shader_->Bind();

		const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
		uint32_t bufferByteSize = static_cast<uint32_t>(VertexPositionColor3D::GetStride() * vertices_.size());
		vertexBuffer_->SetBufferData(vertexPtr, bufferByteSize);

		shader_->SetUniform("world", world);
		shader_->SetUniform("view", view);
		shader_->SetUniform("projection", projection);

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