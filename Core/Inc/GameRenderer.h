#pragma once

#include "GameError.h"

enum class EDrawMode : int32_t
{
	POINTS       = 0x0000,
	LINES        = 0x0001,
	LINE_STRIP   = 0x0003,
	TRIANGLES    = 0x0004,
	TRIANGLE_FAN = 0x0006,
	NONE         = 0xFFFF,
};

namespace GameRenderer
{

void BeginFrame(float red, float green, float blue, float alpha, float depth = 1.0f, uint8_t stencil = 0);
void EndFrame();

void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);
void SetWindowViewport();
void SetVsyncMode(bool bIsEnable);
void SetDepthMode(bool bIsEnable);
void SetStencilMode(bool bIsEnable);
void SetAlphaBlendMode(bool bIsEnable);
void SetMultisampleMode(bool bIsEnable);
void SetCullFaceMode(bool bIsEnable);

}