#include <array>

#include "Assertion.h"
#include "GameModule.h"
#include "GeometryRenderer2D.h"
#include "PlatformModule.h"
#include "TextRenderer.h"

#include "Application.h"
#include "Button.h"
#include "Panel.h"
#include "StartScene.h"

StartScene::StartScene(Application* app)
{
	geometryRenderer2D_ = app->geometryRenderer2D_;
	textRenderer_ = app->textRenderer_;
	postEffectComposer_ = app->postEffectComposer_;
	fonts_ = app->fonts_;
}

void StartScene::Tick(float deltaSeconds)
{
	Update(deltaSeconds);
	PrepareForRendering();
	Render();
}

void StartScene::Enter()
{
	static Panel* titlePanel = nullptr;
	if (!titlePanel)
	{
		titlePanel = GameModule::CreateEntity<Panel>("Resource/Panel/Title.json", fonts_[72], geometryRenderer2D_, textRenderer_);
	}

	static Button* startButton = nullptr;
	if (!startButton)
	{
		auto clickEvent = [&]() 
			{
				SetLink(playScene_);
				bDetectSwitch_ = true;
			};
		startButton = GameModule::CreateEntity<Button>("Resource/Button/Start.json", fonts_[32], EMouseButton::LEFT, clickEvent, geometryRenderer2D_, textRenderer_);
	}

	static Button* quitButton = nullptr;
	if (!quitButton)
	{
		auto clickEvent = [&]() { PlatformModule::SetQuitLoop(true); };
		quitButton = GameModule::CreateEntity<Button>("Resource/Button/Quit.json", fonts_[32], EMouseButton::LEFT, clickEvent, geometryRenderer2D_, textRenderer_);
	}

	titlePanel_ = titlePanel;
	startButton_ = startButton;
	quitButton_ = quitButton;
	bIsEnter_ = true;
}

void StartScene::Exit()
{
	bIsEnter_ = false;
	bDetectSwitch_ = false;
}

void StartScene::Update(float deltaSeconds)
{
	std::array<IEntity*, 2> entities =
	{
		startButton_,
		quitButton_,
	};

	for (auto& entity : entities)
	{
		entity->Tick(deltaSeconds);
	}
}

void StartScene::PrepareForRendering()
{
	Mat4x4 screenOrtho = RenderModule::GetScreenOrtho();

	geometryRenderer2D_->SetOrtho(screenOrtho);
	textRenderer_->SetOrtho(screenOrtho);
}

void StartScene::Render()
{
	std::array<Entity2D*, 3> entities =
	{
		titlePanel_,
		startButton_,
		quitButton_,
	};

	RenderModule::BeginFrame(0.53f, 0.81f, 0.98f, 1.0f);

	for (auto& entity : entities)
	{
		entity->Render();
	}

	RenderModule::EndFrame();
}
