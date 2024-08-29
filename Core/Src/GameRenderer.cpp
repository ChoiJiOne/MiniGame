#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "GameAssert.h"
#include "GameMaker.h"
#include "GameRenderer.h"

extern SDL_Window* window_;

void GameRenderer::BeginFrame(float red, float green, float blue, float alpha, float depth, uint8_t stencil)
{
	SetWindowViewport();

	glClearColor(red, green, blue, alpha);
	glClearDepth(depth);
	glClearStencil(stencil);
	
	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void GameRenderer::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(window_);
}

void GameRenderer::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	glViewport(x, y, width, height);
}

void GameRenderer::SetWindowViewport()
{
	int32_t w = 0;
	int32_t h = 0;
	GameMaker::GetScreenSize<int32_t>(w, h);
	SetViewport(0, 0, w, h);
}

void GameRenderer::SetVsyncMode(bool bIsEnable)
{
	GAME_ASSERT(SDL_GL_SetSwapInterval(static_cast<int32_t>(bIsEnable)) == 0, "%s", SDL_GetError());
}

void GameRenderer::SetDepthMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_CHECK(glEnable(GL_DEPTH_TEST));
	}
	else
	{
		GL_CHECK(glDisable(GL_DEPTH_TEST));
	}
}

void GameRenderer::SetStencilMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_CHECK(glEnable(GL_STENCIL_TEST));
	}
	else
	{
		GL_CHECK(glDisable(GL_STENCIL_TEST));
	}
}

void GameRenderer::SetAlphaBlendMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_CHECK(glEnable(GL_BLEND));
		GL_CHECK(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO));
	}
	else
	{
		GL_CHECK(glDisable(GL_BLEND));
	}
}

void GameRenderer::SetMultisampleMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_CHECK(glEnable(GL_MULTISAMPLE));
	}
	else
	{
		GL_CHECK(glDisable(GL_MULTISAMPLE));
	}
}

void GameRenderer::SetCullFaceMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_CHECK(glEnable(GL_CULL_FACE));
	}
	else
	{
		GL_CHECK(glDisable(GL_CULL_FACE));
	}
}