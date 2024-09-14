#include <glad/glad.h>

#include "Assertion.h"
#include "RenderManager3D.h"
#include "ResourceManager.h"
#include "Shader.h"

RenderManager3D& RenderManager3D::Get()
{
	static RenderManager3D instance;
	return instance;
}

void RenderManager3D::Startup()
{
	app_ = IApp::Get();
}

void RenderManager3D::Shutdown()
{
	app_ = nullptr;
}
