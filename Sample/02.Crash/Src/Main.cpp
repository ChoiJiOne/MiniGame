#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include <imgui.h>

#include "Assertion.h"
#include "IApp.h"
#include "CrashManager.h"
#include "GameUtils.h"

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("02.Crash", 100, 100, 400, 100, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		CrashManager::Get().SetCrashCallbackEvent(
			[&]() 
			{
				::MessageBoxW(NULL, CrashManager::Get().GetCrashDumpFileNamePtr(), L"DETECT CRASH!", MB_OK);
			}
		);
	}

	virtual void Shutdown() override
	{
	}

	virtual void Run() override
	{
		RunLoop(
			[&](float deltaSeconds)
			{
				ImGui::Begin("CRASH", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
				ImGui::SetWindowSize(ImVec2(400.0f, 100.0f));
				if (ImGui::Button("CRASH!")) /** 고의로 크래시 유발! */
				{
					int32_t* a = nullptr;
					*a = 10;
				}
				ImGui::End();

				BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
				EndFrame();
			}
		);
	}

private:

};

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::unique_ptr<IApp> app = std::make_unique<DemoApp>();
	app->Startup();
	app->Run();
	app->Shutdown();
	return 0;
}