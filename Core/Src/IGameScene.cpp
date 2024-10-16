#include "Assertion.h"
#include "IApp.h"
#include "IGameScene.h"

IGameScene::IGameScene()
{
	app_ = IApp::Get();
}

IGameScene::~IGameScene()
{
	app_ = nullptr;
}
