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
#include "Texture2D.h"

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("07.Sprite2D", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		SetVsyncMode(false);
		SetAlphaBlendMode(true);

		textures_ =
		{
			ResourceManager::Get().Create<Texture2D>("GameMaker/Sample/07.Sprite2D/Res/AlienBeige.png", Texture2D::Filter::LINEAR),
			ResourceManager::Get().Create<Texture2D>("GameMaker/Sample/07.Sprite2D/Res/AlienBlue.png", Texture2D::Filter::LINEAR),
			ResourceManager::Get().Create<Texture2D>("GameMaker/Sample/07.Sprite2D/Res/AlienGreen.png", Texture2D::Filter::LINEAR),
			ResourceManager::Get().Create<Texture2D>("GameMaker/Sample/07.Sprite2D/Res/AlienPink.png", Texture2D::Filter::LINEAR),
			ResourceManager::Get().Create<Texture2D>("GameMaker/Sample/07.Sprite2D/Res/AlienYellow.png", Texture2D::Filter::LINEAR),
		};
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

		float sizeX = 66.0f;
		float sizeY = 92.0f;

		RunLoop(
			[&](float deltaSeconds)
			{
				ImGui::Begin("Framerate", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();

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

					uint32_t current = 0;
					for (float x = minX; x <= maxX; x += sizeX * 0.5f)
					{
						for (float y = maxY; y >= minY; y -= sizeY * 0.5f)
						{
							RenderManager2D::Get().DrawSprite(textures_[current], GameMath::Vec2f(x, y), sizeX * 0.5f, sizeY * 0.5f, 0.0f);
							current = (current + 1) % textures_.size();
						}
					}
				}
				RenderManager2D::Get().End();

				EndFrame();
			}
		);
	}

private:
	std::array<Texture2D*, 5> textures_;
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