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
	DemoApp() : IApp("08.String2D", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		SetVsyncMode(false);
		SetAlphaBlendMode(true);

		/** こんにちは */
		font16_ = ResourceManager::Get().Create<TTFont>("GameMaker/Sample/08.String2D/Res/SeoulNamsanEB.ttf", 0x3040, 0x309F, 16.0f);

		/** 안녕하세요 */
		font32_ = ResourceManager::Get().Create<TTFont>("GameMaker/Sample/08.String2D/Res/SeoulNamsanEB.ttf", 0xB155, 0xD558, 32.0f);

		/** Hello, World! */
		font64_ = ResourceManager::Get().Create<TTFont>("GameMaker/Sample/08.String2D/Res/SeoulNamsanEB.ttf", 0x00, 0x127, 64.0f);

		/** Big size String. */
		font128_ = ResourceManager::Get().Create<TTFont>("GameMaker/Sample/08.String2D/Res/SeoulNamsanEB.ttf", 0x00, 0x127, 128.0f);
	}

	virtual void Shutdown() override
	{
		ResourceManager::Get().Destroy(font16_);
		ResourceManager::Get().Destroy(font32_);
		ResourceManager::Get().Destroy(font64_);
		ResourceManager::Get().Destroy(font128_);
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
				ImGui::Begin("Framerate", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();

				BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

				RenderManager2D::Get().Begin();
				{
					for (float x = minX; x <= maxX; x += strideX)
					{
						Vec4f color = (x == 0.0f) ? Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
						RenderManager2D::Get().DrawLine(Vec2f(x, minX), Vec2f(x, maxY), color);
					}

					for (float y = minY; y <= maxY; y += strideY)
					{
						Vec4f color = (y == 0.0f) ? Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
						RenderManager2D::Get().DrawLine(Vec2f(minX, y), Vec2f(maxX, y), color);
					}

					RenderManager2D::Get().DrawString(font16_, L"こんにちは", Vec2f(-380.0f, 130.0f), Vec4f(1.0f, 0.5f, 0.5f, 1.0f));
					RenderManager2D::Get().DrawString(font32_, L"안녕하세요", Vec2f(-380.0f, 110.0f), Vec4f(0.5f, 1.0f, 0.5f, 1.0f));
					RenderManager2D::Get().DrawString(font64_, L"Hello, World!", Vec2f(-380.0f, 80.0f), Vec4f(0.5f, 0.5f, 1.0f, 1.0f));
					RenderManager2D::Get().DrawString(font128_, L"Big String!", Vec2f(-380.0f, 30.0f), Vec4f(0.5f, 0.5f, 0.5f, 1.0f));
				}
				RenderManager2D::Get().End();

				EndFrame();
			}
		);
	}

private:
	TTFont* font16_ = nullptr;
	TTFont* font32_ = nullptr;
	TTFont* font64_ = nullptr;
	TTFont* font128_ = nullptr;
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