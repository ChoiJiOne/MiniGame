#include <glad/glad.h>

#include "Assertion.h"
#include "RenderManager3D.h"
#include "Shader.h"

RenderManager3D RenderManager3D::instance_;

RenderManager3D& RenderManager3D::GetRef()
{
	return instance_;
}

RenderManager3D* RenderManager3D::GetPtr()
{
	return &instance_;
}

void RenderManager3D::Startup()
{
	renderStateMgr_ = RenderStateManager::GetPtr();
}

void RenderManager3D::Shutdown()
{
	renderStateMgr_ = nullptr;
}
