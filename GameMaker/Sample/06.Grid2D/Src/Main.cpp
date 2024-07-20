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
	
	GameMaker::GameEngine::Init({ "06.Grid2D", 100, 100, 800, 600, false, false });

	GameMaker::Renderer2D* renderer = GameMaker::RenderManager::Get().GetRenderer2D();

	std::vector<GameMaker::Vec2f> positions;
	std::vector<GameMaker::Vec4f> colors;
	for (float x = -400.0f; x <= 400.0f; x += 10.0f)
	{
		positions.push_back(GameMaker::Vec2f(x, +300.0f));
		positions.push_back(GameMaker::Vec2f(x, -300.0f));

		if (x == 0.0f)
		{
			colors.push_back(GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
			colors.push_back(GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
		}
		else
		{
			colors.push_back(GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f));
			colors.push_back(GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f));
		}
	}

	for (float y = -300.0f; y <= 300.0f; y += 10.0f)
	{
		positions.push_back(GameMaker::Vec2f(+400.0f, y));
		positions.push_back(GameMaker::Vec2f(-400.0f, y));

		if (y == 0.0f)
		{
			colors.push_back(GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
			colors.push_back(GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			colors.push_back(GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f));
			colors.push_back(GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f));
		}
	}

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds)
		{
			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
			renderer->Begin();
			{
				renderer->DrawLines(positions.data(), colors.data(), positions.size());
			}
			renderer->End();
			GameMaker::RenderManager::Get().EndFrame();
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}