#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "GameEngine.h"
#include "RenderManager.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameMaker::WindowParam param { "CoinDash3D", 100, 100, 800, 600, false, false };
	
	GameMaker::GameEngine::Init(param);

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds) 
		{
			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
			GameMaker::RenderManager::Get().EndFrame();
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}