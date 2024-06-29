#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include <SDL2/SDL.h>

#include "GameEngine.h"
#include "GameInput.h"
#include "GameTimer.h"
#include "GameMath.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameMaker::GameInput::AddWindowEventAction(GameMaker::EWindowEvent::MOVED, [&]() { SDL_Log("MOVED"); });
	GameMaker::GameInput::AddWindowEventAction(GameMaker::EWindowEvent::FOCUS_LOST, [&]() { SDL_Log("FOCUS_LOST"); });
	GameMaker::GameInput::AddWindowEventAction(GameMaker::EWindowEvent::LEAVE, [&]() { SDL_Log("LEAVE"); });
	GameMaker::GameInput::AddWindowEventAction(GameMaker::EWindowEvent::RESIZED, [&]() { SDL_Log("RESIZED"); });
	GameMaker::GameInput::AddWindowEventAction(GameMaker::EWindowEvent::SIZE_CHANGED, [&]() { SDL_Log("SIZE_CHANGED"); });
	GameMaker::GameInput::AddWindowEventAction(GameMaker::EWindowEvent::CLOSE, [&]() { 
		SDL_Log("CLOSE"); 
		});

	GameMaker::GameEngine::Init();
	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds) 
		{
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}