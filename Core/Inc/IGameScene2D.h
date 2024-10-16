#pragma once

#include <vector>

#include "IGameScene.h"

class IEntity;
class IEntity2D;
class IEntityUI;

class IGameScene2D : public IGameScene
{
public:
	IGameScene2D();
	virtual ~IGameScene2D();

	DISALLOW_COPY_AND_ASSIGN(IGameScene2D);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Render() = 0;
	virtual bool IsSceneSwitched() = 0;
	virtual IGameScene* GetSwitchScene() = 0;

protected:
	class RenderManager2D* render2dMgr_ = nullptr;
	class UIManager* uiMgr_ = nullptr;

	std::vector<IEntity*> updateEntities_;
	std::vector<IEntity2D*> renderEntities_;
	std::vector<IEntityUI*> uiEntities_;
};