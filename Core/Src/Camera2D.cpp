#include "Assertion.h"
#include "Camera2D.h"
#include "EntityManager.h"
#include "RenderStateManager.h"
#include "IApp.h"

Camera2D::Camera2D(const Vec2f& center, const Vec2f& size)
	: center_(center)
	, size_(size)
{
	ortho_ = CalculateOrtho(center_, size_);
	collision_ = Rect2D(center_, size_);

	bIsInitialized_ = true;
}

Camera2D::~Camera2D()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Camera2D::Tick(float deltaSeconds)
{
	// nothing...
}

void Camera2D::Release()
{
	CHECK(bIsInitialized_);

	bIsInitialized_ = false;
}

Camera2D* Camera2D::CreateScreenCamera()
{
	Vec2f center;
	Vec2f size;
	RenderStateManager::GetRef().GetScreenSize<float>(size.x, size.y);

	return EntityManager::GetRef().Create<Camera2D>(center, size);
}
