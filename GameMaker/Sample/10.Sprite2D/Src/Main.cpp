#include <array>
#include <cstdint>
#include <vector>
#include <Windows.h>

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
#include "VertexBuffer.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameMaker::GameEngine::Init({ "10.Sprite2D", 100, 100, 800, 600, false, false });

	float minX = -400.0f;
	float maxX = +400.0f;
	float strideX = 10.0f;
	float minY = -300.0f;
	float maxY = +300.0f;
	float strideY = 10.0f;
	
	GameMaker::Renderer2D* renderer = GameMaker::RenderManager::Get().GetRenderer2D();
	GameMaker::Texture2D* texture = GameMaker::ResourceManager::Get().Create<GameMaker::Texture2D>("GameMaker/Sample/10.Sprite2D/Res/AlienBlue.png", true);

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds)
		{
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

				renderer->DrawSprite(texture, GameMaker::Vec2f(0.0f, 0.0f), 66.0f, 92.0f, 0.0f);
				renderer->DrawRectWireframe(GameMaker::Vec2f(0.0f, 0.0f), 66.0f, 92.0f, GameMaker::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
			}
			renderer->End();

			GameMaker::RenderManager::Get().EndFrame();
		}
	);

	GameMaker::GameEngine::Shutdown();

	return 0;
}