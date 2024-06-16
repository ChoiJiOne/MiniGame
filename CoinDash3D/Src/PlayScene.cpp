#include "Assertion.h"
#include "DepthRenderer.h"
#include "GeometryRenderer2D.h"
#include "GeometryRenderer3D.h"

#include "Application.h"
#include "PlayScene.h"

PlayScene::PlayScene(Application* app)
{
	depthRenderer_ = app->depthRenderer_;
	geometryRenderer2D_ = app->geometryRenderer2D_;
	geometryRenderer3D_ = app->geometryRenderer3D_;
	meshRenderer_ = app->meshRenderer_;
	textRenderer_ = app->textRenderer_;
}

void PlayScene::Tick(float deltaSeconds)
{
	RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
	RenderModule::EndFrame();
}

void PlayScene::Enter()
{
	bIsEnter_ = true;
}

void PlayScene::Exit()
{
	bIsEnter_ = false;
	bDetectSwitch_ = false;
}
