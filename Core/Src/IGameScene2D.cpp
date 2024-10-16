#include "Assertion.h"
#include "IEntity.h"
#include "IEntity2D.h"
#include "IEntityUI.h"
#include "IGameScene2D.h"
#include "RenderManager2D.h"
#include "UIManager.h"

IGameScene2D::IGameScene2D()
{
	render2dMgr_ = RenderManager2D::GetPtr();
	uiMgr_ = UIManager::GetPtr();
}

IGameScene2D::~IGameScene2D()
{
	uiMgr_ = nullptr;
	render2dMgr_ = nullptr;
}
