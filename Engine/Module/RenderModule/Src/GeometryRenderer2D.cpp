#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)

#include <glad/glad.h>

#include "MathModule.h"

#include "GeometryRenderer2D.h"
#include "GLAssertion.h"

GeometryRenderer2D::GeometryRenderer2D() 
	: Shader("Resource/Shader/Geometry2D.vert", "Resource/Shader/Geometry2D.frag")
	, vertexBuffer_(static_cast<uint32_t>(VertexPositionColor2D::GetStride() * vertices_.size()), VertexBuffer::EUsage::Dynamic)
{
	GL_FAILED(glGenVertexArrays(1, &vertexArrayObject_));
	GL_FAILED(glBindVertexArray(vertexArrayObject_));

	vertexBuffer_.Bind();
	{
		GL_FAILED(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, VertexPositionColor2D::GetStride(), (void*)(offsetof(VertexPositionColor2D, position))));
		GL_FAILED(glEnableVertexAttribArray(0));

		GL_FAILED(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexPositionColor2D::GetStride(), (void*)(offsetof(VertexPositionColor2D, color))));
		GL_FAILED(glEnableVertexAttribArray(1));
	}
	vertexBuffer_.Unbind();

	GL_FAILED(glBindVertexArray(0));
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

	GL_FAILED(glDeleteVertexArrays(1, &vertexArrayObject_));

	Shader::Release();
}

void GeometryRenderer2D::DrawPoints2D(const std::vector<Vec2f>& positions, const Vec4f& color, float pointSize)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);
	CHECK(pointSize >= 0.0f);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.5f, positions[index].y + 0.5f);
		vertices_[index].color = color;
	}

	pointSize_ = pointSize;
	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::Points, static_cast<uint32_t>(positions.size()));
}

void GeometryRenderer2D::DrawConnectPoints2D(const std::vector<Vec2f>& positions, const Vec4f& color)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.5f, positions[index].y + 0.5f);
		vertices_[index].color = color;
	}

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LineStrip, static_cast<uint32_t>(positions.size()));
}

void GeometryRenderer2D::DrawLine2D(const Vec2f& fromPosition, const Vec2f& toPosition, const Vec4f& color)
{
	vertices_[0] = VertexPositionColor2D(Vec2f(fromPosition.x + 0.5f, fromPosition.y + 0.5f), color);
	vertices_[1] = VertexPositionColor2D(Vec2f(  toPosition.x + 0.5f,   toPosition.y + 0.5f), color);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LineStrip, 2);
}

void GeometryRenderer2D::DrawLine2D(const Vec2f& fromPosition, const Vec4f& fromColor, const Vec2f& toPosition, const Vec4f& toColor)
{
	vertices_[0] = VertexPositionColor2D(Vec2f(fromPosition.x + 0.5f, fromPosition.y + 0.5f), fromColor);
	vertices_[1] = VertexPositionColor2D(Vec2f(  toPosition.x + 0.5f,   toPosition.y + 0.5f),   toColor);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LineStrip, 2);
}

void GeometryRenderer2D::DrawLines2D(const std::vector<Vec2f>& positions, const Vec4f& color)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index].position = Vec2f(positions[index].x + 0.5f, positions[index].y + 0.5f);
		vertices_[index].color = color;
	}

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::Lines, static_cast<uint32_t>(positions.size()));
}

void GeometryRenderer2D::DrawTriangle2D(const Vec2f& fromPosition, const Vec2f& byPosition, const Vec2f& toPosition, const Vec4f& color)
{
	vertices_[0] = VertexPositionColor2D(Vec2f(fromPosition.x + 0.5f, fromPosition.y + 0.5f), color);
	vertices_[1] = VertexPositionColor2D(Vec2f(  byPosition.x + 0.5f,   byPosition.y + 0.5f), color);
	vertices_[2] = VertexPositionColor2D(Vec2f(  toPosition.x + 0.5f,   toPosition.y + 0.5f), color);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::Triangles, 3);
}

