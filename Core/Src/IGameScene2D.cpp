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

void IGameScene2D::Tick(float deltaSeconds)
{
	for (auto& entity : updateEntities_)
	{
		entity->Tick(deltaSeconds);
	}

	for (auto& uiEntity : uiEntities_)
	{
		uiEntity->Tick(deltaSeconds);
	}
}

void IGameScene2D::Render()
{
	renderStateMgr_->BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
	{// 2D ¿£Æ¼Æ¼ ·»´õ¸µ
		render2dMgr_->Begin(mainCamera_);
		{
			for (auto& entity : renderEntities_)
			{
				entity->Render();
			}
		}
		render2dMgr_->End();
	}
	{// UI ·»´õ¸µ
		IEntityUI** uiEntities = uiEntities_.data();
		uint32_t uiEntityCount = static_cast<uint32_t>(uiEntities_.size());
		uiMgr_->BatchRenderUIEntity(uiEntities, uiEntityCount);
	}
	renderStateMgr_->EndFrame();
}

void IGameScene2D::Enter()
{
	bIsEnter_ = true;
}

void IGameScene2D::Exit()
{
	bIsSwitched_ = false;
	bIsEnter_ = false;
}
