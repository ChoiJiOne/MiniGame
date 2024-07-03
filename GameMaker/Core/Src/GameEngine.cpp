#include <SDL2/SDL.h>

#include "Assertion.h"
#include "AudioManager.h"
#include "Config.h"
#include "CrashUtils.h"
#include "EntityManager.h"
#include "GameEngine.h"
#include "GameTimer.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"

using namespace GameMaker;

bool GameEngine::bIsInit_ = false;
void* GameEngine::window_ = nullptr;
bool GameEngine::bShouldCloseWindow_ = false;
std::function<void()> GameEngine::endLoopCallback_ = nullptr;
std::function<void(float)> GameEngine::frameCallback_ = nullptr;

LPTOP_LEVEL_EXCEPTION_FILTER topLevelExceptionFilter;

void GameEngine::Init(const WindowParam& param)
{
	ASSERT(!bIsInit_, "GameEngine has already been initialized.");

	topLevelExceptionFilter = ::SetUnhandledExceptionFilter(CrashUtils::DetectApplicationCrash);

	InitSubSystem();

	uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	if (param.bIsResizble)
	{
		flags |= SDL_WINDOW_RESIZABLE;
	}

	if (param.bIsFullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	window_ = SDL_CreateWindow(param.title.c_str(), param.x, param.y, param.w, param.h, flags);
	CHECK(window_ != nullptr);

	InputManager::Get().Startup();
	AudioManager::Get().Startup();
	ResourceManager::Get().Startup();
	RenderManager::Get().Startup();
	EntityManager::Get().Startup();

	bIsInit_ = true;
}

void GameEngine::Shutdown()
{
	ASSERT(bIsInit_, "GameEngine has already shutdown or initialization has failed.");

	EntityManager::Get().Shutdown();
	ResourceManager::Get().Shutdown();
	RenderManager::Get().Shutdown();
	AudioManager::Get().Shutdown();
	InputManager::Get().Shutdown();

	if (window_)
	{
		SDL_Window* window = reinterpret_cast<SDL_Window*>(window_);
		SDL_DestroyWindow(window);

		window_ = nullptr;
	}

	SDL_Quit();

	::SetUnhandledExceptionFilter(topLevelExceptionFilter);

	bIsInit_ = false;
}

void GameEngine::RunLoop(const std::function<void(float)>& callback)
{
	frameCallback_ = callback;

	InputManager& inputManager = InputManager::Get();

	GameTimer globalTimer;
	globalTimer.Reset();

	while (!bShouldCloseWindow_)
	{
		inputManager.Tick();

		globalTimer.Tick();
		float deltaSeconds = globalTimer.GetDeltaSeconds();

		if (frameCallback_)
		{
			frameCallback_(deltaSeconds);
		}
	}

	if (bShouldCloseWindow_ && endLoopCallback_)
	{
		endLoopCallback_();
	}
}

void GameEngine::InitSubSystem()
{
	SDL_FAILED(SDL_Init(SDL_INIT_EVERYTHING));

	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_MAJOR_VERSION));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_MINOR_VERSION));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_RED_SIZE, GL_RED_SIZE));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, GL_GREEN_SIZE));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, GL_BLUE_SIZE));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, GL_ALPHA_SIZE));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, GL_DEPTH_SIZE));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, GL_STENCIL_SIZE));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_DOUBLEBUFFER));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, GL_MULTISAMPLEBUFFERS));
	SDL_FAILED(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, GL_MULTISAMPLESAMPLES));
}
