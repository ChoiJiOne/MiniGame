#include <glad/glad.h>

#include "Assertion.h"
#include "RenderManager3D.h"
#include "ResourceManager.h"
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
	app_ = IApp::Get();
}

void RenderManager3D::Shutdown()
{
	app_ = nullptr;
}
