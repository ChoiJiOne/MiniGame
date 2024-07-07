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
			colors.push_back(GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 0.3f));
			colors.push_back(GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 0.3f));
		}
		else
		{
			colors.push_back(GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 0.3f));
			colors.push_back(GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	for (int32_t y = -1000; y <= 1000; y += 10)
	{
		positions.push_back(GameMaker::Vec2f(-1000.0f, static_cast<float>(y)));
		positions.push_back(GameMaker::Vec2f(+1000.0f, static_cast<float>(y)));

		if (y == 0)
		{
			colors.push_back(GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 0.3f));
			colors.push_back(GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 0.3f));
		}
		else
		{
			colors.push_back(GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 0.3f));
			colors.push_back(GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	//GameMaker::OrientedRect2D orientedRect(GameMaker::Vec2f(0.0f, 0.0f), GameMaker::Vec2f(200.0f, 100.0f), GameMaker::ToRadian(30.0f));
	//GameMaker::Circle2D circle(GameMaker::Vec2f(200.0f, 200.0f), 50.0f);


	//GameMaker::Point2D point(GameMaker::Vec2f(100.0f, 100.0f));
	GameMaker::Line2D line0(GameMaker::Vec2f(-100.0f, -200.0f), GameMaker::Vec2f(+100.0f, +200.0f));
	GameMaker::Line2D line1(GameMaker::Vec2f(-200.0f, -100.0f), GameMaker::Vec2f(-200.0f, +100.0f));
	GameMaker::Line2D line2(GameMaker::Vec2f(-300.0f, -150.0f), GameMaker::Vec2f(+300.0f, -150.0f));
	GameMaker::Circle2D circle(GameMaker::Vec2f(200.0f, 200.0f), 50.0f);
	//GameMaker::Rect2D rect(GameMaker::Vec2f(-200.0f, 200.0f), GameMaker::Vec2f(151.0f, 100.0f));
	//GameMaker::OrientedRect2D orientedRect(GameMaker::Vec2f(200.0f, -100.0f), GameMaker::Vec2f(200.0f, 100.0f), GameMaker::ToRadian(30.0f));

	GameMaker::Point2D mousePoint;

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds) 
		{
			float w = 0.0f;
			float h = 0.0f;
			GameMaker::RenderManager::Get().GetScreenSize<float>(w, h);
			GameMaker::Vec2i mousePos = GameMaker::InputManager::Get().GetCurrMousePos();
			GameMaker::Vec2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

			mousePoint.center = GameMaker::Vec2f(-w * 0.5f + mousePosF.x, +h * 0.5f - mousePosF.y);
			circle.center = mousePoint.center;

			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer2d->Begin();
			{
				renderer2d->DrawLines(positions.data(), colors.data(), positions.size());
				//renderer2d->DrawPoint(&mousePoint.center, 1, GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f), 10.0f);

				renderer2d->DrawCircle(circle.center, circle.radius, GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

				if (line0.Intersect(&circle))
				{
					renderer2d->DrawLine(line0.start, line0.end, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				}
				else
				{
					renderer2d->DrawLine(line0.start, line0.end, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				}

				if (line1.Intersect(&circle))
				{
					renderer2d->DrawLine(line1.start, line1.end, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				}
				else
				{
					renderer2d->DrawLine(line1.start, line1.end, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				}

				if (line2.Intersect(&circle))
				{
					renderer2d->DrawLine(line2.start, line2.end, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				}
				else
				{
					renderer2d->DrawLine(line2.start, line2.end, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				}

			}
			renderer2d->End();


			GameMaker::RenderManager::Get().EndFrame();
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}