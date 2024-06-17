#include <array>

#include "Assertion.h"
#include "FrameBuffer.h"
#include "GameModule.h"
#include "GeometryRenderer2D.h"
#include "PlatformModule.h"
#include "PostEffectComposer.h"
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
	framebuffer_ = app->framebuffer_;
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
				bIsStartGame_ = true;
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
	bIsStartGame_ = false;
	changeCountdown_ = maxChangeCountdown_;
	bIsEnter_ = true;
}

void StartScene::Exit()
{
	bIsEnter_ = false;
	bDetectSwitch_ = false;
}

void StartScene::Update(float deltaSeconds)
{
	if (bIsStartGame_)
	{
		changeCountdown_ -= deltaSeconds;
		if (changeCountdown_ <= 0.0f)
		{
			SetLink(playScene_);
			bDetectSwitch_ = true;
		}
		return;
	}

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

	framebuffer_->Bind();
	{
		framebuffer_->Clear(0.53f, 0.81f, 0.98f, 1.0f);
		RenderModule::SetWindowViewport();

		for (auto& entity : entities)
		{
			entity->Render();
		}
	}
	framebuffer_->Unbind();

	RenderModule::BeginFrame(0.53f, 0.81f, 0.98f, 1.0f);

	if (bIsStartGame_)
	{
		float rate = changeCountdown_ / maxChangeCountdown_;
		postEffectComposer_->Fade(framebuffer_, 0, rate);
	}
	else
	{
		postEffectComposer_->Blit(framebuffer_, 0);
	}

	RenderModule::EndFrame();
}
