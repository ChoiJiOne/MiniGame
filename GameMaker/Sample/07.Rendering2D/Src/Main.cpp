#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include <imgui.h>

#include "GameEngine.h"
#include "Renderer2D.h"
#include "RenderManager.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	GameMaker::GameEngine::Init({ "07.Rendering2D", 100, 100, 800, 600, false, false });

	GameMaker::RenderManager::Get().SetVsyncMode(false);
	GameMaker::Renderer2D* renderer = GameMaker::RenderManager::Get().GetRenderer2D();

	GameMaker::Vec2f center(0.0f, 0.0f);

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
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 400.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 300.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 200.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 0.0f, 1.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 100.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 0.0f), 200.0f, 50.0f, GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 1.0f));

				//renderer->DrawRect(GameMaker::Vec2f(0.0f, -400.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, -300.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, -200.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 0.0f, 1.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, -100.0f), 200.0f, 50.0f, GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

				//static float time = 0.0f;
				//time += deltaSeconds;

				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 100.0f), 200.0f, 100.0f, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f), time);
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 50.0f), 200.0f, 100.0f, GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
				//renderer->DrawRect(GameMaker::Vec2f(0.0f, 0.0f), 200.0f, 100.0f, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f), -time / 10.0f);
				//GameMaker::Vec2f point;

				//renderer->DrawPoint(GameMaker::Vec2f(), GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f), 50.0f);


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

				static float time = 0.0f;
				time += deltaSeconds;
				float rate = time / 10.0f;
				rate = GameMaker::Clamp<float>(rate, 0.0f, 1.0f);
				//renderer->DrawRectWireframe(GameMaker::Vec2f(0.0f, 100.0f), 200.0f, 100.0f, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f), time);
				//renderer->DrawRectWireframe(GameMaker::Vec2f(0.0f, 50.0f), 200.0f, 100.0f, GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
				//renderer->DrawRectWireframe(GameMaker::Vec2f(0.0f, 0.0f), 200.0f, 100.0f, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f), -time / 10.0f);

				renderer->DrawRoundRect(GameMaker::Vec2f(0.0f, 0.0f), 200.0f, 100.0f, 20.0f, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));

				//GameMaker::Vec2f start(-100.0f, 0.0f);
				//GameMaker::Vec2f control(0.0f, 50.0f);
				//GameMaker::Vec2f end(+100.0f, 0.0f);



				//renderer->DrawPoint(start, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f), 5.0f);
				//renderer->DrawPoint(end, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f), 5.0f);
				//renderer->DrawPoint(control, GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f), 5.0f);

				//renderer->DrawPoint(GameMaker::Vec2f::Bezier(start, end, control, rate), GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f), 5.0f);

				//renderer->DrawTriangleWireframe(GameMaker::Vec2f(-100.5f, -150.75f), GameMaker::Vec2f(0.0f, 299.9f), GameMaker::Vec2f(200.25f, -100.5f), GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
				//renderer->DrawTriangleWireframe(GameMaker::Vec2f(-300.33f, 250.67f), GameMaker::Vec2f(-200.2f, -250.5f), GameMaker::Vec2f(100.5f, -100.25f), GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
				//renderer->DrawTriangleWireframe(GameMaker::Vec2f(150.5f, -200.75f), GameMaker::Vec2f(250.75f, 100.5f), GameMaker::Vec2f(-50.3f, 200.8f), GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));

				//renderer->DrawTriangleWireframe(
				//	GameMaker::Vec2f(-100.5f, -150.75f), GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f),
				//	GameMaker::Vec2f(0.0f, 299.9f), GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f),
				//	GameMaker::Vec2f(200.25f, -100.5f), GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f)
				//);

				//renderer->DrawTriangleWireframe(
				//	GameMaker::Vec2f(-300.33f, 250.67f), GameMaker::Vec4f(1.0f, 1.0f, 0.0f, 1.0f),
				//	GameMaker::Vec2f(-200.2f, -250.5f), GameMaker::Vec4f(1.0f, 0.0f, 1.0f, 1.0f),
				//	GameMaker::Vec2f(100.5f, -100.25f), GameMaker::Vec4f(0.0f, 1.0f, 1.0f, 1.0f)
				//);

				//renderer->DrawTriangleWireframe(
				//	GameMaker::Vec2f(150.5f, -200.75f), GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 1.0f),
				//	GameMaker::Vec2f(250.75f, 100.5f), GameMaker::Vec4f(0.25f, 0.25f, 0.25f, 1.0f),
				//	GameMaker::Vec2f(-50.3f, 200.8f), GameMaker::Vec4f(0.75f, 0.75f, 0.75f, 1.0f)
				//);

				//renderer->DrawLine(
				//	GameMaker::Vec2f(0.0f, 0.0f), GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f),
				//	GameMaker::Vec2f(101.7f, 97.3f), GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f)
				//);
				//renderer->DrawLine(
				//	GameMaker::Vec2f(101.7f, 97.3f), GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f),
				//	GameMaker::Vec2f(203.3f, 57.3f), GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f)
				//);

			}
			renderer->End();
			GameMaker::RenderManager::Get().EndFrame();
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}