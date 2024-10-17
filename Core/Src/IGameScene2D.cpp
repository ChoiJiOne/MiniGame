#include "Assertion.h"
#include "EntityManager.h"
#include "IEntity.h"
#include "IEntity2D.h"
#include "IEntityUI.h"
#include "IGameScene2D.h"
#include "RenderManager2D.h"
#include "RenderStateManager.h"
#include "ResourceManager.h"
#include "UIManager.h"

IGameScene2D::IGameScene2D()
{
	entityMgr_ = EntityManager::GetPtr();
	renderStateMgr_ = RenderStateManager::GetPtr();
	render2dMgr_ = RenderManager2D::GetPtr();
	resourceMgr_ = ResourceManager::GetPtr();
	uiMgr_ = UIManager::GetPtr();
}

IGameScene2D::~IGameScene2D()
{
	uiMgr_ = nullptr;
	resourceMgr_ = nullptr;
	render2dMgr_ = nullptr;
	renderStateMgr_ = nullptr;
	entityMgr_ = nullptr;
}
