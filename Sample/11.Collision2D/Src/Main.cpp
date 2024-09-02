#include <cstdint>
#include <Windows.h>

#include <imgui.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "IApp.h"
#include "RenderManager2D.h"
#include "ResourceManager.h"
#include "TTFont.h"

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("11.Collision2D", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
	}

	virtual void Shutdown() override
	{
		ResourceManager::Get().Destroy(font_);
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
				GameMath::Vec2i mousePos = GetCurrMousePos();
				Press press = GetMousePress(Mouse::LEFT);

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

					RenderManager2D::Get().DrawString(font_, GameUtils::PrintF(L"MOUSE POS: (%d, %d)", mousePos.x, mousePos.y), GameMath::Vec2f(0.0f, 0.0f), GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
					RenderManager2D::Get().DrawString(font_, GameUtils::PrintF(L"MOUSE POS: (%d, %d)", mousePos.x, mousePos.y), GameMath::Vec2f(0.0f, 0.0f), GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
					RenderManager2D::Get().DrawString(font_, GameUtils::PrintF(L"MOUSE POS: (%d, %d)", mousePos.x, mousePos.y), GameMath::Vec2f(0.0f, 0.0f), GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
					RenderManager2D::Get().DrawString(font_, GameUtils::PrintF(L"MOUSE POS: (%d, %d)", mousePos.x, mousePos.y), GameMath::Vec2f(0.0f, 0.0f), GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
					RenderManager2D::Get().DrawString(font_, GameUtils::PrintF(L"MOUSE POS: (%d, %d)", mousePos.x, mousePos.y), GameMath::Vec2f(0.0f, 0.0f), GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
					
				}
				RenderManager2D::Get().End();

				EndFrame();
			}
		);
	}

private:
	TTFont* font_ = nullptr;
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