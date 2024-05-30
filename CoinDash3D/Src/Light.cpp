#include "Light.h"

Light::Light()
{
	position_ = Vec3f(10.0f, 10.0f, 10.0f);
	direction_ = Vec3f::Normalize(Vec3f(-1.0f, -5.0f, -1.0f));

	view_ = Mat4x4::LookAt(position_, direction_, worldUp_);
	projection_ = Mat4x4::Ortho(-10.0f, +10.0f, -10.0f, +10.0f, 0.1f, 100.0f);

	bIsInitialized_ = true;
}

Light::~Light()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Light::Tick(float deltaSeconds)
{
}

void Light::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}