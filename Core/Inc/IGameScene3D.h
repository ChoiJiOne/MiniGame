#pragma once

#include <map>

#include "IGameScene.h"

class Camera3D;
class IEntity;
class IEntity2D;
class IEntity3D;
class IEntityUI;

class IGameScene3D : public IGameScene 
{
public:
	IGameScene3D();
	virtual ~IGameScene3D();

	DISALLOW_COPY_AND_ASSIGN(IGameScene3D);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() = 0;
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	class EntityManager* entityMgr_ = nullptr;
	class RenderStateManager* renderStateMgr_ = nullptr;
	class RenderManager3D* render3dMgr_ = nullptr;
	class RenderManager2D* render2dMgr_ = nullptr;
	class UIManager* uiMgr_ = nullptr;
	class ResourceManager* resourceMgr_ = nullptr;

	Camera3D* mainCamera_ = nullptr;

	std::map<std::string, IEntity*> updateEntities_;
	std::map<std::string, IEntity3D*> render3dEntities_;
	std::map<std::string, IEntity2D*> render2dEntities_;
	std::map<std::string, IEntityUI*> uiEntities_;
};