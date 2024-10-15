#include "Assertion.h"
#include "Camera2D.h"
#include "EntityManager.h"
#include "UIManager.h"

UIManager& UIManager::Get()
{
	static UIManager instance;
	return instance;
}

void UIManager::Startup()
{
	uiCamera_ = Camera2D::CreateScreenCamera();
}

void UIManager::Shutdown()
{
	EntityManager::Get().Destroy(uiCamera_);
	uiCamera_ = nullptr;
}