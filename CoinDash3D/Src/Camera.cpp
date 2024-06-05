#include "MathModule.h"
#include "InputController.h"
#include "RenderModule.h"

#include "Camera.h"

Camera::Camera()
{
	yaw_ = MathModule::ToRadian(-90.0f);
	pitch_ = MathModule::ToRadian(-30.0f);
	eyePosition_ = Vec3f(0.0f, 3.0f, 4.0f);
	UpdateState();

	int32_t screenWidth = 0;
	int32_t screenHeight = 0;
	RenderModule::GetScreenSize(screenWidth, screenHeight);
	aspectRatio_ = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	fov_ = PiDiv4;
	nearZ_ = 0.01f;
	farZ_ = 100.0f;
	projection_ = Mat4x4::Perspective(fov_, aspectRatio_, nearZ_, farZ_);
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
	if (!bIsActive_)
	{
		return;
	}

	bool bIsUpdateState = false;

	if (InputController::GetKeyPressState(EKey::KEY_LBUTTON) == EPressState::HELD)
	{
		CursorPos prev = InputController::GetPrevCursorPos();
		CursorPos curr = InputController::GetCurrCursorPos();

		float xoffset = static_cast<float>(curr.x - prev.x);
		float yoffset = static_cast<float>(prev.y - curr.y);

		xoffset *= 0.01f;
		yoffset *= 0.01f;

		yaw_ += xoffset;
		pitch_ += yoffset;

		pitch_ = MathModule::Clamp<float>(pitch_, -PiDiv2 + 0.1f, +PiDiv2 - 0.1f);
		bIsUpdateState = true;
	}

	if (InputController::GetKeyPressState(EKey::KEY_W) == EPressState::HELD)
	{
		eyePosition_ += eyeDirection_ * deltaSeconds * speed_;
		bIsUpdateState = true;
	}

	if (InputController::GetKeyPressState(EKey::KEY_S) == EPressState::HELD)
	{
		eyePosition_ -= eyeDirection_ * deltaSeconds * speed_;
		bIsUpdateState = true;
	}

	if (InputController::GetKeyPressState(EKey::KEY_A) == EPressState::HELD)
	{
		eyePosition_ -= rightDirection_ * deltaSeconds * speed_;
		bIsUpdateState = true;
	}

	if (InputController::GetKeyPressState(EKey::KEY_D) == EPressState::HELD)
	{
		eyePosition_ += rightDirection_ * deltaSeconds * speed_;
		bIsUpdateState = true;
	}

	if (bIsUpdateState)
	{
		UpdateState();
	}
}

void Camera::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
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