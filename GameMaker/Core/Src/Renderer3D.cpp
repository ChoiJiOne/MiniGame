#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26819)
#pragma warning(disable: 26451)

#include <glad/glad.h>

#include "Assertion.h"
#include "Camera3D.h"
#include "Renderer3D.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "VertexBuffer.h"

using namespace GameMaker;

static RenderManager* renderManager = nullptr;


Renderer3D::Renderer3D()
{
	if (!renderManager)
	{
		renderManager = &RenderManager::Get();
	}

	ResourceManager& resourceManager = ResourceManager::Get();

	std::string vsPath = "GameMaker/Shader/Renderer3D.vert";
	std::string fsPath = "GameMaker/Shader/Renderer3D.frag";
	shader_ = resourceManager.Create<Shader>(vsPath, fsPath);

	uint32_t byteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	VertexBuffer::EUsage usage = VertexBuffer::EUsage::DYNAMIC;
	vertexBuffer_ = resourceManager.Create<VertexBuffer>(byteSize, usage);

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

void Renderer3D::Begin(const Mat4x4& view, const Mat4x4& projection)
{
	CHECK(!bIsBegin_);

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	bIsBeforeEnableDepth_ = static_cast<bool>(originEnableDepth);
	renderManager->SetDepthMode(false);

	shader_->Bind();
	{
		shader_->SetUniform("view", view);
		shader_->SetUniform("projection", projection);
	}
	shader_->Unbind();

	bIsBegin_ = true;
}

void GameMaker::Renderer3D::Begin(const Camera3D* camera3D)
{
	CHECK(!bIsBegin_);
	CHECK(camera3D != nullptr);

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	bIsBeforeEnableDepth_ = static_cast<bool>(originEnableDepth);
	renderManager->SetDepthMode(false);

	shader_->Bind();
	{
		shader_->SetUniform("view", camera3D->GetView());
		shader_->SetUniform("projection", camera3D->GetProjection());
	}
	shader_->Unbind();

	bIsBegin_ = true;
}

void Renderer3D::End()
{
	CHECK(bIsBegin_);

	renderManager->SetDepthMode(bIsBeforeEnableDepth_);

	bIsBegin_ = false;
}

void Renderer3D::DrawPoint(const Vec3f* positions, uint32_t size, const Vec4f& color, float pointSize)
{
	CHECK(size <= MAX_VERTEX_SIZE);
	CHECK(pointSize >= 0.0f);

	for (uint32_t index = 0; index < size; ++index)
	{
		vertices_[index].position = positions[index];
		vertices_[index].color = color;
	}

	pointSize_ = pointSize;
	Draw(Mat4x4::Identity(), EDrawMode::POINTS, size);
}

void Renderer3D::DrawPoint(const Vec3f* positions, const Vec4f* colors, uint32_t size, float pointSize)
{
	CHECK(size <= MAX_VERTEX_SIZE);
	CHECK(pointSize >= 0.0f);

	for (uint32_t index = 0; index < size; ++index)
	{
		vertices_[index].position = positions[index];
		vertices_[index].color = colors[index];
	}

	pointSize_ = pointSize;
	Draw(Mat4x4::Identity(), EDrawMode::POINTS, size);
}

void Renderer3D::DrawLine(const Vec3f* positions, const Vec4f* colors, uint32_t size)
{
	CHECK(size <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < size; ++index)
	{
		vertices_[index].position = positions[index];
		vertices_[index].color = colors[index];
	}

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, size);
}

void Renderer3D::DrawLine(const Vec3f& startPos, const Vec3f& endPos, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = startPos;
	vertices_[vertexCount++].color = color;

	vertices_[vertexCount].position = endPos;
	vertices_[vertexCount++].color = color;

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer3D::DrawLine(const Vec3f& startPos, const Vec4f& startColor, const Vec3f& endPos, const Vec4f& endColor)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount].position = startPos;
	vertices_[vertexCount++].color = startColor;

	vertices_[vertexCount].position = endPos;
	vertices_[vertexCount++].color = endColor;

	Draw(Mat4x4::Identity(), EDrawMode::LINE_STRIP, vertexCount);
}

void Renderer3D::DrawLines(const Vec3f* positions, uint32_t size, const Vec4f& color)
{
	CHECK(size <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < size; ++index)
	{
		vertices_[index].position = positions[index];
		vertices_[index].color = color;
	}

	Draw(Mat4x4::Identity(), EDrawMode::LINES, size);
}

