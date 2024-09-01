#pragma once

#include <array>
#include <map>
#include <queue>

#include "GameMath.h"
#include "IApp.h"
#include "IResource.h"

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
	DISALLOW_COPY_AND_ASSIGN(RenderManager2D);

	static RenderManager2D& Get();

	void Begin(const Camera2D* camera2D = nullptr);
	void End();

	void DrawPoint(const GameMath::Vec2f& point, const GameMath::Vec4f& color, float pointSize = 1.0f);
	void DrawLine(const GameMath::Vec2f& startPos, const GameMath::Vec2f& endPos, const GameMath::Vec4f& color);
	void DrawLine(const GameMath::Vec2f& startPos, const GameMath::Vec4f& startColor, const GameMath::Vec2f& endPos, const GameMath::Vec4f& endColor);
	void DrawTriangle(const GameMath::Vec2f& fromPos, const GameMath::Vec2f& byPos, const GameMath::Vec2f& toPos, const GameMath::Vec4f& color);
	void DrawTriangle(const GameMath::Vec2f& fromPos, const GameMath::Vec4f& fromColor, const GameMath::Vec2f& byPos, const GameMath::Vec4f& byColor, const GameMath::Vec2f& toPos, const GameMath::Vec4f& toColor);
	void DrawTriangleWireframe(const GameMath::Vec2f& fromPos, const GameMath::Vec2f& byPos, const GameMath::Vec2f& toPos, const GameMath::Vec4f& color);
	void DrawTriangleWireframe(const GameMath::Vec2f& fromPos, const GameMath::Vec4f& fromColor, const GameMath::Vec2f& byPos, const GameMath::Vec4f& byColor, const GameMath::Vec2f& toPos, const GameMath::Vec4f& toColor);
	void DrawRect(const GameMath::Vec2f& center, float w, float h, const GameMath::Vec4f& color, float rotate = 0.0f);
	void DrawRectWireframe(const GameMath::Vec2f& center, float w, float h, const GameMath::Vec4f& color, float rotate = 0.0f);
	void DrawRoundRect(const GameMath::Vec2f& center, float w, float h, float side, const GameMath::Vec4f& color, float rotate = 0.0f);
	void DrawRoundRectWireframe(const GameMath::Vec2f& center, float w, float h, float side, const GameMath::Vec4f& color, float rotate = 0.0f);
	void DrawCircle(const GameMath::Vec2f& center, float radius, const GameMath::Vec4f& color);
	void DrawCircleWireframe(const GameMath::Vec2f& center, float radius, const GameMath::Vec4f& color);

	void DrawSprite(ITexture* texture, const GameMath::Vec2f& center, float w, float h, float rotate = 0.0f, bool bFlipH = false, bool bFlipV = false);
	void DrawSprite(ITexture* texture, const GameMath::Vec2f& center, float w, float h, const GameMath::Vec3f& blend, float factor = 0.0f, float rotate = 0.0f, bool bFlipH = false, bool bFlipV = false);
	
	void DrawString(TTFont* font, const std::wstring& text, const GameMath::Vec2f& pos, const GameMath::Vec4f& color);
	
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

		GameMath::Vec2f position;
		GameMath::Vec2f uv;
		GameMath::Vec4f color;
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

		EDrawMode drawMode;
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