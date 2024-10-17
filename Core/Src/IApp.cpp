#include <algorithm>
#include <map>
#include <windows.h>

/** 서드 파티 라이브러리 사용을 위한 헤더 선언 시작 */
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <SDL2/SDL.h>

#include <glad/glad.h>

#include <mimalloc-new-delete.h>
/** 서드 파티 라이브러리 사용을 위한 헤더 선언 종료*/

#include "Assertion.h"
#include "AudioManager.h"
#include "CrashManager.h"
#include "Config.h"
#include "DebugDrawManager3D.h"
#include "EntityManager.h"
#include "GameTimer.h"
#include "IApp.h"
#include "IGameScene.h"
#include "InputManager.h"
#include "RenderManager2D.h"
#include "RenderManager3D.h"
#include "RenderStateManager.h"
#include "ResourceManager.h"
#include "UIManager.h"

static LPTOP_LEVEL_EXCEPTION_FILTER topLevelExceptionFilter_;
extern LONG WINAPI DetectApplicationCrash(EXCEPTION_POINTERS* ep);

IApp* IApp::instance_ = nullptr;

IApp::IApp(const char* title, int32_t x, int32_t y, int32_t w, int32_t h, bool bIsResizble, bool bIsFullscreen)
{
	topLevelExceptionFilter_ = ::SetUnhandledExceptionFilter(DetectApplicationCrash);
	instance_ = this;

	ASSERT(SDL_SetMemoryFunctions(mi_malloc, mi_calloc, mi_realloc, mi_free) == 0, "%s", SDL_GetError());
	ASSERT(SDL_Init(SDL_INIT_EVERYTHING) == 0, "%s", SDL_GetError());

	RenderStateManager::GetRef().PreStartup();
	CreateAppWindow(title, x, y, w, h, bIsResizble, bIsFullscreen);
	RenderStateManager::GetRef().PostStartup();

	AudioManager::GetRef().Startup();
	RenderManager2D::GetRef().Startup();
	RenderManager3D::GetRef().Startup();
	DebugDrawManager3D::GetRef().Startup();
	UIManager::GetRef().Startup();
	InputManager::GetRef().Startup();
}

IApp::~IApp()
{
	UIManager::GetRef().Shutdown();
	EntityManager::GetRef().Cleanup();
	DebugDrawManager3D::GetRef().Shutdown();
	RenderManager3D::GetRef().Shutdown();
	RenderManager2D::GetRef().Shutdown();
	ResourceManager::GetRef().Cleanup();
	AudioManager::GetRef().Shutdown();
	RenderStateManager::GetRef().Shutdown();

	SDL_DestroyWindow(reinterpret_cast<SDL_Window*>(window_));
	window_ = nullptr;

	SDL_Quit();

	::SetUnhandledExceptionFilter(topLevelExceptionFilter_);
}

void IApp::Run()
{
	GameTimer timer;

	InputManager& inputMgr = InputManager::GetRef();
	
	timer.Reset();
	currentScene_->Enter();
	while (!inputMgr.IsQuit())
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			inputMgr.ProcessPollingEvent(&e);
		}
		else
		{
			inputMgr.UpdateKeyboardState();
			inputMgr.UpdateMouseState();

			timer.Tick();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			currentScene_->Tick(timer.GetDeltaSeconds());
			currentScene_->Render();

			if (currentScene_->IsSceneSwitched())
			{
				currentScene_->Exit();
				currentScene_ = currentScene_->GetSwitchScene();
				currentScene_->Enter();
			}
		}
	}
}

void IApp::AddSceneByName(const std::string& name, IGameScene* scene)
{
	auto it = scenes_.find(name);
	ASSERT(it == scenes_.end(), "Already add '%s'", name.c_str());

	scenes_.insert({ name, scene });
}

void IApp::DeleteScenesByName(const std::string& name)
{
	auto it = scenes_.find(name);
	ASSERT(it != scenes_.end(), "Can't find '%s'.", name.c_str());

	scenes_.erase(it);
}

IApp* IApp::Get()
{
	return instance_;
}

void IApp::CreateAppWindow(const char* title, int32_t x, int32_t y, int32_t w, int32_t h, bool bIsResizble, bool bIsFullscreen)
{
	uint32_t baseFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	baseFlags |= (bIsResizble ? SDL_WINDOW_RESIZABLE : 0);
	baseFlags |= (bIsFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

	SDL_Window* window = SDL_CreateWindow(title, x, y, w, h, baseFlags);
	ASSERT(window != nullptr, "%s", SDL_GetError());

	window_ = window;
	RenderStateManager::GetRef().window_ = window;
}