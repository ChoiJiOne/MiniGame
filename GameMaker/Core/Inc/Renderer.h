#pragma once

#include <array>

#include "GameMath.h"
#include "IResource.h"
#include "RenderManager.h"


namespace GameMaker
{
class Camera2D;
class ITexture;
class Shader;
class TTFont;
class VertexBuffer;

/**
 * 2D 렌더링을 수행하는 렌더러로, 좌표계의 기준은 카메라를 중심으로 다음과 같습니다. 
 *     +y
 *     │
 * ────┼────x+
 *     │
 */
class Renderer : public IResource
{
public:
	explicit Renderer();
	virtual ~Renderer();

	DISALLOW_COPY_AND_ASSIGN(Renderer);

	virtual void Release() override;

	void Begin(const Camera2D* camera2D = nullptr);
	void End();

private:
	struct Vertex
	{
		Vertex() noexcept : position(0.0f, 0.0f), uv(0.0f, 0.0f), color(0.0f, 0.0f, 0.0f, 0.0f) {}
		Vertex(Vec2f&& p, Vec2f& t, Vec4f&& c) noexcept : position(p), uv(t), color(c) {}
		Vertex(const Vec2f& p, const Vec2f& t, const Vec4f& c) noexcept : position(p), uv(t), color(c) {}
		Vertex(float x, float y, float u, float v, float r, float g, float b, float a) noexcept : position(x, y), uv(u, v), color(r, g, b, a) {}
		Vertex(Vertex&& instance) noexcept : position(instance.position), uv(instance.uv), color(instance.color) {}
		Vertex(const Vertex& instance) noexcept : position(instance.position), uv(instance.uv), color(instance.color) {}

		Vertex& operator=(Vertex&& instance) noexcept
		{
			if (this == &instance) return *this;

			position = instance.position;
			uv = instance.uv;
			color = instance.color;

			return *this;
		}

		Vertex& operator=(const Vertex& instance) noexcept
		{
			if (this == &instance) return *this;

			position = instance.position;
			uv = instance.uv;
			color = instance.color;

			return *this;
		}

		static uint32_t GetStride()
		{
			return sizeof(Vertex);
		}

		Vec2f position;
		Vec2f uv;
		Vec4f color;
	};

	enum class EType
	{
		GEOMETRY = 0x00,
		STRING  = 0x01,
		SPRITE  = 0x02,
	};

private:
	static const int32_t MAX_VERTEX_SIZE = 20000;
	std::array<Vertex, MAX_VERTEX_SIZE> vertices_;
	
	uint32_t vertexArrayObject_ = 0;
	VertexBuffer* vertexBuffer_ = nullptr;

	bool bIsBegin_ = false;
	Mat4x4 ortho_; /** 직교 투영 행렬 */
};

}