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
#include "ResourceManager.h"
#include "Sound.h"

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("10.Sound", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		jump_ = ResourceManager::Get().Create<Sound>("GameMaker\\Sample\\10.Sound\\Res\\Jump.mp3");
		up_ = ResourceManager::Get().Create<Sound>("GameMaker\\Sample\\10.Sound\\Res\\Up.wav");
	}

	virtual void Shutdown() override
	{
		ResourceManager::Get().Destroy(up_);
		ResourceManager::Get().Destroy(jump_);
	}

	virtual void Run() override
	{
		RunLoop(
			[&](float deltaSeconds)
			{
				DisplaySoundStatus(ImVec2(0.0f, 0.0f), ImVec2(400.0f, 100.0f), jump_, "JUMP", jumpVolume_, bIsLoopJump_);
				DisplaySoundStatus(ImVec2(0.0f, 100.0f), ImVec2(400.0f, 100.0f), up_, "UP", upVolume_, bIsLoopUp_);

				BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
				EndFrame();
			}
		);
	}

	void DisplaySoundStatus(const ImVec2& pos, const ImVec2& size, Sound* sound, const char* name, float& volume, bool& bIsLoop)
	{
		ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::SetWindowPos(pos);
		ImGui::SetWindowSize(size);

		if (ImGui::Button("PLAY"))
		{
			sound->Play();
		}

		ImGui::SameLine();
		if (ImGui::Button("STOP"))
		{
			sound->Stop();
		}

		ImGui::SameLine();
		if (ImGui::Button("RESET"))
		{
			sound->Reset();
		}

		ImGui::SameLine();
		ImGui::Text("LOOP");

		ImGui::SameLine();
		if (ImGui::Checkbox("##LOOP", &bIsLoop))
		{
			sound->SetLooping(bIsLoop);
		}

		ImGui::Text("VOLUME");

		ImGui::SameLine();
		ImGui::SliderFloat("##VOLUME", &volume, 0.0f, 1.0f);
		sound->SetVolume(volume);
		ImGui::End();
	}

private:
	Sound* jump_ = nullptr;
	float jumpVolume_ = 1.0f;
	bool bIsLoopJump_ = false;

	Sound* up_ = nullptr;
	float upVolume_ = 1.0f;
	bool bIsLoopUp_ = false;
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