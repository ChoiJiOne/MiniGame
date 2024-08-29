#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "GameAssert.h"
#include "GameMaker.h"
#include "GameRenderer.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameError error = GameMaker::Startup("02.Triangle", 100, 100, 800, 600);
	GAME_ASSERT(error.GetCode() == ErrorCode::OK, "%s", error.GetMessagePtr());

	GameMaker::RunLoop(
		[](float deltaSeconds) 
		{
			GameRenderer::BeginFrame(1.0f, 1.0f, 1.0f, 1.0f);
			GameRenderer::EndFrame();
		}
	);

	GameMaker::Shutdown();
	return 0;
}