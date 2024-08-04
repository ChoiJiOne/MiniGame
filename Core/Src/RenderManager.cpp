#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "Assertion.h"
#include "Config.h"
#include "GameEngine.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "RenderManager.h"
#include "ResourceManager.h"

using namespace GameMaker;

template <>
void RenderManager::GetScreenSize(float& outWidth, float& outHeight)
{
	SDL_Window* window = reinterpret_cast<SDL_Window*>(window_);
	int32_t w = 0;
	int32_t h = 0;
	SDL_GetWindowSize(window, &w, &h);

	outWidth = static_cast<float>(w);
	outHeight = static_cast<float>(h);
}

template <>
void RenderManager::GetScreenSize(int32_t& outWidth, int32_t& outHeight)
{
	SDL_Window* window = reinterpret_cast<SDL_Window*>(window_);
	SDL_GetWindowSize(window, &outWidth, &outHeight);
}

RenderManager& RenderManager::Get()
{
	static RenderManager instance;
	return instance;
}

void RenderManager::Startup()
{
	CHECK(!bIsStartup_);

	window_ = GameEngine::GetGameWindow();
	SDL_Window* currentMainWindow = reinterpret_cast<SDL_Window*>(window_);
	CHECK(window_ != nullptr);

	context_ = SDL_GL_CreateContext(currentMainWindow);
	CHECK(context_ != nullptr);

	SDL_FAILED(SDL_GL_MakeCurrent(currentMainWindow, context_));

	CHECK(gladLoadGLLoader((GLADloadproc)(SDL_GL_GetProcAddress)));

	SetDepthMode(static_cast<bool>(DEPTH_MODE));
	SetAlphaBlendMode(static_cast<bool>(ALPHA_MODE));
	SetPointSizeMode(static_cast<bool>(POINT_SIZE_MODE));
	SetCullFaceMode(static_cast<bool>(CULL_FACE_MODE));

	renderer2D_ = ResourceManager::Get().Create<Renderer2D>();
	renderer3D_ = ResourceManager::Get().Create<Renderer3D>();

	bIsStartup_ = true;
}

void RenderManager::Shutdown()
{
	CHECK(bIsStartup_);

	if (renderer3D_)
	{
		ResourceManager::Get().Destroy(renderer3D_);
		renderer3D_ = nullptr;
	}

	if (renderer2D_)
	{
		ResourceManager::Get().Destroy(renderer2D_);
		renderer2D_ = nullptr;
	}

	if (context_)
	{
		SDL_GL_DeleteContext(context_);
		context_ = nullptr;
	}

	bIsStartup_ = false;
}

void RenderManager::BeginFrame(float red, float green, float blue, float alpha, float depth, uint8_t stencil)
{
	SetWindowViewport();

	glClearColor(red, green, blue, alpha);
	glClearDepth(depth);
	glClearStencil(stencil);

	GL_FAILED(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void RenderManager::EndFrame()
{
	/** 모든 렌더링이 끝나면 ImGui 렌더링 수행. */
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(reinterpret_cast<SDL_Window*>(window_));
}

void RenderManager::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	glViewport(x, y, width, height);
}

void RenderManager::SetWindowViewport()
{
	int32_t w;
	int32_t h;
	GetScreenSize<int32_t>(w, h);
	SetViewport(0, 0, w, h);
}

void RenderManager::SetVsyncMode(bool bIsEnable)
{
	SDL_FAILED(SDL_GL_SetSwapInterval(static_cast<int32_t>(bIsEnable)));
}

void RenderManager::SetDepthMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_DEPTH_TEST));
	}
	else
	{
		GL_FAILED(glDisable(GL_DEPTH_TEST));
	}
}

void RenderManager::SetStencilMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_STENCIL_TEST));
	}
	else
	{
		GL_FAILED(glDisable(GL_STENCIL_TEST));
	}
}

void RenderManager::SetAlphaBlendMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_BLEND));
		GL_FAILED(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO));
	}
	else
	{
		GL_FAILED(glDisable(GL_BLEND));
	}
}

void RenderManager::SetMultisampleMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_MULTISAMPLE));
	}
	else
	{
		GL_FAILED(glDisable(GL_MULTISAMPLE));
	}
}

void RenderManager::SetPointSizeMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_PROGRAM_POINT_SIZE));
	}
	else
	{
		GL_FAILED(glDisable(GL_PROGRAM_POINT_SIZE));
	}
}

void RenderManager::SetCullFaceMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_CULL_FACE));
	}
	else
	{
		GL_FAILED(glDisable(GL_CULL_FACE));
	}
}

void RenderManager::ExecuteDrawVertex(uint32_t vertexCount, const EDrawMode& drawMode)
{
	GL_FAILED(glDrawArrays(static_cast<GLenum>(drawMode), 0, vertexCount));
}

void RenderManager::ExecuteDrawVertexInstanced(uint32_t vertexCount, const EDrawMode& drawMode, uint32_t instanceCounts)
{
	GL_FAILED(glDrawArraysInstanced(static_cast<GLenum>(drawMode), 0, vertexCount, instanceCounts));
}

void RenderManager::ExecuteDrawIndex(uint32_t indexCount, const EDrawMode& drawMode)
{
	GL_FAILED(glDrawElements(static_cast<GLenum>(drawMode), indexCount, GL_UNSIGNED_INT, nullptr));
}

void RenderManager::ExecuteDrawIndexInstanced(uint32_t indexCount, const EDrawMode& drawMode, uint32_t instanceCounts)
{
	GL_FAILED(glDrawElementsInstanced(static_cast<GLenum>(drawMode), indexCount, GL_UNSIGNED_INT, nullptr, instanceCounts));
}