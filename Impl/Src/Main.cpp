#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "CrashModule.h"
#include "PlatformModule.h"
#include "RenderModule.h"

#include "GeometryRenderer2D.h"

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

	GeometryRenderer2D* renderer = RenderModule::CreateResource<GeometryRenderer2D>();

	PlatformModule::RunLoop(
		[&](float deltaSeconds) 
		{
			renderer->SetOrtho(RenderModule::GetScreenOrtho());

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer->DrawLine2D(Vec2f(0.0f, 0.0f), Vec2f(400.0f, 300.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

			RenderModule::EndFrame();
		}
	);

	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}