void GeometryRenderer2D::DrawTriangle2D(const Vec2f& fromPosition, const Vec4f& fromColor, const Vec2f& byPosition, const Vec4f& byColor, const Vec2f& toPosition, const Vec4f& toColor)
{
	vertices_[0] = VertexPositionColor2D(Vec2f(fromPosition.x + 0.5f, fromPosition.y + 0.5f), fromColor);
	vertices_[1] = VertexPositionColor2D(Vec2f(  byPosition.x + 0.5f,   byPosition.y + 0.5f), byColor);
	vertices_[2] = VertexPositionColor2D(Vec2f(  toPosition.x + 0.5f,   toPosition.y + 0.5f), toColor);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::Triangles, 3);
}

void GeometryRenderer2D::DrawWireframeTriangle2D(const Vec2f& fromPosition, const Vec2f& byPosition, const Vec2f& toPosition, const Vec4f& color)
{
	vertices_[0] = VertexPositionColor2D(Vec2f(fromPosition.x + 0.5f, fromPosition.y + 0.5f), color);
	vertices_[1] = VertexPositionColor2D(Vec2f(  byPosition.x + 0.5f,   byPosition.y + 0.5f), color);
	vertices_[2] = VertexPositionColor2D(Vec2f(  toPosition.x + 0.5f,   toPosition.y + 0.5f), color);
	vertices_[3] = VertexPositionColor2D(Vec2f(fromPosition.x + 0.5f, fromPosition.y + 0.5f), color);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LineStrip, 4);
}

void GeometryRenderer2D::DrawWireframeTriangle2D(const Vec2f& fromPosition, const Vec4f& fromColor, const Vec2f& byPosition, const Vec4f& byColor, const Vec2f& toPosition, const Vec4f& toColor)
{
	vertices_[0] = VertexPositionColor2D(Vec2f(fromPosition.x + 0.5f, fromPosition.y + 0.5f), fromColor);
	vertices_[1] = VertexPositionColor2D(Vec2f(  byPosition.x + 0.5f,   byPosition.y + 0.5f),   byColor);
	vertices_[2] = VertexPositionColor2D(Vec2f(  toPosition.x + 0.5f,   toPosition.y + 0.5f),   toColor);
	vertices_[3] = VertexPositionColor2D(Vec2f(fromPosition.x + 0.5f, fromPosition.y + 0.5f), fromColor);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LineStrip, 4);
}

void GeometryRenderer2D::DrawRectangle2D(const Vec2f& center, float width, float height, float rotate, const Vec4f& color)
{
	vertices_[0] = VertexPositionColor2D(Vec2f(center.x - width / 2.0f + 0.5f, center.y + height / 2.0f + 0.5f), color);
	vertices_[1] = VertexPositionColor2D(Vec2f(center.x + width / 2.0f + 0.5f, center.y + height / 2.0f + 0.5f), color);
	vertices_[2] = VertexPositionColor2D(Vec2f(center.x + width / 2.0f + 0.5f, center.y - height / 2.0f + 0.5f), color);
	vertices_[3] = VertexPositionColor2D(Vec2f(center.x - width / 2.0f + 0.5f, center.y - height / 2.0f + 0.5f), color);

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);
	
	DrawGeometry2D(transform, EDrawMode::TriangleFan, 4);
}

void GeometryRenderer2D::DrawWireframeRectangle2D(const Vec2f& center, float width, float height, float rotate, const Vec4f& color)
{
	vertices_[0] = VertexPositionColor2D(Vec2f(center.x - width / 2.0f + 0.5f, center.y - height / 2.0f + 0.5f), color);
	vertices_[1] = VertexPositionColor2D(Vec2f(center.x - width / 2.0f + 0.5f, center.y + height / 2.0f + 0.5f), color);
	vertices_[2] = VertexPositionColor2D(Vec2f(center.x + width / 2.0f + 0.5f, center.y + height / 2.0f + 0.5f), color);
	vertices_[3] = VertexPositionColor2D(Vec2f(center.x + width / 2.0f + 0.5f, center.y - height / 2.0f + 0.5f), color);
	vertices_[4] = VertexPositionColor2D(Vec2f(center.x - width / 2.0f + 0.5f, center.y - height / 2.0f + 0.5f), color);

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);

	DrawGeometry2D(transform, EDrawMode::LineStrip, 5);
}

