#include <SDL2/SDL.h>

#include "Assertion.h"
#include "GameEngine.h"
#include "GameInput.h"
#include "GameTimer.h"

using namespace GameMaker;

bool GameEngine::bIsInit_ = false;
void* GameEngine::gameWindow_ = nullptr;
bool GameEngine::bIsQuitLoop_ = false;
std::function<void()> GameEngine::endLoopCallback_ = nullptr;
std::function<void(float)> GameEngine::frameCallback_ = nullptr;

#define GL_MAJOR_VERSION      4
#define GL_MINOR_VERSION      6
#define GL_RED_SIZE           8
#define GL_GREEN_SIZE         8
#define GL_BLUE_SIZE          8
#define GL_ALPHA_SIZE         8
#define GL_DEPTH_SIZE         24
#define GL_STENCIL_SIZE       8
#define GL_DOUBLEBUFFER       1
#define GL_MULTISAMPLEBUFFERS 1
#define GL_MULTISAMPLESAMPLES 16

void GameEngine::Init()
{
	ASSERT(!bIsInit_, "GameEngine has already been initialized.");

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

	gameWindow_ = SDL_CreateWindow("CoinDash3D", 100, 100, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	CHECK(gameWindow_ != nullptr);

	bIsInit_ = true;
}

void GameEngine::Shutdown()
{
	ASSERT(bIsInit_, "GameEngine has already shutdown or initialization has failed.");

	if (gameWindow_)
	{
		SDL_Window* window = reinterpret_cast<SDL_Window*>(gameWindow_);
		SDL_DestroyWindow(window);

		gameWindow_ = nullptr;
	}

	SDL_Quit();

	bIsInit_ = false;
}

void GameEngine::RunLoop(const std::function<void(float)>& callback)
{
	frameCallback_ = callback;

	GameTimer timer;
	timer.Reset();

	while (!bIsQuitLoop_)
	{
		GameInput::Tick();

		timer.Tick();
		float deltaSeconds = timer.GetDeltaSeconds();

		if (frameCallback_)
		{
			frameCallback_(deltaSeconds);
		}
	}

	if (bIsQuitLoop_ && endLoopCallback_)
	{
		endLoopCallback_();
	}
}
