#pragma once

#include <array>
#include <queue>

#include "GameMath.h"
#include "IApp.h"
#include "IResource.h"

class Camera3D;
class Shader;
class VertexBuffer;

/** 3D 디버그 드로우 매니저는 싱글턴입니다. */
class DebugDrawManager3D
{
public:
	DISALLOW_COPY_AND_ASSIGN(DebugDrawManager3D);

	static DebugDrawManager3D& Get();

	void Begin(const Camera3D* camera3D = nullptr);
	void End();

	void DrawLine(const Vec3f& startPos, const Vec3f& endPos, const Vec4f& color);
	void DrawLine(const Vec3f& startPos, const Vec4f& startColor, const Vec3f& endPos, const Vec4f& endColor);
	void DrawCube(const Mat4x4& world, const Vec3f& extents, const Vec4f& color);
	void DrawViewfrustum(const Mat4x4& view, const Mat4x4& projection, const Vec4f& color);

private:
	friend class IApp;

	DebugDrawManager3D() = default;
	virtual ~DebugDrawManager3D() {}

	struct Vertex
	{
		static uint32_t GetStride()
		{
			return sizeof(Vertex);
		}

		Vec4f position;
		Vec4f color;
	};

	struct RenderCommand
	{
		DrawMode drawMode;
		uint32_t startVertexIndex;
		uint32_t vertexCount;
	};

	void Startup(); /** IApp 내부에서만 사용하는 메서드입니다. */
	void Shutdown(); /** IApp 내부에서만 사용하는 메서드입니다. */

	void Flush();
	bool IsFullCommandQueue(uint32_t vertexCount);

private:
	IApp* app_ = nullptr;

	bool bIsBegin_ = false;

	static const int32_t MAX_VERTEX_BUFFER_SIZE = 1000;
	std::array<Vertex, MAX_VERTEX_BUFFER_SIZE> vertices_;

	uint32_t vertexArrayObject_ = 0;
	VertexBuffer* vertexBuffer_ = nullptr;

	Shader* shader_ = nullptr;
	std::queue<RenderCommand> commandQueue_;

	bool originEnableDepth_ = true;
	bool originEnableCull_ = true;
};