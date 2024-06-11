#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "CrashModule.h"
#include "GameModule.h"
#include "GLTFLoader.h"
#include "PlatformModule.h"
#include "RenderModule.h"

#include "Camera.h"
#include "GLTFLoader.h"
#include "GeometryRenderer2D.h"
#include "GeometryRenderer3D.h"
#include "SkinnedMesh.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "GLTFLoader.h"
#include "CrossFadeController.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	PlatformModule::WindowConstructParams windowParam{ L"CoinDash3D", 100, 100, 800, 600, false, false };

	CrashModule::Init();
	PlatformModule::Init(windowParam);
	RenderModule::Errors error =  RenderModule::Init(PlatformModule::GetWindowHandle());
	GameModule::Init();

	PlatformModule::SetEndLoopCallback([&]() { RenderModule::Uninit(); });
	
	GeometryRenderer2D* renderer2d = RenderModule::CreateResource<GeometryRenderer2D>();
	GeometryRenderer3D* renderer3d = RenderModule::CreateResource<GeometryRenderer3D>();
	Camera* camera = GameModule::CreateEntity<Camera>();

	PlatformModule::RunLoop(
		[&](float deltaSeconds)
		{
			camera->Tick(deltaSeconds);

			renderer3d->SetView(camera->GetView());
			renderer3d->SetProjection(camera->GetProjection());
			renderer2d->SetOrtho(RenderModule::GetScreenOrtho());

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer3d->DrawGrid3D(Vec3f(100.0f, 100.0f, 100.0f), 1.0f);
			//renderer2d->DrawCircle2D(Vec2f(200.0f, 200.0f), 10.0f, Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
			renderer2d->DrawWireframeCircle2D(Vec2f(200.0f, 200.0f), 10.0f, Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

			static float time = 0.0f;
			time += deltaSeconds;
			renderer2d->DrawHorizonProgressBar2D(Vec2f(400.0f, 300.0f), 100.0f, 20.0f, time / 5.0f, Vec4f(1.0f, 0.0f, 0.0f, 1.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

			RenderModule::EndFrame();
		}
	);

	GameModule::Uninit();
	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}