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


	GameMaker::Point2D mousePoint;

	GameMaker::Line2D line(GameMaker::Vec2f(-100.0f, -200.0f), GameMaker::Vec2f(+100.0f, +200.0f));
	//GameMaker::Circle2D circle(GameMaker::Vec2f(200.0f, 200.0f), 50.0f);
	GameMaker::Rect2D rect(GameMaker::Vec2f(200.0f, -10.0f), GameMaker::Vec2f(200.0f, 100.0f));
	GameMaker::OrientedRect2D orientedRect(GameMaker::Vec2f(-200.0f, -100.0f), GameMaker::Vec2f(200.0f, 100.0f), GameMaker::ToRadian(30.0f));

	GameMaker::Circle2D circle0(GameMaker::Vec2f(0.0f, 0.0f), 50.0f);
	GameMaker::Circle2D circle1(GameMaker::Vec2f(200.0f, 200.0f), 500.0f);

	GameMaker::Rect2D rect0(GameMaker::Vec2f(0.0f, 0.0f), GameMaker::Vec2f(200.0f, 100.0f));
	GameMaker::Rect2D rect1(GameMaker::Vec2f(200.0f, -10.0f), GameMaker::Vec2f(100.0f, 200.0f));

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds) 
		{
			float w = 0.0f;
			float h = 0.0f;
			GameMaker::RenderManager::Get().GetScreenSize<float>(w, h);
			GameMaker::Vec2i mousePos = GameMaker::InputManager::Get().GetCurrMousePos();
			GameMaker::Vec2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

			mousePoint.center = GameMaker::Vec2f(-w * 0.5f + mousePosF.x, +h * 0.5f - mousePosF.y);
			//line.end = mousePoint.center;
			rect0.center = mousePoint.center;

			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer2d->Begin();
			{
				renderer2d->DrawLines(positions.data(), colors.data(), positions.size());

				if (rect0.Intersect(&rect1))
				{
					renderer2d->DrawRect(rect0.center, rect0.size.x, rect0.size.y, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
					renderer2d->DrawRect(rect1.center, rect1.size.x, rect1.size.y, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				}
				else
				{
					renderer2d->DrawRect(rect0.center, rect0.size.x, rect0.size.y, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
					renderer2d->DrawRect(rect1.center, rect1.size.x, rect1.size.y, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				}

				//if (circle0.Intersect(&circle1))
				//{
				//	renderer2d->DrawCircle(circle0.center, circle0.radius, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				//	renderer2d->DrawCircle(circle1.center, circle1.radius, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				//}
				//else
				//{
				//	renderer2d->DrawCircle(circle0.center, circle0.radius, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//	renderer2d->DrawCircle(circle1.center, circle1.radius, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//}

				//if (circle0.Intersect(&rect))
				//{

				//	renderer2d->DrawRect(rect.center, rect.size.x, rect.size.y, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				//	renderer2d->DrawCircle(circle0.center, circle0.radius, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				//}
				//else
				//{
				//	renderer2d->DrawRect(rect.center, rect.size.x, rect.size.y, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//	renderer2d->DrawCircle(circle0.center, circle0.radius, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//}

				//orientedRect.rotate += deltaSeconds;
				//if (circle0.Intersect(&orientedRect))
				//{
				//	renderer2d->DrawRect(orientedRect.center, orientedRect.size.x, orientedRect.size.y, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f), orientedRect.rotate);
				//	renderer2d->DrawCircle(circle0.center, circle0.radius, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				//}
				//else
				//{
				//	renderer2d->DrawRect(orientedRect.center, orientedRect.size.x, orientedRect.size.y, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f), orientedRect.rotate);
				//	renderer2d->DrawCircle(circle0.center, circle0.radius, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//}

				//if (line.Intersect(&rect))
				//{
				//	renderer2d->DrawRect(rect.center, rect.size.x, rect.size.y, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				//	renderer2d->DrawLine(line.start, line.end, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				//}
				//else
				//{
				//	renderer2d->DrawRect(rect.center, rect.size.x, rect.size.y, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//	renderer2d->DrawLine(line.start, line.end, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//}

				//if (line.Intersect(&orientedRect))
				//{
				//	renderer2d->DrawRect(orientedRect.center, orientedRect.size.x, orientedRect.size.y, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f), orientedRect.rotate);
				//	renderer2d->DrawLine(line.start, line.end, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
				//}
				//else
				//{
				//	renderer2d->DrawRect(orientedRect.center, orientedRect.size.x, orientedRect.size.y, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f), orientedRect.rotate);
				//	renderer2d->DrawLine(line.start, line.end, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//}
			}
			renderer2d->End();


			GameMaker::RenderManager::Get().EndFrame();
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}