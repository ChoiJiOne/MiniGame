#include "InputManager.h"
#include "RenderManager.h"

#include "Camera.h"

Camera::Camera()
{
	yaw_ = GameMaker::ToRadian(-90.0f);
	pitch_ = GameMaker::ToRadian(-30.0f);
	position_ = GameMaker::Vec3f(0.0f, 3.0f, 4.0f);
	UpdateState();

	float screenWidth = 0;
	float screenHeight = 0;
	GameMaker::RenderManager::Get().GetScreenSize<float>(screenWidth, screenHeight);

	aspectRatio_ = screenWidth / screenHeight;
	fov_ = GameMaker::PI_DIV_4;
	nearZ_ = 0.01f;
	farZ_ = 100.0f;
	projection_ = GameMaker::Mat4x4::Perspective(fov_, aspectRatio_, nearZ_, farZ_);
	screenOrtho_ = GameMaker::Mat4x4::Ortho(-screenWidth * 0.5f, +screenWidth * 0.5f, -screenHeight * 0.5f, +screenHeight * 0.5f, -1.0f, 1.0f);
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

	if (GameMaker::InputManager::Get().GetMousePressState(GameMaker::EMouse::LEFT) == GameMaker::EPressState::HELD)
	{
		GameMaker::Vec2i prev = GameMaker::InputManager::Get().GetPrevMousePos();
		GameMaker::Vec2i curr = GameMaker::InputManager::Get().GetCurrMousePos();

		float xoffset = static_cast<float>(curr.x - prev.x);
		float yoffset = static_cast<float>(prev.y - curr.y);

		xoffset *= 0.01f;
		yoffset *= 0.01f;

		yaw_ += xoffset;
		pitch_ += yoffset;

		pitch_ = GameMaker::Clamp<float>(pitch_, -GameMaker::PI_DIV_2 + 0.1f, +GameMaker::PI_DIV_2 - 0.1f);
		bIsUpdateState = true;
	}
	
	if (GameMaker::InputManager::Get().GetKeyPressState(GameMaker::EKey::KEY_W) == GameMaker::EPressState::HELD)
	{
		position_ += direction_ * deltaSeconds * speed_;
		bIsUpdateState = true;
	}

	if (GameMaker::InputManager::Get().GetKeyPressState(GameMaker::EKey::KEY_S) == GameMaker::EPressState::HELD)
	{
		position_ -= direction_ * deltaSeconds * speed_;
		bIsUpdateState = true;
	}

	if (GameMaker::InputManager::Get().GetKeyPressState(GameMaker::EKey::KEY_A) == GameMaker::EPressState::HELD)
	{
		position_ -= rightDirection_ * deltaSeconds * speed_;
		bIsUpdateState = true;
	}

	if (GameMaker::InputManager::Get().GetKeyPressState(GameMaker::EKey::KEY_D) == GameMaker::EPressState::HELD)
	{
		position_ += rightDirection_ * deltaSeconds * speed_;
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
	GameMaker::Vec3f direction;
	direction.x = GameMaker::Cos(yaw_) * GameMaker::Cos(pitch_);
	direction.y = GameMaker::Sin(pitch_);
	direction.z = GameMaker::Sin(yaw_) * GameMaker::Cos(pitch_);

	direction_ = GameMaker::Vec3f::Normalize(direction);
	rightDirection_ = GameMaker::Vec3f::Normalize(GameMaker::Vec3f::Cross(direction_, worldUpDirection_));
	upDirection_ = GameMaker::Vec3f::Normalize(GameMaker::Vec3f::Cross(rightDirection_, direction_));

	view_ = GameMaker::Mat4x4::LookAt(position_, position_ + direction_, upDirection_);
}