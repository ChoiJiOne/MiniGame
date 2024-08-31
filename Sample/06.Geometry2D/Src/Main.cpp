#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "IApp.h"
#include "RenderManager2D.h"

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("06.Geometry2D", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		SetVsyncMode(false);
		SetAlphaBlendMode(true);
	}

	virtual void Shutdown() override
	{
	}

	virtual void Run() override
	{
		float minX = -400.0f;
		float maxX = +400.0f;
		float strideX = 10.0f;
		float minY = -300.0f;
		float maxY = +300.0f;
		float strideY = 10.0f;

		RunLoop(
			[&](float deltaSeconds)
			{
				BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

				RenderManager2D::Get().Begin();
				{
					for (float x = minX; x <= maxX; x += strideX)
					{
						GameMath::Vec4f color = (x == 0.0f) ? GameMath::Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : GameMath::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
						RenderManager2D::Get().DrawLine(GameMath::Vec2f(x, minX), GameMath::Vec2f(x, maxY), color);
					}

					for (float y = minY; y <= maxY; y += strideY)
					{
						GameMath::Vec4f color = (y == 0.0f) ? GameMath::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMath::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
						RenderManager2D::Get().DrawLine(GameMath::Vec2f(minX, y), GameMath::Vec2f(maxX, y), color);
					}

					// 첫 번째 줄 (y = 300)
					RenderManager2D::Get().DrawPoint(GameMath::Vec2f(-380.0f, 300.0f), GameMath::Vec4f(1.0f, 0.5f, 0.0f, 1.0f), 5.0f);
					RenderManager2D::Get().DrawLine(GameMath::Vec2f(-250.0f, 300.0f), GameMath::Vec2f(-150.0f, 300.0f), GameMath::Vec4f(0.7f, 0.3f, 0.9f, 1.0f));
					RenderManager2D::Get().DrawLine(GameMath::Vec2f(0.0f, 300.0f), GameMath::Vec4f(0.9f, 0.7f, 0.2f, 1.0f), GameMath::Vec2f(150.0f, 300.0f), GameMath::Vec4f(0.1f, 0.6f, 0.8f, 1.0f));
					RenderManager2D::Get().DrawTriangle(GameMath::Vec2f(250.0f, 300.0f), GameMath::Vec2f(300.0f, 250.0f), GameMath::Vec2f(350.0f, 300.0f), GameMath::Vec4f(0.5f, 0.0f, 0.5f, 1.0f));

					// 두 번째 줄 (y = 200)
					RenderManager2D::Get().DrawTriangle(GameMath::Vec2f(-350.0f, 200.0f), GameMath::Vec4f(0.3f, 0.7f, 0.9f, 1.0f), GameMath::Vec2f(-300.0f, 150.0f), GameMath::Vec4f(0.8f, 0.6f, 0.2f, 1.0f), GameMath::Vec2f(-250.0f, 200.0f), GameMath::Vec4f(0.2f, 0.9f, 0.4f, 1.0f));
					RenderManager2D::Get().DrawTriangleWireframe(GameMath::Vec2f(-150.0f, 200.0f), GameMath::Vec2f(-50.0f, 150.0f), GameMath::Vec2f(50.0f, 200.0f), GameMath::Vec4f(0.0f, 0.8f, 0.6f, 1.0f));
					RenderManager2D::Get().DrawTriangleWireframe(GameMath::Vec2f(150.0f, 200.0f), GameMath::Vec4f(0.4f, 0.6f, 1.0f, 1.0f), GameMath::Vec2f(200.0f, 150.0f), GameMath::Vec4f(1.0f, 0.2f, 0.4f, 1.0f), GameMath::Vec2f(250.0f, 200.0f), GameMath::Vec4f(0.7f, 0.7f, 0.3f, 1.0f));
					RenderManager2D::Get().DrawRect(GameMath::Vec2f(350.0f, 200.0f), 100.0f, 70.0f, GameMath::Vec4f(0.9f, 0.4f, 0.7f, 1.0f), 10.0f);

					// 세 번째 줄 (y = 100)
					RenderManager2D::Get().DrawRectWireframe(GameMath::Vec2f(-320.0f, 100.0f), 120.0f, 80.0f, GameMath::Vec4f(0.1f, 0.8f, 0.8f, 1.0f), 20.0f);
					RenderManager2D::Get().DrawRoundRect(GameMath::Vec2f(-100.0f, 100.0f), 140.0f, 90.0f, 15.0f, GameMath::Vec4f(0.6f, 0.2f, 0.9f, 1.0f), 15.0f);
					RenderManager2D::Get().DrawRoundRectWireframe(GameMath::Vec2f(120.0f, 100.0f), 160.0f, 100.0f, 20.0f, GameMath::Vec4f(0.9f, 0.7f, 0.2f, 1.0f), 25.0f);
					RenderManager2D::Get().DrawCircle(GameMath::Vec2f(350.0f, 100.0f), 60.0f, GameMath::Vec4f(0.2f, 0.9f, 0.5f, 1.0f));

					// 네 번째 줄 (y = 0)
					RenderManager2D::Get().DrawCircleWireframe(GameMath::Vec2f(-350.0f, 0.0f), 70.0f, GameMath::Vec4f(1.0f, 0.5f, 0.0f, 1.0f));
					RenderManager2D::Get().DrawPoint(GameMath::Vec2f(-150.0f, 0.0f), GameMath::Vec4f(0.7f, 0.1f, 0.9f, 1.0f), 10.0f);
					RenderManager2D::Get().DrawLine(GameMath::Vec2f(50.0f, 0.0f), GameMath::Vec2f(150.0f, 0.0f), GameMath::Vec4f(0.3f, 0.9f, 0.6f, 1.0f));
					RenderManager2D::Get().DrawTriangle(GameMath::Vec2f(300.0f, 0.0f), GameMath::Vec2f(350.0f, -50.0f), GameMath::Vec2f(400.0f, 0.0f), GameMath::Vec4f(0.8f, 0.5f, 0.2f, 1.0f));

					// 다섯 번째 줄 (y = -200)
					RenderManager2D::Get().DrawTriangleWireframe(GameMath::Vec2f(-380.0f, -200.0f), GameMath::Vec2f(-280.0f, -250.0f), GameMath::Vec2f(-180.0f, -200.0f), GameMath::Vec4f(0.0f, 0.8f, 0.6f, 1.0f));
					RenderManager2D::Get().DrawRect(GameMath::Vec2f(-100.0f, -200.0f), 100.0f, 70.0f, GameMath::Vec4f(0.9f, 0.4f, 0.7f, 1.0f), 30.0f);
					RenderManager2D::Get().DrawRoundRectWireframe(GameMath::Vec2f(100.0f, -200.0f), 160.0f, 100.0f, 20.0f, GameMath::Vec4f(0.9f, 0.7f, 0.2f, 1.0f), 25.0f);
					RenderManager2D::Get().DrawCircle(GameMath::Vec2f(350.0f, -200.0f), 60.0f, GameMath::Vec4f(0.2f, 0.9f, 0.5f, 1.0f));

					// 여섯 번째 줄 (y = -300)
					RenderManager2D::Get().DrawCircleWireframe(GameMath::Vec2f(-350.0f, -300.0f), 70.0f, GameMath::Vec4f(1.0f, 0.5f, 0.0f, 1.0f));
					RenderManager2D::Get().DrawRectWireframe(GameMath::Vec2f(-150.0f, -300.0f), 120.0f, 80.0f, GameMath::Vec4f(0.1f, 0.8f, 0.8f, 1.0f), 20.0f);
					RenderManager2D::Get().DrawRoundRect(GameMath::Vec2f(50.0f, -300.0f), 140.0f, 90.0f, 15.0f, GameMath::Vec4f(0.6f, 0.2f, 0.9f, 1.0f), 15.0f);
					RenderManager2D::Get().DrawTriangle(GameMath::Vec2f(250.0f, -300.0f), GameMath::Vec2f(300.0f, -350.0f), GameMath::Vec2f(350.0f, -300.0f), GameMath::Vec4f(0.5f, 0.0f, 0.5f, 1.0f));
				}
				RenderManager2D::Get().End();
				
				EndFrame();
			}
		);
	}
};

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::unique_ptr<IApp> app = std::make_unique<DemoApp>();
	app->Startup();
	app->Run();
	app->Shutdown();
	return 0;
}