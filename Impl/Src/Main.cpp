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
#include "Renderer2D.h"

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
	
	Renderer2D* renderer = RenderModule::CreateResource<Renderer2D>();

	std::vector<Vec2f> positions;
	std::vector<Vec4f> colors;

	for (uint32_t x = 0; x <= 784; x += 10)
	{
		positions.push_back(Vec2f(static_cast<float>(x), 0.0f));
		positions.push_back(Vec2f(static_cast<float>(x), 561.0f));
	}

	for (uint32_t y = 0; y <= 561; y += 10)
	{
		positions.push_back(Vec2f(0.0f, static_cast<float>(y)));
		positions.push_back(Vec2f(784.0f, static_cast<float>(y)));
	}
		
	PlatformModule::RunLoop(
		[&](float deltaSeconds) 
		{
			renderer->SetOrtho(RenderModule::GetScreenOrtho());

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer->DrawLines(positions.data(), positions.size(), Vec4f(1.0f, 1.0f, 1.0f, 0.5f));

			renderer->DrawLine(
				Vec2f(0.4f, 0.4f), Vec4f(1.0f, 0.0f, 0.0f, 1.0f), 
				Vec2f(100.0f, 100.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f)
			);

			////renderer->DrawTriangle(Vec2f(100.0f, 100.0f), Vec2f(400.0f, 100.0f), Vec2f(250.0f, 300.0f), Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
			//renderer->DrawTriangle(
			//	Vec2f(100.0f, 100.0f), Vec4f(1.0f, 0.0f, 0.0f, 1.0f),
			//	Vec2f(400.0f, 100.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f),
			//	Vec2f(250.0f, 300.0f), Vec4f(0.0f, 0.0f, 1.0f, 1.0f)
			//);
			////renderer->DrawTriangleWireframe(Vec2f(100.0f, 100.0f), Vec2f(400.0f, 100.0f), Vec2f(250.0f, 300.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
			//renderer->DrawTriangleWireframe(
			//	Vec2f(100.0f, 100.0f), Vec4f(0.0f, 0.0f, 1.0f, 1.0f),
			//	Vec2f(400.0f, 100.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f),
			//	Vec2f(250.0f, 300.0f), Vec4f(1.0f, 0.0f, 0.0f, 1.0f)
			//);

			static float time = 0.0f;
			time += deltaSeconds;


			//renderer->DrawRect(Vec2f(400.3f, 300.0f), 101.7f, 100.3f, Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
			renderer->DrawRectWireframe(Vec2f(400.3f, 300.0f), 101.7f, 100.3f, Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
			renderer->DrawRoundRect(Vec2f(400.3f, 300.0f), 101.7f, 100.3f, 10.0f, Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
			renderer->DrawRoundRectWireframe(Vec2f(400.3f, 300.0f), 101.7f, 100.3f, 10.0f, Vec4f(1.0f, 1.0f, 0.0f, 1.0f));

			RenderModule::EndFrame();
		}
	);

	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}