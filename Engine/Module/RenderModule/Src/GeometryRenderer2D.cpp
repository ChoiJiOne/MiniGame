#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)

#include <glad/glad.h>

#include "GLAssertion.h"
#include "GeometryRenderer2D.h"
#include "RenderModule.h"
#include "Shader.h"

GeometryRenderer2D::GeometryRenderer2D()
{
	shader_ = RenderModule::CreateResource<Shader>("Resource/Shader/Geometry2D.vert", "Resource/Shader/Geometry2D.frag");

	uint32_t byteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	VertexBuffer::EUsage usage = VertexBuffer::EUsage::DYNAMIC;
	vertexBuffer_ = RenderModule::CreateResource<VertexBuffer>(byteSize, usage);

	GL_FAILED(glGenVertexArrays(1, &vertexArrayObject_));
	GL_FAILED(glBindVertexArray(vertexArrayObject_));
	{
		vertexBuffer_->Bind();

		GL_FAILED(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, position))));
		GL_FAILED(glEnableVertexAttribArray(0));

		GL_FAILED(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, color))));
		GL_FAILED(glEnableVertexAttribArray(1));

		vertexBuffer_->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));

	bIsInitialized_ = true;
}

GeometryRenderer2D::~GeometryRenderer2D()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void GeometryRenderer2D::Release()
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

void GeometryRenderer2D::DrawPoints2D(const std::vector<Vec2f>& positions, const Vec4f& color, float pointSize)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);
	CHECK(pointSize >= 0.0f);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x, positions[index].y);
		vertices_[index].color = color;
	}

	pointSize_ = pointSize;
	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::POINTS, static_cast<uint32_t>(positions.size()));
}

void GeometryRenderer2D::DrawConnectPoints2D(const std::vector<Vec2f>& positions, const Vec4f& color)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x, positions[index].y);
		vertices_[index].color = color;
	}

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LINE_STRIP, static_cast<uint32_t>(positions.size()));
}

void GeometryRenderer2D::DrawLine2D(const Vec2f& fromPosition, const Vec2f& toPosition, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec2f(fromPosition.x, fromPosition.y), color);
	vertices_[vertexCount++] = Vertex(Vec2f(  toPosition.x,   toPosition.y), color);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void GeometryRenderer2D::DrawLine2D(const Vec2f& fromPosition, const Vec4f& fromColor, const Vec2f& toPosition, const Vec4f& toColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec2f(fromPosition.x, fromPosition.y), fromColor);
	vertices_[vertexCount++] = Vertex(Vec2f(  toPosition.x,   toPosition.y),   toColor);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void GeometryRenderer2D::DrawLines2D(const std::vector<Vec2f>& positions, const Vec4f& color)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x, positions[index].y);
		vertices_[index].color = color;
	}

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LINES, static_cast<uint32_t>(positions.size()));
}

void GeometryRenderer2D::DrawTriangle2D(const Vec2f& fromPosition, const Vec2f& byPosition, const Vec2f& toPosition, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec2f(fromPosition.x, fromPosition.y), color);
	vertices_[vertexCount++] = Vertex(Vec2f(  byPosition.x,   byPosition.y), color);
	vertices_[vertexCount++] = Vertex(Vec2f(  toPosition.x,   toPosition.y), color);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::TRIANGLES, vertexCount);
}

void GeometryRenderer2D::DrawTriangle2D(const Vec2f& fromPosition, const Vec4f& fromColor, const Vec2f& byPosition, const Vec4f& byColor, const Vec2f& toPosition, const Vec4f& toColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec2f(fromPosition.x, fromPosition.y), fromColor);
	vertices_[vertexCount++] = Vertex(Vec2f(  byPosition.x,   byPosition.y),   byColor);
	vertices_[vertexCount++] = Vertex(Vec2f(  toPosition.x,   toPosition.y),   toColor);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::TRIANGLES, vertexCount);
}

void GeometryRenderer2D::DrawWireframeTriangle2D(const Vec2f& fromPosition, const Vec2f& byPosition, const Vec2f& toPosition, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec2f(fromPosition.x, fromPosition.y), color);
	vertices_[vertexCount++] = Vertex(Vec2f(  byPosition.x,   byPosition.y), color);
	vertices_[vertexCount++] = Vertex(Vec2f(  toPosition.x,   toPosition.y), color);
	vertices_[vertexCount++] = Vertex(Vec2f(fromPosition.x, fromPosition.y), color);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void GeometryRenderer2D::DrawWireframeTriangle2D(const Vec2f& fromPosition, const Vec4f& fromColor, const Vec2f& byPosition, const Vec4f& byColor, const Vec2f& toPosition, const Vec4f& toColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec2f(fromPosition.x, fromPosition.y), fromColor);
	vertices_[vertexCount++] = Vertex(Vec2f(  byPosition.x,   byPosition.y),   byColor);
	vertices_[vertexCount++] = Vertex(Vec2f(  toPosition.x,   toPosition.y),   toColor);
	vertices_[vertexCount++] = Vertex(Vec2f(fromPosition.x, fromPosition.y), fromColor);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void GeometryRenderer2D::DrawRectangle2D(const Vec2f& center, float width, float height, float rotate, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	float halfW = width * 0.5f;
	float halfH = height * 0.5f;

	vertices_[vertexCount++] = Vertex(Vec2f(center.x - halfW, center.y + halfH), color);
	vertices_[vertexCount++] = Vertex(Vec2f(center.x + halfW, center.y + halfH), color);
	vertices_[vertexCount++] = Vertex(Vec2f(center.x + halfW, center.y - halfH), color);
	vertices_[vertexCount++] = Vertex(Vec2f(center.x - halfW, center.y - halfH), color);

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry2D(transform, EDrawMode::TRIANGLE_FAN, vertexCount);
}

