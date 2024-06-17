#include "Assertion.h"
#include "GeometryRenderer2D.h"
#include "TextRenderer.h"

#include "Application.h"
#include "Button.h"
#include "StartScene.h"

StartScene::StartScene(Application* app)
{
	geometryRenderer2D_ = app->geometryRenderer2D_;
	textRenderer_ = app->textRenderer_;
	fonts_ = app->fonts_;
}

void StartScene::Tick(float deltaSeconds)
{
	RenderModule::BeginFrame(0.53f, 0.81f, 0.98f, 1.0f);
	
	RenderModule::EndFrame();
}

void StartScene::Enter()
{
	bIsEnter_ = true;
}

void StartScene::Exit()
{
	bIsEnter_ = false;
	bDetectSwitch_ = false;
}