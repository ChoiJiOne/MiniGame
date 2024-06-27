#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include <imgui.h>
#include <glad/glad.h>

#include "Assertion.h"
#include "CrashModule.h"
#include "PlatformModule.h"
#include "RenderModule.h"

#include "Mat.h"
#include "Renderer2D.h"
#include "TTFont.h"

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

	TTFont* font = RenderModule::CreateResource<TTFont>("Resource/Font/SeoulNamsanEB.ttf", 0x0, 0x127, 32.0f);
	Renderer2D* renderer = RenderModule::CreateResource<Renderer2D>();
	
	std::vector<Vec2f> positions;
	std::vector<Vec4f> colors;

	for (float x = -400.0f; x <= 400.0f; x += 10.0f)
	{
		positions.push_back(Vec2f(x, -300.0f));
		positions.push_back(Vec2f(x, +300.0f));

		if (MathModule::NearZero(x))
		{
			colors.push_back(Vec4f(1.0f, 0.0f, 0.0f, 0.5f));
			colors.push_back(Vec4f(1.0f, 0.0f, 0.0f, 0.5f));
		}
		else
		{
			colors.push_back(Vec4f(1.0f, 1.0f, 1.0f, 0.5f));
			colors.push_back(Vec4f(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}

	for (float y = -300.0f; y <= 300.0f; y += 10)
	{
		positions.push_back(Vec2f(-400.0f, y));
		positions.push_back(Vec2f(+400.0f, y));

		if (MathModule::NearZero(y))
		{
			colors.push_back(Vec4f(0.0f, 0.0f, 1.0f, 0.5f));
			colors.push_back(Vec4f(0.0f, 0.0f, 1.0f, 0.5f));
		}
		else
		{
			colors.push_back(Vec4f(1.0f, 1.0f, 1.0f, 0.5f));
			colors.push_back(Vec4f(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}

	int32_t width = 0;
	int32_t height = 0;
	RenderModule::GetScreenSize(width, height);

	PlatformModule::RunLoop(
		[&](float deltaSeconds)
		{
			renderer->SetOrtho(Mat4x4::Ortho(-width * 0.5f, +width * 0.5f, -height * 0.5f, +height * 0.5f, -1.0f, 1.0f));

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer->DrawLines(positions.data(), colors.data(), positions.size());
			renderer->DrawLine(Vec2f(-200.0f, -200.0f), Vec2f(+200.0f, +200.0f), Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
			//renderer->DrawCircleWireframe(Vec2f(400.3f, 300.0f), 50.3f, Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
			//renderer->DrawEllipse(Vec2f(400.3f, 300.0f), 50.0f, 200.0f, Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
			//renderer->DrawEllipseWireframe(Vec2f(400.3f, 300.0f), 50.0f, 200.0f, Vec4f(1.0f, 0.0f, 1.0f, 1.0f));

			//renderer->DrawString(font, L"Hello, World", Vec2f(100.0f, 100.0f), Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

			RenderModule::EndFrame();
		}
	);

	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}