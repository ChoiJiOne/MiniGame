#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)

#include <glad/glad.h>

#include "GLAssertion.h"
#include "Renderer3D.h"
#include "RenderModule.h"
#include "Shader.h"

Renderer3D::Renderer3D()
{
	shader_ = RenderModule::CreateResource<Shader>("Resource/Shader/Geometry3D.vert", "Resource/Shader/Geometry3D.frag");

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

void Renderer3D::DrawPoints3D(const std::vector<Vec3f>& positions, const Vec4f& color, float pointSize)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index] = Vertex(positions[index], color);
	}

	pointSize_ = pointSize;
	DrawGeometry3D(Mat4x4::Identity(), EDrawMode::POINTS, static_cast<uint32_t>(positions.size()));
}

void Renderer3D::DrawConnectPoints3D(const std::vector<Vec3f>& positions, const Vec4f& color)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index] = Vertex(positions[index], color);
	}

	DrawGeometry3D(Mat4x4::Identity(), EDrawMode::LINE_STRIP, static_cast<uint32_t>(positions.size()));
}

void Renderer3D::DrawLine3D(const Vec3f& fromPosition, const Vec3f& toPosition, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(fromPosition, color);
	vertices_[vertexCount++] = Vertex(toPosition, color);

	DrawGeometry3D(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer3D::DrawLine3D(const Vec3f& fromPosition, const Vec4f& fromColor, const Vec3f& toPosition, const Vec4f& toColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(fromPosition, fromColor);
	vertices_[vertexCount++] = Vertex(toPosition, toColor);

	DrawGeometry3D(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer3D::DrawLines3D(const std::vector<Vec3f>& positions, const Vec4f& color)
{
	CHECK(positions.size() <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < positions.size(); ++index)
	{
		vertices_[index] = Vertex(Vec3f(positions[index].x, positions[index].y, positions[index].z), color);
	}

	DrawGeometry3D(Mat4x4::Identity(), EDrawMode::LINES, static_cast<uint32_t>(positions.size()));
}

void Renderer3D::DrawQuad3D(const Mat4x4& world, float width, float height, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec3f(-width * 0.5f, -height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(+width * 0.5f, -height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(+width * 0.5f, +height * 0.5f, 0.0f), color);

	vertices_[vertexCount++] = Vertex(Vec3f(-width * 0.5f, -height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(+width * 0.5f, +height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(-width * 0.5f, +height * 0.5f, 0.0f), color);

	DrawGeometry3D(world, EDrawMode::TRIANGLES, vertexCount);
}

void Renderer3D::DrawHorizonProgressBar3D(const Mat4x4& world, float width, float height, float rate, const Vec4f& color, const Vec4f& bgColor)
{
	rate = MathModule::Clamp<float>(rate, 0.0f, 1.0f);

	float x0 = -width * 0.5f;
	float x1 = +width * 0.5f;
	float y0 = -height * 0.5f;
	float y1 = +height * 0.5f;
	float x = x0 + rate * width;

	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec3f(x0, y0, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f( x, y0, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f( x, y1, 0.0f), color);

	vertices_[vertexCount++] = Vertex(Vec3f(x0, y0, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f( x, y1, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(x0, y1, 0.0f), color);

	vertices_[vertexCount++] = Vertex(Vec3f( x, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = Vertex(Vec3f(x1, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = Vertex(Vec3f(x1, y1, 0.0f), bgColor);

	vertices_[vertexCount++] = Vertex(Vec3f( x, y0, 0.0f), bgColor);
	vertices_[vertexCount++] = Vertex(Vec3f(x1, y1, 0.0f), bgColor);
	vertices_[vertexCount++] = Vertex(Vec3f( x, y1, 0.0f), bgColor);

	DrawGeometry3D(world, EDrawMode::TRIANGLES, vertexCount);
}

void Renderer3D::DrawVerticalProgressBar3D(const Mat4x4& world, float width, float height, float rate, const Vec4f& color, const Vec4f& bgColor)
{
	rate = MathModule::Clamp<float>(rate, 0.0f, 1.0f);

	float x0 = -width * 0.5f;
	float x1 = +width * 0.5f;
	float y0 = -height * 0.5f;
	float y1 = +height * 0.5f;
	float y = y0 + rate * height;

	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec3f(x0, y0, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(x1, y0, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(x1, y, 0.0f), color);

	vertices_[vertexCount++] = Vertex(Vec3f(x0, y0, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(x1, y, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(x0, y, 0.0f), color);

	vertices_[vertexCount++] = Vertex(Vec3f(x0, y, 0.0f), bgColor);
	vertices_[vertexCount++] = Vertex(Vec3f(x1, y, 0.0f), bgColor);
	vertices_[vertexCount++] = Vertex(Vec3f(x1, y1, 0.0f), bgColor);

	vertices_[vertexCount++] = Vertex(Vec3f(x0, y, 0.0f), bgColor);
	vertices_[vertexCount++] = Vertex(Vec3f(x1, y1, 0.0f), bgColor);
	vertices_[vertexCount++] = Vertex(Vec3f(x0, y1, 0.0f), bgColor);

	DrawGeometry3D(world, EDrawMode::TRIANGLES, vertexCount);
}

void Renderer3D::DrawCube3D(const Mat4x4& world, const Vec3f& extents, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	Vec3f minPosition = -extents * 0.5f;
	Vec3f maxPosition = extents * 0.5f;

	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, maxPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, maxPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, maxPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, maxPosition.y, minPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, maxPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, maxPosition.y, minPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, maxPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, maxPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, minPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, minPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, minPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, minPosition.y, minPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, minPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, minPosition.y, minPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, minPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, minPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, maxPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, minPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, maxPosition.y, maxPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, minPosition.y, maxPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, maxPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(minPosition.x, minPosition.y, minPosition.z), color);

	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, maxPosition.y, minPosition.z), color);
	vertices_[vertexCount++] = Vertex(Vec3f(maxPosition.x, minPosition.y, minPosition.z), color);

	DrawGeometry3D(world, EDrawMode::LINES, vertexCount);
}

void Renderer3D::DrawSphere3D(const Mat4x4& world, float radius, const Vec4f& color)
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
			vertices_[vertexCount++] = Vertex(
				Vec3f(
					radius * MathModule::Sin(phi) * MathModule::Cos(theta0),
					radius * MathModule::Cos(phi),
					radius * MathModule::Sin(phi) * MathModule::Sin(theta0)
				),
				color
			);

			CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
			vertices_[vertexCount++] = Vertex(
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
			vertices_[vertexCount++] = Vertex(
				Vec3f(
					radius * MathModule::Cos(phi0) * MathModule::Cos(theta),
					radius * MathModule::Sin(phi0),
					radius * MathModule::Cos(phi0) * MathModule::Sin(theta)
				),
				color
			);

			CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
			vertices_[vertexCount++] = Vertex(
				Vec3f(
					radius * MathModule::Cos(phi1) * MathModule::Cos(theta),
					radius * MathModule::Sin(phi1),
					radius * MathModule::Cos(phi1) * MathModule::Sin(theta)
				),
				color
			);
		}
	}

	DrawGeometry3D(world, EDrawMode::LINES, vertexCount);
}

void Renderer3D::DrawViewfrustum3D(const Mat4x4& view, const Mat4x4& projection, const Vec4f& color)
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

	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(frustumCorners[0], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[1], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[2], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[3], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[4], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[5], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[6], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[7], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[0], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[2], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[1], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[3], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[4], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[6], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[5], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[7], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[0], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[4], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[1], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[5], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[2], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[6], color);

	vertices_[vertexCount++] = Vertex(frustumCorners[3], color);
	vertices_[vertexCount++] = Vertex(frustumCorners[7], color);

	DrawGeometry3D(Mat4x4::Identity(), EDrawMode::LINES, vertexCount);
}

void Renderer3D::DrawGrid3D(const Vec3f& extensions, float stride)
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
		vertices_[vertexCount++] = Vertex(Vec3f(x, 0.0f, minZPosition), color);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = Vertex(Vec3f(x, 0.0f, maxZPosition), color);
	}

	for (float z = minZPosition; z <= maxZPosition; z += stride)
	{
		Vec4f color = MathModule::NearZero(z) ? Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : Vec4f(1.0f, 1.0f, 1.0f, 0.4f);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = Vertex(Vec3f(minXPosition, 0.0f, z), color);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = Vertex(Vec3f(maxXPosition, 0.0f, z), color);
	}

	CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
	vertices_[vertexCount++] = Vertex(Vec3f(0.0f, minYPosition, 0.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f));

	CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
	vertices_[vertexCount++] = Vertex(Vec3f(0.0f, maxYPosition, 0.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f));

	DrawGeometry3D(Mat4x4::Identity(), EDrawMode::LINES, static_cast<uint32_t>(vertexCount));
}

void Renderer3D::DrawGeometry3D(const Mat4x4& world, const EDrawMode& drawMode, uint32_t vertexCount)
{
	CHECK(drawMode != EDrawMode::NONE);

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	RenderModule::SetDepthMode(true);
	{
		shader_->Bind();

		const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
		uint32_t bufferByteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
		vertexBuffer_->SetBufferData(vertexPtr, bufferByteSize);

		shader_->SetUniform("world", world);
		shader_->SetUniform("view", view_);
		shader_->SetUniform("projection", projection_);

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