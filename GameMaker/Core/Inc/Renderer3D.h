#pragma once

#include <array>
#include <queue>

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

	void Begin(const Camera3D* camera3D);
	void End();

	void DrawLine(const Vec3f& startPos, const Vec3f& endPos, const Vec4f& color);
	void DrawLine(const Vec3f& startPos, const Vec4f& startColor, const Vec3f& endPos, const Vec4f& endColor);
	//void DrawQuad(const Mat4x4& world, float width, float height, const Vec4f& color);
	//void DrawCube(const Mat4x4& world, const Vec3f& extents, const Vec4f& color);
	//void DrawSphere(const Mat4x4& world, float radius, const Vec4f& color);
	//void DrawViewfrustum(const Mat4x4& view, const Mat4x4& projection, const Vec4f& color);
	//void DrawGrid(const Vec3f& extensions, float stride);

private:
	struct Vertex
	{
		Vertex() noexcept : position(0.0f, 0.0f, 0.0f, 0.0f), color(0.0f, 0.0f, 0.0f, 0.0f) {}
		Vertex(Vec4f&& p, Vec4f&& c) noexcept : position(p), color(c) {}
		Vertex(const Vec4f& p, const Vec4f& c) noexcept : position(p), color(c) {}
		Vertex(float x, float y, float z, float w, float r, float g, float b, float a) noexcept : position(x, y, z, w), color(r, g, b, a) {}
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

		Vec4f position;
		Vec4f color;
	};

	struct RenderCommand
	{
		EDrawMode drawMode;
		uint32_t startVertexIndex;
		uint32_t vertexCount;
	};
	
private:
	bool bIsBegin_ = false;

	static const int32_t MAX_VERTEX_SIZE = 20000;
	std::array<Vertex, MAX_VERTEX_SIZE> vertices_;
	
	uint32_t vertexArrayObject_ = 0;
	VertexBuffer* vertexBuffer_ = nullptr;

	Shader* shader_ = nullptr;
	std::queue<RenderCommand> commandQueue_;
};

}