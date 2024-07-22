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
	GameMaker::TTFont* hangul = GameMaker::ResourceManager::Get().Create<GameMaker::TTFont>("GameMaker/Sample/07.Rendering2D/Res/SeoulNamsanEB.ttf", 0x00, 0xD7FF, 32.0f);
	GameMaker::Texture2D* texture = GameMaker::ResourceManager::Get().Create<GameMaker::Texture2D>("GameMaker/Sample/07.Rendering2D/Res/awesomeface.png", true);

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds)
		{			
			ImGui::Begin("Framerate", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
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

				renderer->DrawSprite(texture, GameMaker::Vec2f(-200.0f, 200.0f), 100.0f, 100.0f, 0.0f);
				renderer->DrawSprite(texture, GameMaker::Vec2f(+0.0f, 200.0f), 100.0f, 100.0f, 0.0f, false, true);
				renderer->DrawSprite(texture, GameMaker::Vec2f(+200.0f, 200.0f), 100.0f, 100.0f, 0.0f, true, false);

				float w = 0.0f, h = 0.0f;

				font->MeasureText(L"None", w, h);
				renderer->DrawString(font, L"None", GameMaker::Vec2f(-200.0f, 140.0f) + GameMaker::Vec2f(-0.5f * w, 0.5f * h), GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

				font->MeasureText(L"FlipV", w, h);
				renderer->DrawString(font, L"FlipV", GameMaker::Vec2f(0.0f, 140.0f) + GameMaker::Vec2f(-0.5f * w, 0.5f * h), GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

				font->MeasureText(L"FlipH", w, h);
				renderer->DrawString(font, L"FlipH", GameMaker::Vec2f(+200.0f, 140.0f) + GameMaker::Vec2f(-0.5f * w, 0.5f * h), GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

				renderer->DrawString(font, L"Hello, World!", GameMaker::Vec2f(50.0f, 50.0f), GameMaker::Vec4f(0.4f, 1.0f, 0.4f, 1.0f));
				renderer->DrawString(hangul, L"안녕하세요~", GameMaker::Vec2f(-350.0f, 100.0f), GameMaker::Vec4f(1.0f, 0.0f, 1.0f, 1.0f));
				renderer->DrawString(hangul, L"한글 문자열 출력 테스트!", GameMaker::Vec2f(-350.0f, 60.0f), GameMaker::Vec4f(0.0f, 1.0f, 1.0f, 1.0f));

				renderer->DrawLine(GameMaker::Vec2f(-390.0f, -151.0f), GameMaker::Vec2f(-300.0f, -50.0f), GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				renderer->DrawTriangleWireframe(GameMaker::Vec2f(-250.0f, -50.0f), GameMaker::Vec2f(-200.0f, -150.0f), GameMaker::Vec2f(-150.0f, -100.0f), GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
				renderer->DrawTriangleWireframe(
					GameMaker::Vec2f(-100.0f, -50.0f), GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f), 
					GameMaker::Vec2f(-50.0f, -150.0f), GameMaker::Vec4f(1.0f, 1.0f, 0.0f, 1.0f), 
					GameMaker::Vec2f(0.0f, -100.0f), GameMaker::Vec4f(0.0f, 1.0f, 1.0f, 1.0f)
				);
				renderer->DrawRectWireframe(GameMaker::Vec2f(100.0f, -100.0f), 50.0f, 100.0f, GameMaker::Vec4f(1.0f, 0.0f, 1.0f, 1.0f), GameMaker::PI_DIV_4); // 45도 회전
				renderer->DrawRoundRectWireframe(GameMaker::Vec2f(220.0f, -100.0f), 50.0f, 100.0f, 10.0f, GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 1.0f), GameMaker::ToRadian(-30.0f)); // 30도 회전
				renderer->DrawCircleWireframe(GameMaker::Vec2f(330.0f, -100.0f), 50.0f, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

				renderer->DrawTriangle(GameMaker::Vec2f(-390.0f, -300.0f), GameMaker::Vec2f(-250.0f, -200.0f), GameMaker::Vec2f(-300.0f, -150.0f), GameMaker::Vec4f(0.7f, 0.3f, 0.6f, 1.0f));
				renderer->DrawTriangle(
					GameMaker::Vec2f(-160.0f, -260.0f),GameMaker::Vec4f(0.8f, 0.2f, 0.5f, 1.0f),
					GameMaker::Vec2f(-90.0f, -180.0f), GameMaker::Vec4f(0.2f, 0.8f, 0.5f, 1.0f),
					GameMaker::Vec2f(-230.0f, -160.0f), GameMaker::Vec4f(0.5f, 0.7f, 0.1f, 1.0f)
				);
				renderer->DrawRect(GameMaker::Vec2f(0.0f, -200.0f), 100.0f, 20.0f, GameMaker::Vec4f(1.0f, 0.5f, 0.2f, 1.0f), GameMaker::ToRadian(-67.0f));
				renderer->DrawRoundRect(GameMaker::Vec2f(150.0f, -220.0f), 150.0f, 50.0f, 20.0f, GameMaker::Vec4f(0.3f, 1.0f, 0.7f, 1.0f), GameMaker::ToRadian(31.0f));
				renderer->DrawCircle(GameMaker::Vec2f(280.0f, -200.0f), 50.0f, GameMaker::Vec4f(1.0f, 1.0f, 0.3f, 1.0f));
			}
			renderer->End();
			GameMaker::RenderManager::Get().EndFrame();
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}