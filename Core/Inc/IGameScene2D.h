#pragma once

#include <vector>

#include "IGameScene.h"

class Camera2D;
class IEntity;
class IEntity2D;
class IEntityUI;

class IGameScene2D : public IGameScene
{
public:
	IGameScene2D();
	virtual ~IGameScene2D();

	DISALLOW_COPY_AND_ASSIGN(IGameScene2D);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	class EntityManager* entityMgr_ = nullptr;
	class RenderStateManager* renderStateMgr_ = nullptr;
	class RenderManager2D* render2dMgr_ = nullptr;
	class ResourceManager* resourceMgr_ = nullptr;
	class UIManager* uiMgr_ = nullptr;

	Camera2D* mainCamera_ = nullptr;

	std::vector<IEntity*> updateEntities_;
	std::vector<IEntity2D*> renderEntities_;
	std::vector<IEntityUI*> uiEntities_;
};