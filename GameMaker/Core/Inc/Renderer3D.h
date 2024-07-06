#pragma once

#include <array>

#include "GameMath.h"
#include "IResource.h"
#include "RenderManager.h"


namespace GameMaker
{
class Camera3D;
class Shader;
class VertexBuffer;

class Renderer3D : public IResource
{
public:
	explicit Renderer3D();
	virtual ~Renderer3D();

	DISALLOW_COPY_AND_ASSIGN(Renderer3D);

	virtual void Release() override;

	void Begin(const Mat4x4& view, const Mat4x4& projection);
	void Begin(const Camera3D* camera3D);
	void End();

	void DrawPoint(const Vec3f* positions, uint32_t size, const Vec4f& color, float pointSize = 1.0f); /** 3D 점들의 개수는 MAX_VERTEX_SIZE(10000)의 크기를 넘을 수 없습니다. */
	void DrawPoint(const Vec3f* positions, const Vec4f* colors, uint32_t size, float pointSize = 1.0f);
	void DrawLine(const Vec3f* positions, const Vec4f* colors, uint32_t size);
	void DrawLine(const Vec3f& startPos, const Vec3f& endPos, const Vec4f& color);
	void DrawLine(const Vec3f& startPos, const Vec4f& startColor, const Vec3f& endPos, const Vec4f& endColor);
	void DrawLines(const Vec3f* positions, uint32_t size, const Vec4f& color);
	void DrawLines(const Vec3f* positions, const Vec4f* colors, uint32_t size);
	void DrawQuad(const Mat4x4& world, float width, float height, const Vec4f& color);
	void DrawCube(const Mat4x4& world, const Vec3f& extents, const Vec4f& color);
	void DrawSphere(const Mat4x4& world, float radius, const Vec4f& color);
	void DrawViewfrustum(const Mat4x4& view, const Mat4x4& projection, const Vec4f& color);
	void DrawGrid(const Vec3f& extensions, float stride);

private:
	struct Vertex
	{
		Vertex() noexcept : position(0.0f, 0.0f, 0.0f), color(0.0f, 0.0f, 0.0f, 0.0f) {}
		Vertex(Vec3f&& p, Vec4f&& c) noexcept : position(p), color(c) {}
		Vertex(const Vec3f& p, const Vec4f& c) noexcept : position(p), color(c) {}
		Vertex(float x, float y, float z, float r, float g, float b, float a) noexcept : position(x, y, z), color(r, g, b, a) {}
		Vertex(Vertex&& instance) noexcept : position(instance.position), color(instance.color) {}
		Vertex(const Vertex& instance) noexcept : position(instance.position), color(instance.color) {}

		Vertex& operator=(Vertex&& instance) noexcept
		{
			if (this == &instance) return *this;

			position = instance.position;
			color = instance.color;

			return *this;
		}

		Vertex& operator=(const Vertex& instance) noexcept
		{
			if (this == &instance) return *this;

			position = instance.position;
			color = instance.color;

			return *this;
		}

		static uint32_t GetStride()
		{
			return sizeof(Vertex);
		}

		Vec3f position;
		Vec4f color;
	};


private:
	void Draw(const Mat4x4& world, const EDrawMode& drawMode, uint32_t vertexCount);

private:
	static const int32_t MAX_VERTEX_SIZE = 10000;
	static const int32_t MAX_SLICE_SIZE = 7;

	std::array<Vertex, MAX_VERTEX_SIZE> vertices_;
	bool bIsBegin_ = false;
	float pointSize_ = 1.0f;
	uint32_t vertexArrayObject_ = 0;
	VertexBuffer* vertexBuffer_;
	Shader* shader_ = nullptr;
	bool bIsBeforeEnableDepth_ = true;
};

}