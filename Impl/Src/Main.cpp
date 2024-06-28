#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include <imgui.h>
#include <glad/glad.h>

#include "Assertion.h"
#include "CrashModule.h"
#include "GameModule.h"
#include "PlatformModule.h"
#include "RenderModule.h"

#include "Mat.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "TTFont.h"

#include "Camera.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	PlatformModule::WindowConstructParams windowParam{ L"Impl", 200, 200, 800, 600, false, false };

	ASSERT(CrashModule::Init() == CrashModule::Errors::OK, "Failed to initialize CrashModule.");
	ASSERT(PlatformModule::Init(windowParam) == PlatformModule::Errors::OK, "Failed to initialize PlatformModule.");
	ASSERT(RenderModule::Init(PlatformModule::GetWindowHandle()) == RenderModule::Errors::OK, "Failed to initialize RenderModule.");

	PlatformModule::SetEndLoopCallback([&]() { RenderModule::Uninit(); });

	TTFont* font = RenderModule::CreateResource<TTFont>("Impl/Resource/Font/SeoulNamsanEB.ttf", 0x0, 0x127, 32.0f);
	Renderer2D* renderer2d = RenderModule::CreateResource<Renderer2D>();
	Renderer3D* renderer3d = RenderModule::CreateResource<Renderer3D>();

	Camera* camera = GameModule::CreateEntity<Camera>();

	PlatformModule::RunLoop(
		[&](float deltaSeconds)
		{
			camera->Tick(deltaSeconds);

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer3d->Begin(camera->GetView(), camera->GetProjection());
			{
				renderer3d->DrawGrid(Vec3f(100.0f, 100.0f, 100.0f), 1.0f);
				renderer3d->DrawSphere(Mat4x4::Identity(), 1.0f, Vec4f(1.0f, 0.0f, 1.0f, 1.0f));
			}
			renderer3d->End();

			renderer2d->Begin(camera->GetScreenOrtho());
			{
				renderer2d->DrawRoundRectWireframe(Vec2f(0.0f, 0.0f), 100.0f, 100.0f, 10.0f, Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
			}
			renderer2d->End();

			RenderModule::EndFrame();
		}
	);

	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}