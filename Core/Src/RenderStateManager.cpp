#include <glad/glad.h>

#include "Assertion.h"
#include "RenderStateManager.h"

RenderStateManager RenderStateManager::instance_;

RenderStateManager& RenderStateManager::GetRef()
{
	return instance_;
}

RenderStateManager* RenderStateManager::GetPtr()
{
	return &instance_;
}
