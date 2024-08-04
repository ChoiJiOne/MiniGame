#pragma once

#include <array>
#include <map>
#include <queue>

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
class Renderer2D : public IResource
{
public:
	explicit Renderer2D();
	virtual ~Renderer2D();

	DISALLOW_COPY_AND_ASSIGN(Renderer2D);

	virtual void Release() override;

	void Begin(const Camera2D* camera2D = nullptr);
	void End();

	void DrawPoint(const Vec2f& point, const Vec4f& color, float pointSize = 1.0f);
	void DrawLine(const Vec2f& startPos, const Vec2f& endPos, const Vec4f& color);
	void DrawLine(const Vec2f& startPos, const Vec4f& startColor, const Vec2f& endPos, const Vec4f& endColor);
	void DrawTriangle(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color);
	void DrawTriangle(const Vec2f& fromPos, const Vec4f& fromColor, const Vec2f& byPos, const Vec4f& byColor, const Vec2f& toPos, const Vec4f& toColor);
	void DrawTriangleWireframe(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color);
	void DrawTriangleWireframe(const Vec2f& fromPos, const Vec4f& fromColor, const Vec2f& byPos, const Vec4f& byColor, const Vec2f& toPos, const Vec4f& toColor);
	void DrawRect(const Vec2f& center, float w, float h, const Vec4f& color, float rotate = 0.0f);
	void DrawRectWireframe(const Vec2f& center, float w, float h, const Vec4f& color, float rotate = 0.0f);
	void DrawRoundRect(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate = 0.0f);
	void DrawRoundRectWireframe(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate = 0.0f);
	void DrawCircle(const Vec2f& center, float radius, const Vec4f& color);
	void DrawCircleWireframe(const Vec2f& center, float radius, const Vec4f& color);

	void DrawString(TTFont* font, const std::wstring& text, const Vec2f& pos, const Vec4f& color);
	
	void DrawSprite(ITexture* texture, const Vec2f& center, float w, float h, float rotate = 0.0f, bool bFlipH = false, bool bFlipV = false);
	void DrawSprite(ITexture* texture, const Vec2f& center, float w, float h, const Vec3f& blend, float factor = 0.0f, float rotate = 0.0f, bool bFlipH = false, bool bFlipV = false);

private:
	enum class EType
	{
		GEOMETRY = 0x00,
		STRING   = 0x01,
		SPRITE   = 0x02,
	};

	struct Vertex
	{
		static uint32_t GetStride()
		{
			return sizeof(Vertex);
		}

		Vec2f position;
		Vec2f uv;
		Vec4f color;
	};

	struct RenderCommand
	{
		EDrawMode drawMode;
		uint32_t startVertexIndex;
		uint32_t vertexCount;
		EType type;
		ITexture* texture;
		TTFont* font;
	};

private:
	bool bIsBegin_ = false;

	static const int32_t MAX_VERTEX_SIZE = 20000;
	std::array<Vertex, MAX_VERTEX_SIZE> vertices_;
	
	uint32_t vertexArrayObject_ = 0;
	VertexBuffer* vertexBuffer_ = nullptr;

	std::map<EType, Shader*> shaders_;
	std::queue<RenderCommand> commandQueue_;
};

}