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
#include "Renderer3D.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameMaker::GameEngine::Init({ "08.Grid3D", 100, 100, 800, 600, false, false });

	GameMaker::Renderer3D* renderer = GameMaker::RenderManager::Get().GetRenderer3D();
	GameMaker::FreeEulerCamera3D* camera = GameMaker::EntityManager::Get().Create<GameMaker::FreeEulerCamera3D>(GameMaker::Vec3f(3.0f, 3.0f, 3.0f), -GameMaker::ToRadian(135.0f), -GameMaker::PI / 6.0f, GameMaker::PI_DIV_4, 0.01f, 100.0f);

	float minX = -50.0f;
	float maxX = +50.0f;
	float minY = -50.0f;
	float maxY = +50.0f;
	float minZ = -50.0f;
	float maxZ = +50.0f;
	float stride = 1.0f;

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds)
		{
			camera->Tick(deltaSeconds);

			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer->Begin(camera);
			{
				GameMaker::Vec4f color;
				for (float x = minX; x <= maxX; x += stride)
				{
					color = (x == 0.0f) ? GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
					renderer->DrawLine(GameMaker::Vec3f(x, 0.0f, minZ), GameMaker::Vec3f(x, 0.0f, maxZ), color);
				}

				for (float z = minZ; z <= maxZ; z += stride)
				{
					color = (z == 0.0f) ? GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
					renderer->DrawLine(GameMaker::Vec3f(minX, 0.0f, z), GameMaker::Vec3f(maxX, 0.0f, z), color);
				}

				renderer->DrawLine(GameMaker::Vec3f(0.0f, minY, 0.0f), GameMaker::Vec3f(0.0f, maxY, 0.0f), GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
			}
			renderer->End();

			GameMaker::RenderManager::Get().EndFrame();
		}
	);

	GameMaker::GameEngine::Shutdown();

	return 0;
}