#include "Assertion.h"
#include "EntityManager.h"
#include "IEntity.h"
#include "IEntity2D.h"
#include "IEntity3D.h"
#include "IEntityUI.h"
#include "IGameScene3D.h"
#include "RenderManager2D.h"
#include "RenderManager3D.h"
#include "RenderStateManager.h"
#include "ResourceManager.h"
#include "UIManager.h"

IGameScene3D::IGameScene3D()
{
	entityMgr_ = EntityManager::GetPtr();
	renderStateMgr_ = RenderStateManager::GetPtr();
	render2dMgr_ = RenderManager2D::GetPtr();
	render3dMgr_ = RenderManager3D::GetPtr();
	resourceMgr_ = ResourceManager::GetPtr();
	uiMgr_ = UIManager::GetPtr();
}

IGameScene3D::~IGameScene3D()
{
	uiMgr_ = nullptr;
	resourceMgr_ = nullptr;
	render3dMgr_ = nullptr;
	render2dMgr_ = nullptr;
	renderStateMgr_ = nullptr;
	entityMgr_ = nullptr;
}

void IGameScene3D::Tick(float deltaSeconds)
{
	for (auto& updateEntity : updateEntities_)
	{
		updateEntity.second->Tick(deltaSeconds);
	}

	for (auto& uiEntity : uiEntities_)
	{
		uiEntity.second->Tick(deltaSeconds);
	}
}

void IGameScene3D::Enter()
{
	bIsEnter_ = true;
}

void IGameScene3D::Exit()
{
	bIsSwitched_ = false;
	bIsEnter_ = false;
}