void GeometryRenderer2D::DrawWireframeRectangle2D(const Vec2f& center, float width, float height, float rotate, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	float halfW = width * 0.5f;
	float halfH = height * 0.5f;

	vertices_[vertexCount++] = Vertex(Vec2f(center.x - halfW, center.y + halfH), color);
	vertices_[vertexCount++] = Vertex(Vec2f(center.x + halfW, center.y + halfH), color);
	vertices_[vertexCount++] = Vertex(Vec2f(center.x + halfW, center.y - halfH), color);
	vertices_[vertexCount++] = Vertex(Vec2f(center.x - halfW, center.y - halfH), color);
	vertices_[vertexCount++] = Vertex(Vec2f(center.x - halfW, center.y + halfH), color);

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry2D(transform, EDrawMode::LINE_STRIP, vertexCount);
}

void GeometryRenderer2D::DrawRoundRectangle2D(const Vec2f& center, float width, float height, float side, float rotate, const Vec4f& color)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	side = MathModule::Min<float>(side, MathModule::Min<float>(halfWidth, halfHeight));

	uint32_t vertexCount = 0;
	vertices_[vertexCount++].position = Vec2f(center.x, center.y);

	auto computeSpline = [&](const Vec2f& startPosition, const Vec2f& endPosition, const Vec2f& controlPosition)
		{
			for (int32_t slice = 0; slice <= MAX_SLICE_SIZE; ++slice)
			{
				float t = static_cast<float>(slice) / static_cast<float>(MAX_SLICE_SIZE);
				Vec2f p = startPosition * (1.0f - t) + controlPosition * t;
				Vec2f q = controlPosition * (1.0f - t) + endPosition * t;
				Vec2f r = p * (1.0f - t) + q * t;
				vertices_[vertexCount++].position = Vec2f(r.x, r.y);
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
	vertices_[vertexCount++].position = Vec2f(control.x + side, control.y);

	for (int32_t index = 0; index < vertexCount; ++index)
	{
		vertices_[index].color = color;
	}

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry2D(transform, EDrawMode::TRIANGLE_FAN, vertexCount);
}

void GeometryRenderer2D::DrawWireframeRoundRectangle2D(const Vec2f& center, float width, float height, float side, float rotate, const Vec4f& color)
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
				vertices_[vertexCount++].position = Vec2f(r.x, r.y);
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
	vertices_[vertexCount++].position = Vec2f(control.x + side, control.y);

	for (int32_t index = 0; index < vertexCount; ++index)
	{
		vertices_[index].color = color;
	}

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry2D(transform, EDrawMode::LINE_STRIP, vertexCount);
}

void GeometryRenderer2D::DrawCircle2D(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount)
{
	CHECK(radius >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	for (int32_t slice = 1; slice <= sliceCount; ++slice)
	{
		float radian = (static_cast<float>(slice - 1) * TwoPi) / static_cast<float>(sliceCount);
		float x = radius * MathModule::Cos(radian);
		float y = radius * MathModule::Sin(radian);

		vertices_[slice] = Vertex(Vec2f(center.x + x, center.y + y), color);
	}

	vertices_[0] = Vertex(Vec2f(center.x, center.y), color);
	vertices_[sliceCount + 1] = vertices_[1];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 2);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::TRIANGLE_FAN, vertexCount);
}

void GeometryRenderer2D::DrawWireframeCircle2D(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount)
{
	CHECK(radius >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE);

	for (int32_t index = 0; index < sliceCount; ++index)
	{
		float radian = (static_cast<float>(index) * TwoPi) / static_cast<float>(sliceCount);
		float x = radius * MathModule::Cos(radian);
		float y = radius * MathModule::Sin(radian);

		vertices_[index] = Vertex(Vec2f(center.x + x, center.y + y), color);
	}

	vertices_[sliceCount] = vertices_[0];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 1);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void GeometryRenderer2D::DrawEllipse2D(const Vec2f& center, float xAxis, float yAxis, float rotate, const Vec4f& color, int32_t sliceCount)
{
	CHECK(xAxis >= 0.0f && yAxis >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	float halfXAxis = xAxis * 0.5f;
	float haflYAxis = yAxis * 0.5f;

	for (int32_t slice = 1; slice <= sliceCount; ++slice)
	{
		float radian = (static_cast<float>(slice - 1) * TwoPi) / static_cast<float>(sliceCount);
		float x = halfXAxis * MathModule::Cos(radian);
		float y = haflYAxis * MathModule::Sin(radian);

		vertices_[slice] = Vertex(Vec2f(center.x + x, center.y + y), color);
	}

	vertices_[0] = Vertex(Vec2f(center.x, center.y), color);
	vertices_[sliceCount + 1] = vertices_[1];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 2);

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry2D(transform, EDrawMode::TRIANGLE_FAN, vertexCount);
}

