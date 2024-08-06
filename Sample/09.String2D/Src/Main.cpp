
#include <array>
#include <cstdint>
#include <vector>
#include <Windows.h>

#include <imgui.h>
#include <glad/glad.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "EntityManager.h"
#include "FreeEulerCamera3D.h"
#include "GameEngine.h"
#include "GameMath.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "Shader.h"
#include "Texture2D.h"
#include "TTFont.h"
#include "VertexBuffer.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameMaker::GameEngine::Init({ "09.String2D", 100, 100, 800, 600, false, false });
	GameMaker::RenderManager::Get().SetVsyncMode(false);

	float minX = -400.0f;
	float maxX = +400.0f;
	float strideX = 10.0f;
	float minY = -300.0f;
	float maxY = +300.0f;
	float strideY = 10.0f;

	GameMaker::Renderer2D* renderer = GameMaker::RenderManager::Get().GetRenderer2D();

	GameMaker::TTFont* font8 = GameMaker::ResourceManager::Get().Create<GameMaker::TTFont>("GameMaker/Sample/09.String2D/Res/SeoulNamsanEB.ttf", 0x00, 0x127, 8.0f);
	GameMaker::TTFont* font16 = GameMaker::ResourceManager::Get().Create<GameMaker::TTFont>("GameMaker/Sample/09.String2D/Res/SeoulNamsanEB.ttf", 0x00, 0x127, 16.0f);
	GameMaker::TTFont* font32 = GameMaker::ResourceManager::Get().Create<GameMaker::TTFont>("GameMaker/Sample/09.String2D/Res/SeoulNamsanEB.ttf", 0x00, 0x127, 32.0f);
	GameMaker::TTFont* font64 = GameMaker::ResourceManager::Get().Create<GameMaker::TTFont>("GameMaker/Sample/09.String2D/Res/SeoulNamsanEB.ttf", 0x00, 0x127, 64.0f);
	GameMaker::TTFont* font128 = GameMaker::ResourceManager::Get().Create<GameMaker::TTFont>("GameMaker/Sample/09.String2D/Res/SeoulNamsanEB.ttf", 0x00, 0x127, 128.0f);

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
				for (float x = minX; x <= maxX; x += strideX)
				{
					GameMaker::Vec4f color = (x == 0.0f) ? GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
					renderer->DrawLine(GameMaker::Vec2f(x, minX), GameMaker::Vec2f(x, maxY), color);
				}

				for (float y = minY; y <= maxY; y += strideY)
				{
					GameMaker::Vec4f color = (y == 0.0f) ? GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
					renderer->DrawLine(GameMaker::Vec2f(minX, y), GameMaker::Vec2f(maxX, y), color);
				}

				renderer->DrawString(font8, L"Hello, World!", GameMaker::Vec2f(-380.0f, 130.0f), GameMaker::Vec4f(1.0f, 0.5f, 0.5f, 1.0f));
				renderer->DrawString(font16, L"Hello, World!", GameMaker::Vec2f(-380.0f, 110.0f), GameMaker::Vec4f(0.5f, 1.0f, 0.5f, 1.0f));
				renderer->DrawString(font32, L"Hello, World!", GameMaker::Vec2f(-380.0f, 80.0f), GameMaker::Vec4f(0.5f, 0.5f, 1.0f, 1.0f));
				renderer->DrawString(font64, L"Hello, World!", GameMaker::Vec2f(-380.0f, 40.0f), GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 1.0f));
				renderer->DrawString(font128, L"Hello, World!", GameMaker::Vec2f(-380.0f, -30.0f), GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
			}
			renderer->End();

			GameMaker::RenderManager::Get().EndFrame();
		}
	);

	GameMaker::GameEngine::Shutdown();

	return 0;
}