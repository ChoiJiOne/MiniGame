#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Collision2D.h"
#include "GameEngine.h"
#include "GameMath.h"
#include "InputManager.h"
#include "Renderer2D.h"
#include "RenderManager.h"


int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameMaker::WindowParam param { "CoinDash3D", 100, 100, 800, 600, false, false };
	
	GameMaker::GameEngine::Init(param);

	GameMaker::Renderer2D* renderer2d = GameMaker::RenderManager::Get().GetRenderer2D();

	std::vector<GameMaker::Vec2f> positions;
	std::vector<GameMaker::Vec4f> colors;

	for (int32_t x = -1000; x <= 1000; x += 10)
	{
		positions.push_back(GameMaker::Vec2f(static_cast<float>(x), -1000.0f));
		positions.push_back(GameMaker::Vec2f(static_cast<float>(x), +1000.0f));

		if (x == 0)
		{
			colors.push_back(GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
			colors.push_back(GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
		}
		else
		{
			colors.push_back(GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 0.5f));
			colors.push_back(GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}

	for (int32_t y = -1000; y <= 1000; y += 10)
	{
		positions.push_back(GameMaker::Vec2f(-1000.0f, static_cast<float>(y)));
		positions.push_back(GameMaker::Vec2f(+1000.0f, static_cast<float>(y)));

		if (y == 0)
		{
			colors.push_back(GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
			colors.push_back(GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			colors.push_back(GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 0.5f));
			colors.push_back(GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}

	GameMaker::OrientedRect2D orientedRect(GameMaker::Vec2f(0.0f, 0.0f), GameMaker::Vec2f(200.0f, 100.0f), GameMaker::ToRadian(30.0f));
	GameMaker::Circle2D circle(GameMaker::Vec2f(200.0f, 200.0f), 50.0f);
	GameMaker::Point2D point;

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds) 
		{
			float w = 0.0f;
			float h = 0.0f;
			GameMaker::RenderManager::Get().GetScreenSize<float>(w, h);
			GameMaker::Vec2i mousePos = GameMaker::InputManager::Get().GetCurrMousePos();
			GameMaker::Vec2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

			point.center = GameMaker::Vec2f(
				-w * 0.5f + mousePosF.x,
				+h * 0.5f - mousePosF.y
			);

			orientedRect.rotate += deltaSeconds;

			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer2d->Begin();
			{
				renderer2d->DrawLines(positions.data(), colors.data(), positions.size());
				renderer2d->DrawPoint(&point.center, 1, GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f), 5.0f);
				renderer2d->DrawCircle(circle.center, circle.radius, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				renderer2d->DrawRect(orientedRect.center, orientedRect.size.x, orientedRect.size.y, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f), orientedRect.rotate);
			}
			renderer2d->End();


			GameMaker::RenderManager::Get().EndFrame();
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}