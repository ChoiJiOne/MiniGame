#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include <SDL2/SDL.h>

#include "GameEngine.h"
#include "GameTimer.h"
#include "GameMath.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "FileUtils.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "ResourceManager.h"
#include "EntityManager.h"

#include "Camera.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameMaker::WindowParam param { "CoinDash3D", 100, 100, 800, 600, false, false };
	
	GameMaker::GameEngine::Init(param);

	GameMaker::Renderer2D* renderer2d = GameMaker::ResourceManager::Get().Create<GameMaker::Renderer2D>();
	GameMaker::Renderer3D* renderer3d = GameMaker::ResourceManager::Get().Create<GameMaker::Renderer3D>();

	Camera* camera = GameMaker::EntityManager::Get().Create<Camera>();

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds) 
		{
			camera->Tick(deltaSeconds);

			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer3d->Begin(camera->GetView(), camera->GetProjection());
			{
				renderer3d->DrawGrid(GameMaker::Vec3f(100.0f, 100.0f, 100.0f), 1.0f);
				renderer3d->DrawCube(GameMaker::Mat4x4::Identity(), GameMaker::Vec3f(1.0f, 1.0f, 1.0f), GameMaker::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
			}
			renderer3d->End();

			renderer2d->Begin(camera->GetScreenOrtho());
			{
				renderer2d->DrawRoundRectWireframe(GameMaker::Vec2f(0.0f, 0.0f), 100.0f, 100.0f, 10.0f, GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
			}
			renderer2d->End();


			GameMaker::RenderManager::Get().EndFrame();
		}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}