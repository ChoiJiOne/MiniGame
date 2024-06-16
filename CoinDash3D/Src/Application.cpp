#include "Assertion.h"

#include "Application.h"

Application::Application()
{
	PlatformModule::WindowConstructParams windowParam{ L"CoinDash3D", 100, 100, 800, 600, false, false };

	ASSERT(CrashModule::Init() == CrashModule::Errors::OK, "Failed to initialize CrashModule.");
	ASSERT(PlatformModule::Init(windowParam) == PlatformModule::Errors::OK, "Failed to initialize PlatformModule.");
	ASSERT(AudioModule::Init() == AudioModule::Errors::OK, "Failed to initialize AudioModule.");
	ASSERT(RenderModule::Init(PlatformModule::GetWindowHandle()) == RenderModule::Errors::OK, "Failed to initialize RenderModule.");
	ASSERT(GameModule::Init() == GameModule::Errors::OK, "Failed to initialize GameModule.");

	PlatformModule::SetEndLoopCallback([&]() { RenderModule::Uninit(); });
}

Application::~Application()
{
	GameModule::Uninit();
	AudioModule::Uninit();
	PlatformModule::Uninit();
	CrashModule::Uninit();
}

void Application::Init()
{
}

void Application::Run()
{
	PlatformModule::RunLoop(
		[&](float deltaSeconds)
		{
			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
			RenderModule::EndFrame();
		}
	);
}