void GeometryRenderer2D::DrawRoundRectangle2D(const Vec2f& center, float width, float height, float side, float rotate, const Vec4f& color)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	side = MathModule::Min<float>(side, MathModule::Min<float>(halfWidth, halfHeight));

	uint32_t vertexCount = 0;
	vertices_[vertexCount++].position = center;

	auto computeSpline = [&](const Vec2f& startPosition, const Vec2f& endPosition, const Vec2f& controlPosition)
	{
		for (int32_t slice = 0; slice <= MAX_SLICE_SIZE; ++slice)
		{
			float t = static_cast<float>(slice) / static_cast<float>(MAX_SLICE_SIZE);
			Vec2f p = startPosition * (1.0f - t) + controlPosition * t;
			Vec2f q = controlPosition * (1.0f - t) + endPosition * t;
			Vec2f r = p * (1.0f - t) + q * t;
			vertices_[vertexCount++].position = r;
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

	DrawGeometry2D(transform, EDrawMode::TriangleFan, vertexCount);
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
			vertices_[vertexCount++].position = r;
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

	DrawGeometry2D(transform, EDrawMode::LineStrip, vertexCount);
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

		vertices_[slice] = VertexPositionColor2D(Vec2f(center.x + x + 0.5f, center.y + y + 0.5f), color);
	}

	vertices_[0] = VertexPositionColor2D(Vec2f(center.x + 0.5f, center.y + 0.5f), color);
	vertices_[sliceCount + 1] = vertices_[1];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 2);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::TriangleFan, vertexCount);
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

		vertices_[index] = VertexPositionColor2D(Vec2f(center.x + x + 0.5f, center.y + y + 0.5f), color);
	}

	vertices_[sliceCount] = vertices_[0];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 1);

	DrawGeometry2D(Mat4x4::Identity(), EDrawMode::LineStrip, vertexCount);
}

void GeometryRenderer2D::DrawEllipse2D(const Vec2f& center, float xAxis, float yAxis, float rotate, const Vec4f& color, int32_t sliceCount)
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

		vertices_[slice] = VertexPositionColor2D(Vec2f(center.x + x + 0.5f, center.y + y + 0.5f), color);
	}

	vertices_[0] = VertexPositionColor2D(Vec2f(center.x + 0.5f, center.y + 0.5f), color);
	vertices_[sliceCount + 1] = vertices_[1];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 2);

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);

	DrawGeometry2D(transform, EDrawMode::TriangleFan, vertexCount);
}

void GeometryRenderer2D::DrawWireframeEllipse2D(const Vec2f& center, float xAxis, float yAxis, float rotate, const Vec4f& color, int32_t sliceCount)
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

		vertices_[index] = VertexPositionColor2D(Vec2f(center.x + x + 0.5f, center.y + y + 0.5f), color);
	}

	vertices_[sliceCount] = vertices_[0];
	uint32_t vertexCount = static_cast<uint32_t>(sliceCount + 1);

	Mat4x4 transform = Mat4x4::Translation(-center.x, -center.y, 0.0f) * Mat4x4::RotateZ(rotate) * Mat4x4::Translation(+center.x, +center.y, 0.0f);

	DrawGeometry2D(transform, EDrawMode::LineStrip, vertexCount);
}

void GeometryRenderer2D::DrawGeometry2D(const Mat4x4& transform, const EDrawMode& drawMode, uint32_t vertexCount)
{
	CHECK(drawMode != EDrawMode::None);

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	RenderModule::SetDepthMode(false);
	{
		const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
		uint32_t bufferByteSize = static_cast<uint32_t>(VertexPositionColor2D::GetStride() * vertices_.size());
		vertexBuffer_.SetBufferData(vertexPtr, bufferByteSize);
		
		Shader::Bind();
		if (drawMode == EDrawMode::Points)
		{
			Shader::SetUniform("pointSize", pointSize_);
		}

		Shader::SetUniform("transform", transform);
		Shader::SetUniform("ortho", ortho_);

		GL_FAILED(glBindVertexArray(vertexArrayObject_));
		RenderModule::ExecuteDrawVertex(vertexCount, drawMode);
		GL_FAILED(glBindVertexArray(0));

		Shader::Unbind();
	}
	RenderModule::SetDepthMode(originEnableDepth);
}

#pragma warning(pop)