#pragma once

#include <array>
#include <map>
#include <queue>

#include "GameMath.h"
#include "IApp.h"
#include "IResource.h"

class Atlas2D;
class Camera2D;
class ITexture;
class Shader;
class TTFont;
class VertexBuffer;

/**
 * - 2D 렌더 매니저는 싱글턴입니다.
 * - 2D 좌표계의 기준은 카메라를 중심으로 다음과 같습니다.
 *     +y
 *     │
 * ────┼────x+
 *     │
 */
class RenderManager2D
{
public:
	struct SpriteRenderOptions
	{
		Vec3f blend; /** 스프라이트와 혼합할 색상 입니다. */
		float factor; /** 스프라이트와 혼합할 색상의 비율입니다. factor의 값이 0이라면 색상이 혼합되지 않습니다. */
		bool bIsFlipH = false; /** 스프라이트를 가로로 뒤집을 지 여부입니다. */
		bool bIsFlipV = false; /** 스프라이트를 세로로 뒤집을 지 여부입니다. */
	};

public:
	DISALLOW_COPY_AND_ASSIGN(RenderManager2D);

	static RenderManager2D& Get();

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

	void DrawSprite(ITexture* texture, const Vec2f& center, float w, float h, float rotate = 0.0f, bool bFlipH = false, bool bFlipV = false);
	void DrawSprite(ITexture* texture, const Vec2f& center, float w, float h, const Vec3f& blend, float factor = 0.0f, float rotate = 0.0f, bool bFlipH = false, bool bFlipV = false);
	void DrawSprite(Atlas2D* atlas, const std::string& name, const Vec2f& center, float w, float h, float rotate = 0.0f, bool bFlipH = false, bool bFlipV = false);
	void DrawSprite(Atlas2D* atlas, const std::string& name, const Vec2f& center, float w, float h, const Vec3f& blend, float factor = 0.0f, float rotate = 0.0f, bool bFlipH = false, bool bFlipV = false);
	
	void DrawString(TTFont* font, const std::wstring& text, const Vec2f& pos, const Vec4f& color);
	
private:
	friend class IApp;

	RenderManager2D() = default;
	virtual ~RenderManager2D() {}

	struct Vertex
	{
		static uint32_t GetStride()
		{
			return sizeof(Vertex);
		}

		Vec2f position;
		Vec2f uv;
		Vec4f color;
		uint32_t unit; /** 텍스처 유닛입니다. */
	};

	struct RenderCommand
	{
		static const uint32_t MAX_TEXTURE_UNIT = 10;

		enum class Type
		{
			GEOMETRY = 0x00,
			STRING   = 0x01,
			SPRITE   = 0x02,
		};

		DrawMode drawMode;
		uint32_t startVertexIndex;
		uint32_t vertexCount;
		Type type;
		ITexture* texture[MAX_TEXTURE_UNIT] = { nullptr, };
		TTFont* font[MAX_TEXTURE_UNIT] = { nullptr, };
	};

	void Startup(); /** IApp 내부에서만 사용하는 메서드입니다. */
	void Shutdown(); /** IApp 내부에서만 사용하는 메서드입니다. */

	void Flush();
	bool IsFullCommandQueue(uint32_t vertexCount);

private:
	IApp* app_ = nullptr;

	bool bIsBegin_ = false;

	static const int32_t MAX_VERTEX_BUFFER_SIZE = 30000;
	std::array<Vertex, MAX_VERTEX_BUFFER_SIZE> vertices_;

	uint32_t vertexArrayObject_ = 0;
	VertexBuffer* vertexBuffer_ = nullptr;

	std::map<RenderCommand::Type, Shader*> shaders_;
	std::queue<RenderCommand> commandQueue_;

	bool originEnableDepth_ = false;
	bool originEnableCull_ = false;
};