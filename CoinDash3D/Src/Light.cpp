#include "MathModule.h"

#include "Camera.h"
#include "Light.h"

Light::Light(Camera* camera)
	: camera_(camera)
{
	direction_ = Vec3f::Normalize(Vec3f(0.0f, -2.0f, -1.0f));
	color_ = Vec3f(1.0f, 1.0f, 1.0f);

	Vec3f eyePosition = camera_->GetEyePosition();
	position_ = eyePosition;

	view_ = Mat4x4::LookAt(position_, position_ + direction_, Vec3f(0.0f, 1.0f, 0.0f));
	projection_ = Mat4x4::Ortho(-10.0f, +10.0f, -10.0f, +10.0f, 1.0f, 50.0f);
	lightSpaceMatrix_ = view_ * projection_;

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
	Vec3f eyePosition = camera_->GetEyePosition();
	position_ = eyePosition;

	view_ = Mat4x4::LookAt(position_, position_ + direction_, Vec3f(0.0f, 1.0f, 0.0f));
	projection_ = Mat4x4::Ortho(-10.0f, +10.0f, -10.0f, +10.0f, 1.0f, 50.0f);
	lightSpaceMatrix_ = view_ * projection_;
}

void Light::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}