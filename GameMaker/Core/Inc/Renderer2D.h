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
 * 2D 렌더링을 수행하는 렌더러로, 좌표계 기준은 다음과 같습니다.
 *     +y
 *     │
 * ────┼────x+
 *     │
 */
class Renderer2D : public IResource
{
public:
	explicit Renderer2D();
	virtual ~Renderer2D();

	DISALLOW_COPY_AND_ASSIGN(Renderer2D);

	virtual void Release() override;

	void Begin(const Camera2D* camera2D = nullptr);
	void End();

	void DrawPoint(const Vec2f* positions, uint32_t size, const Vec4f& color, float pointSize = 1.0f);
	void DrawPoint(const Vec2f* positions, const Vec4f* colors, uint32_t size, float pointSize = 1.0f);
	void DrawLine(const Vec2f* positions, const Vec4f* colors, uint32_t size);
	void DrawLine(const Vec2f& startPos, const Vec2f& endPos, const Vec4f& color);
	void DrawLine(const Vec2f& startPos, const Vec4f& startColor, const Vec2f& endPos, const Vec4f& endColor);
	void DrawLines(const Vec2f* positions, uint32_t size, const Vec4f& color);
	void DrawLines(const Vec2f* positions, const Vec4f* colors, uint32_t size);
	void DrawTriangle(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color);
	void DrawTriangle(const Vec2f& fromPos, const Vec4f& fromColor, const Vec2f& byPos, const Vec4f& byColor, const Vec2f& toPos, const Vec4f& toColor);
	void DrawTriangleWireframe(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color);
	void DrawTriangleWireframe(const Vec2f& fromPos, const Vec4f& fromColor, const Vec2f& byPos, const Vec4f& byColor, const Vec2f& toPos, const Vec4f& toColor);
	void DrawRect(const Vec2f& center, float w, float h, const Vec4f& color, float rotate = 0.0f);
	void DrawRectWireframe(const Vec2f& center, float w, float h, const Vec4f& color, float rotate = 0.0f);
	void DrawRoundRect(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate = 0.0f);
	void DrawRoundRectWireframe(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate = 0.0f);
	void DrawCircle(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount = 300);
	void DrawCircleWireframe(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount = 300);
	void DrawEllipse(const Vec2f& center, float xAxis, float yAxis, const Vec4f& color, float rotate = 0.0f, int32_t sliceCount = 300);
	void DrawEllipseWireframe(const Vec2f& center, float xAxis, float yAxis, const Vec4f& color, float rotate = 0.0f, int32_t sliceCount = 300);
	void DrawString(const TTFont* font, const std::wstring& text, const Vec2f& pos, const Vec4f& color);
	void DrawSprite(const ITexture* texture, const Vec2f& center, float w, float h, float rotate = 0.0f, bool bFlipH = false, bool bFlipV = false);

	/** factor의 값이 0.0 이면 blend 값에 영향을 받지 않고, 1.0이면 blend 색상만 렌더링합니다. */
	void DrawSprite(const ITexture* texture, const Vec2f& center, float w, float h, const Vec4f& blend, float factor = 0.0f, float rotate = 0.0f, bool bFlipH = false, bool bFlipV = false);

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

	enum class EMode
	{
		GEOMETRY = 0x00,
		STRING   = 0x01,
		SPRITE   = 0x02,
	};


private:
	/** 텍스트 영역 설정 시 좌표는 왼쪽 상단 좌표 기준입니다. */
	uint32_t SetGlyphVertexBuffer(const TTFont* font, const std::wstring& text, const Vec2f& pos, const Vec4f& color);
	void Draw(const Mat4x4& transform, const EDrawMode& drawMode, uint32_t vertexCount, const EMode& mode);

private:
	static const int32_t MAX_VERTEX_SIZE = 10000;
	static const int32_t MAX_SLICE_SIZE = 20;
	static const int32_t GLYPH_ATLAS_BIND_SLOT = 0;
	static const int32_t SPRITE_BIND_SLOT = 1;

	std::array<Vertex, MAX_VERTEX_SIZE> vertices_;
	bool bIsBegin_ = false;
	float pointSize_ = 1.0f;
	float factor_ = 0.0f;
	uint32_t vertexArrayObject_ = 0;
	VertexBuffer* vertexBuffer_ = nullptr;
	Shader* shader_ = nullptr;
	bool bIsBeforeEnableDepth_ = true;
	bool bIsBeforeEnableCull_ = false;
};

}