void GeometryRenderer2D::DrawWireframeEllipse2D(const Vec2f& center, float xAxis, float yAxis, float rotate, const Vec4f& color, int32_t sliceCount)
{
	CHECK(xAxis >= 0.0f && yAxis >= 0.0f);
	CHECK(sliceCount <= MAX_VERTEX_SIZE - 2);

	float halfXAxis = xAxis * 0.5f;
	float haflYAxis = yAxis * 0.5f;

	for (int32_t index = 0; index < sliceCount; ++index)
	{
		float radian = (static_cast<float>(index) * TwoPi) / static_cast<float>(sliceCount);
		float x = halfXAxis * MathModule::Cos(radian);
		float y = haflYAxis * MathModule::Sin(radian);

		vertices_[index] = Vertex(Vec2f(center.x + x, center.y + y), color);
	}

	vertices_[sliceCount] = vertices_[0];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 1);

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	DrawGeometry2D(transform, EDrawMode::LINE_STRIP, vertexCount);
}

void GeometryRenderer2D::DrawHorizonProgressBar2D(const Vec2f& center, float width, float height, float rate, const Vec4f& color, const Vec4f& bgColor)
{
	rate = MathModule::Clamp<float>(rate, 0.0f, 1.0f);

	float x0 = -width * 0.5f;
	float x1 = +width * 0.5f;
	float y0 = -height * 0.5f;
	float y1 = +height * 0.5f;
	float x = x0 + rate * width;

	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec2f(x0, y0), color);
	vertices_[vertexCount++] = Vertex(Vec2f( x, y0), color);
	vertices_[vertexCount++] = Vertex(Vec2f( x, y1), color);

	vertices_[vertexCount++] = Vertex(Vec2f(x0, y0), color);
	vertices_[vertexCount++] = Vertex(Vec2f( x, y1), color);
	vertices_[vertexCount++] = Vertex(Vec2f(x0, y1), color);

	vertices_[vertexCount++] = Vertex(Vec2f( x, y0), bgColor);
	vertices_[vertexCount++] = Vertex(Vec2f(x1, y0), bgColor);
	vertices_[vertexCount++] = Vertex(Vec2f(x1, y1), bgColor);

	vertices_[vertexCount++] = Vertex(Vec2f( x, y0), bgColor);
	vertices_[vertexCount++] = Vertex(Vec2f(x1, y1), bgColor);
	vertices_[vertexCount++] = Vertex(Vec2f( x, y1), bgColor);

	Mat4x4 transform = Mat4x4::Translation(center.x, center.y, 0.0f);

	DrawGeometry2D(transform, EDrawMode::TRIANGLES, vertexCount);
}

void GeometryRenderer2D::DrawVerticalProgressBar2D(const Vec2f& center, float width, float height, float rate, const Vec4f& color, const Vec4f& bgColor)
{
	rate = MathModule::Clamp<float>(rate, 0.0f, 1.0f);

	float x0 = -width * 0.5f;
	float x1 = +width * 0.5f;
	float y0 = -height * 0.5f;
	float y1 = +height * 0.5f;
	float y = y1 - rate * height;

	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec2f(x0, y0), bgColor);
	vertices_[vertexCount++] = Vertex(Vec2f(x1, y0), bgColor);
	vertices_[vertexCount++] = Vertex(Vec2f(x1, y), bgColor);

	vertices_[vertexCount++] = Vertex(Vec2f(x0, y0), bgColor);
	vertices_[vertexCount++] = Vertex(Vec2f(x1,  y), bgColor);
	vertices_[vertexCount++] = Vertex(Vec2f(x0,  y), bgColor);

	vertices_[vertexCount++] = Vertex(Vec2f(x0,  y), color);
	vertices_[vertexCount++] = Vertex(Vec2f(x1,  y), color);
	vertices_[vertexCount++] = Vertex(Vec2f(x1, y1), color);

	vertices_[vertexCount++] = Vertex(Vec2f(x0,  y), color);
	vertices_[vertexCount++] = Vertex(Vec2f(x1, y1), color);
	vertices_[vertexCount++] = Vertex(Vec2f(x0, y1), color);

	Mat4x4 transform = Mat4x4::Translation(center.x, center.y, 0.0f);

	DrawGeometry2D(transform, EDrawMode::TRIANGLES, vertexCount);
}

void GeometryRenderer2D::DrawGeometry2D(const Mat4x4& transform, const EDrawMode& drawMode, uint32_t vertexCount)
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