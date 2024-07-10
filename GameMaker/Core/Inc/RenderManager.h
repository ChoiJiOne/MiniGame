#pragma once

#include "IManager.h"


namespace GameMaker
{
class Renderer2D;
class Renderer3D;

enum class EDrawMode : int32_t
{
	POINTS       = 0x0000,
	LINES        = 0x0001,
	LINE_STRIP   = 0x0003,
	TRIANGLES    = 0x0004,
	TRIANGLE_FAN = 0x0006,
	NONE         = 0xFFFF,
};

class RenderManager : public IManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(RenderManager);

	static RenderManager& Get();

	virtual void Startup() override;
	virtual void Shutdown() override;

	void BeginFrame(float red, float green, float blue, float alpha, float depth = 1.0f, uint8_t stencil = 0);
	void EndFrame();

	void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);
	void SetWindowViewport();
	void SetVsyncMode(bool bIsEnable);
	void SetDepthMode(bool bIsEnable);
	void SetStencilMode(bool bIsEnable);
	void SetAlphaBlendMode(bool bIsEnable);
	void SetMultisampleMode(bool bIsEnable);
	void SetPointSizeMode(bool bIsEnable);
	void SetCullFaceMode(bool bIsEnable);

	void ExecuteDrawVertex(uint32_t vertexCount, const EDrawMode& drawMode);
	void ExecuteDrawVertexInstanced(uint32_t vertexCount, const EDrawMode& drawMode, uint32_t instanceCounts);
	void ExecuteDrawIndex(uint32_t indexCount, const EDrawMode& drawMode);
	void ExecuteDrawIndexInstanced(uint32_t indexCount, const EDrawMode& drawMode, uint32_t instanceCounts);

	void* GetContext() { return context_; }

	template <typename T>
	void GetScreenSize(T& outWidth, T& outHeight);

	Renderer2D* GetRenderer2D() { return renderer2D_; }
	Renderer3D* GetRenderer3D() { return renderer3D_; }

private:
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(RenderManager);

private:
	void* window_ = nullptr;
	void* context_ = nullptr; /** OpenGL Context. */
	Renderer2D* renderer2D_ = nullptr;
	Renderer3D* renderer3D_ = nullptr;
};

}