void Renderer3D::DrawLines(const Vec3f* positions, const Vec4f* colors, uint32_t size)
{
	CHECK(size <= MAX_VERTEX_SIZE);

	for (std::size_t index = 0; index < size; ++index)
	{
		vertices_[index].position = positions[index];
		vertices_[index].color = colors[index];
	}

	Draw(Mat4x4::Identity(), EDrawMode::LINES, size);
}

void Renderer3D::DrawQuad(const Mat4x4& world, float width, float height, const Vec4f& color)
{
	uint32_t vertexCount = 0;

	vertices_[vertexCount++] = Vertex(Vec3f(-width * 0.5f, -height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(+width * 0.5f, -height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(+width * 0.5f, +height * 0.5f, 0.0f), color);

	vertices_[vertexCount++] = Vertex(Vec3f(-width * 0.5f, -height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(+width * 0.5f, +height * 0.5f, 0.0f), color);
	vertices_[vertexCount++] = Vertex(Vec3f(-width * 0.5f, +height * 0.5f, 0.0f), color);

	Draw(world, EDrawMode::TRIANGLES, vertexCount);
}

void Renderer3D::DrawCube(const Mat4x4& world, const Vec3f& extents, const Vec4f& color)
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

	Draw(world, EDrawMode::LINES, vertexCount);
}

void Renderer3D::DrawSphere(const Mat4x4& world, float radius, const Vec4f& color)
{
	static const uint32_t sliceCount = 20;

	uint32_t vertexCount = 0;
	float stackStep = PI / static_cast<float>(sliceCount);
	float sliceStep = TWO_PI / static_cast<float>(sliceCount);

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
					radius * Sin(phi) * Cos(theta0),
					radius * Cos(phi),
					radius * Sin(phi) * Sin(theta0)
				),
				color
			);

			CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
			vertices_[vertexCount++] = Vertex(
				Vec3f(
					radius * Sin(phi) * Cos(theta1),
					radius * Cos(phi),
					radius * Sin(phi) * Sin(theta1)
				),
				color
			);
		}
	}

	stackStep = TWO_PI / static_cast<float>(sliceCount);
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
					radius * Cos(phi0) * Cos(theta),
					radius * Sin(phi0),
					radius * Cos(phi0) * Sin(theta)
				),
				color
			);

			CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
			vertices_[vertexCount++] = Vertex(
				Vec3f(
					radius * Cos(phi1) * Cos(theta),
					radius * Sin(phi1),
					radius * Cos(phi1) * Sin(theta)
				),
				color
			);
		}
	}

	Draw(world, EDrawMode::LINES, vertexCount);
}

void Renderer3D::DrawViewfrustum(const Mat4x4& view, const Mat4x4& projection, const Vec4f& color)
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

	Draw(Mat4x4::Identity(), EDrawMode::LINES, vertexCount);
}

void Renderer3D::DrawGrid(const Vec3f& extensions, float stride)
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
		Vec4f color = NearZero(x) ? Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : Vec4f(1.0f, 1.0f, 1.0f, 0.4f);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = Vertex(Vec3f(x, 0.0f, minZPosition), color);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = Vertex(Vec3f(x, 0.0f, maxZPosition), color);
	}

	for (float z = minZPosition; z <= maxZPosition; z += stride)
	{
		Vec4f color = NearZero(z) ? Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : Vec4f(1.0f, 1.0f, 1.0f, 0.4f);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = Vertex(Vec3f(minXPosition, 0.0f, z), color);

		CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
		vertices_[vertexCount++] = Vertex(Vec3f(maxXPosition, 0.0f, z), color);
	}

	CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
	vertices_[vertexCount++] = Vertex(Vec3f(0.0f, minYPosition, 0.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f));

	CHECK(0 <= vertexCount && vertexCount < MAX_VERTEX_SIZE);
	vertices_[vertexCount++] = Vertex(Vec3f(0.0f, maxYPosition, 0.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f));

	Draw(Mat4x4::Identity(), EDrawMode::LINES, static_cast<uint32_t>(vertexCount));
}

void Renderer3D::Draw(const Mat4x4& world, const EDrawMode& drawMode, uint32_t vertexCount)
{
	CHECK(drawMode != EDrawMode::NONE);

	const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
	uint32_t bufferByteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	vertexBuffer_->SetBufferData(vertexPtr, bufferByteSize);

	shader_->Bind();
	{
		shader_->SetUniform("world", world);

		if (drawMode == EDrawMode::POINTS)
		{
			shader_->SetUniform("pointSize", pointSize_);
		}

		GL_FAILED(glBindVertexArray(vertexArrayObject_));
		renderManager->ExecuteDrawVertex(vertexCount, drawMode);
		GL_FAILED(glBindVertexArray(0));
	}
	shader_->Unbind();
}

#pragma warning(pop)