#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "GameEngine.h"
#include "Renderer2D.h"
#include "RenderManager.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	GameMaker::GameEngine::Init({ "07.Rendering2D", 100, 100, 800, 600, false, false });

	GameMaker::Renderer2D* renderer = GameMaker::RenderManager::Get().GetRenderer2D();

	GameMaker::Vec2f center(0.0f, 0.0f);

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds)
		{
			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
			renderer->Begin();
			{
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 400.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 300.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 200.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 0.0f, 1.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 100.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 0.0f), 200.0f, 50.0f, GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 1.0f));

				//renderer->DrawRect(GameMaker::Vec2f(0.0f, -400.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, -300.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, -200.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 0.0f, 1.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, -100.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

				static float time = 0.0f;
				time += deltaSeconds;

				renderer->DrawRect(GameMaker::Vec2f(0.0f, 100.0f), 200.0f, 100.0f, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f), time);
				renderer->DrawRect(GameMaker::Vec2f(0.0f, 50.0f), 200.0f, 100.0f, GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
				renderer->DrawRect(GameMaker::Vec2f(0.0f, 0.0f), 200.0f, 100.0f, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f), -time / 10.0f);
			}
			renderer->End();
			GameMaker::RenderManager::Get().EndFrame();
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}