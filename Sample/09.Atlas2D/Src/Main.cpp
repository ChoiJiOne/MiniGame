#include <cstdint>
#include <Windows.h>

#include <imgui.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "Atlas2D.h"
#include "IApp.h"
#include "RenderManager2D.h"
#include "ResourceManager.h"

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("09.Atlas2D", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		SetVsyncMode(false);
		SetAlphaBlendMode(true);

		bird_ = ResourceManager::Get().Create<Atlas2D>(
			"GameMaker/Sample/09.Atlas2D/Res/bird/bird.png", 
			"GameMaker/Sample/09.Atlas2D/Res/bird/bird.json", 
			Texture2D::Filter::NEAREST
		);
	}

	virtual void Shutdown() override
	{
		ResourceManager::Get().Destroy(bird_);
	}

	virtual void Run() override
	{
		float minX = -400.0f;
		float maxX = +400.0f;
		float strideX = 10.0f;
		float minY = -300.0f;
		float maxY = +300.0f;
		float strideY = 10.0f;

		const std::vector<std::string>& keys = bird_->GetKeys();
		
		RunLoop(
			[&](float deltaSeconds)
			{
				stepTime_ += deltaSeconds;
				if (stepTime_ >= 0.1f)
				{
					stepTime_ -= 0.1f;
					current_ = (current_ + 1) % keys.size();
				}

				ImGui::Begin("Framerate", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				if (ImGui::Button("FLIP-V"))
				{
					bIsFlipV_ = !bIsFlipV_;
				}
				ImGui::SameLine();
				if (ImGui::Button("FLIP-H"))
				{
					bIsFlipH_ = !bIsFlipH_;
				}
				ImGui::SameLine();
				ImGui::Text("FLIP-V [%s]", bIsFlipV_ ? "ON" : "OFF");
				ImGui::SameLine();
				ImGui::Text("FLIP-H [%s]", bIsFlipH_ ? "ON" : "OFF");
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

					RenderManager2D::Get().DrawSprite(bird_, keys[current_], Vec2f(0.0f, 0.0f), 128.0f, 128.0f, 0.0f, bIsFlipH_, bIsFlipV_);
				}
				RenderManager2D::Get().End();

				EndFrame();
			}
		);
	}

private:
	float stepTime_ = 0.0f;
	int32_t current_ = 0;
	Atlas2D* bird_ = nullptr;

	bool bIsFlipH_ = false;
	bool bIsFlipV_ = false;
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