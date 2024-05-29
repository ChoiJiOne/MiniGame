#include "MathModule.h"
#include "InputController.h"
#include "RenderModule.h"

#include "Camera.h"
#include "Character.h"

Camera::Camera(Character* character)
	: character_(character)
	, distance_(10.0f)
{
	yaw_ = MathModule::ToRadian(-90.0f);
	pitch_ = MathModule::ToRadian(-45.0f);
	eyePosition_ = GetEyePositionFromCharacter();
	UpdateState();

	int32_t screenWidth = 0;
	int32_t screenHeight = 0;
	RenderModule::GetScreenSize(screenWidth, screenHeight);
	aspectRatio_ = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	fov_ = PiDiv4;
	nearZ_ = 0.01f;
	farZ_ = 100.0f;
	projection_ = Mat4x4::Perspective(fov_, aspectRatio_, nearZ_, farZ_);

	minPitch_ = -PiDiv2 + 0.1f;
	maxPitch_ = +PiDiv2 - 0.1f;

	bIsInitialized_ = true;
}

Camera::~Camera()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Camera::Tick(float deltaSeconds)
{
	eyePosition_ = GetEyePositionFromCharacter();
	UpdateState();
}

void Camera::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}

Vec3f Camera::GetEyePositionFromCharacter()
{
	Vec3f position;
	position.y = distance_ * MathModule::Sin(-pitch_);
	position.z = distance_ * MathModule::Cos(-pitch_);

	position += character_->GetTransform().position;
	return position;
}

void Camera::UpdateState()
{
	Vec3f direction;
	direction.x = MathModule::Cos(yaw_) * MathModule::Cos(pitch_);
	direction.y = MathModule::Sin(pitch_);
	direction.z = MathModule::Sin(yaw_) * MathModule::Cos(pitch_);

	eyeDirection_ = Vec3f::Normalize(direction);
	rightDirection_ = Vec3f::Normalize(Vec3f::Cross(eyeDirection_, worldUpDirection_));
	upDirection_ = Vec3f::Normalize(Vec3f::Cross(rightDirection_, eyeDirection_));

	view_ = Mat4x4::LookAt(eyePosition_, eyePosition_ + eyeDirection_, upDirection_);
}