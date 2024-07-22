#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include <imgui.h>

#include "GameEngine.h"
#include "Renderer2D.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "TTFont.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	GameMaker::GameEngine::Init({ "07.Rendering2D", 100, 100, 800, 600, false, false });

	GameMaker::RenderManager::Get().SetVsyncMode(false);
	GameMaker::Renderer2D* renderer = GameMaker::RenderManager::Get().GetRenderer2D();

	GameMaker::TTFont* font = GameMaker::ResourceManager::Get().Create<GameMaker::TTFont>("GameMaker/Sample/07.Rendering2D/Res/SeoulNamsanEB.ttf", 0x00, 0x127, 32.0f);
	GameMaker::Texture2D* texture = GameMaker::ResourceManager::Get().Create<GameMaker::Texture2D>("GameMaker/Sample/07.Rendering2D/Res/awesomeface.png", true);

	bool bIsVFlip = false;
	bool bIsHFlip = false;

	float factor = 0.0f;

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds)
		{
			ImGui::Begin("Framerate", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::Checkbox("V Flip", &bIsVFlip);
			ImGui::Checkbox("H Flip", &bIsHFlip);
			ImGui::SliderFloat("factor", &factor, 0.0f, 1.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
			renderer->Begin();
			{
				for (float x = -400.0f; x <= 400.0f; x += 10.0f)
				{
					GameMaker::Vec4f color = (x == 0.0f) ? GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
					renderer->DrawLine(GameMaker::Vec2f(x, -300.0f), GameMaker::Vec2f(x, +300.0f), color);
				}

				for (float y = -300.0f; y <= 300.0f; y += 10.0f)
				{
					GameMaker::Vec4f color = (y == 0.0f) ? GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
					renderer->DrawLine(GameMaker::Vec2f(-400.0f, y), GameMaker::Vec2f(+400.0f, y), color);
				}

				renderer->DrawString(font, L"Hello, World!", GameMaker::Vec2f(0.0f, 0.0f), GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				renderer->DrawString(font, L"This is Test String", GameMaker::Vec2f(0.0f, 30.0f), GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));

				static float time = 0.0f;
				time += deltaSeconds;
				renderer->DrawSprite(texture, GameMaker::Vec2f(0.0f, 0.0f), 100.0f, 100.0f, GameMaker::Vec3f(1.0f, 0.0f, 0.0f), factor, 0.0f, bIsHFlip, bIsVFlip);
				renderer->DrawSprite(texture, GameMaker::Vec2f(100.0f, 0.0f), 100.0f, 100.0f, time, bIsHFlip, bIsVFlip);
				renderer->DrawSprite(texture, GameMaker::Vec2f(-100.0f, 0.0f), 100.0f, 100.0f, time, bIsHFlip, bIsVFlip);
				renderer->DrawSprite(texture, GameMaker::Vec2f(0.0f, 100.0f), 100.0f, 100.0f, time, bIsHFlip, bIsVFlip);
				renderer->DrawSprite(texture, GameMaker::Vec2f(0.0f, -100.0f), 100.0f, 100.0f, time, bIsHFlip, bIsVFlip);
			}
			renderer->End();
			GameMaker::RenderManager::Get().EndFrame();
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}