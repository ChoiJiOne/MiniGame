#include <array>
#include <utility>

#include "Assertion.h"
#include "FreeEulerCamera3D.h"
#include "IApp.h"

static IApp* appPtr = nullptr;

FreeEulerCamera3D::FreeEulerCamera3D(const GameMath::Vec3f& position, float yaw, float pitch, float fov, float nearZ, float farZ)
	: yaw_(yaw)
	, pitch_(pitch)
{
	if (!appPtr)
	{
		appPtr = IApp::Get();
	}

	position_ = position;
	yaw_ = yaw;
	pitch_ = pitch;
	UpdateDirectionState();

	float width = 0;
	float height = 0;
	appPtr->GetScreenSize<float>(width, height);
	aspectRatio_ = width / height;

	fov_ = fov;
	nearZ_ = nearZ;
	farZ_ = farZ;
	projection_ = GameMath::Mat4x4::Perspective(fov_, aspectRatio_, nearZ_, farZ_);

	bIsInitialized_ = true;
}

FreeEulerCamera3D::FreeEulerCamera3D(const GameMath::Vec3f& position, float yaw, float pitch, float aspectRatio, float fov, float nearZ, float farZ)
	: yaw_(yaw)
	, pitch_(pitch)
{
	position_ = position;
	yaw_ = yaw;
	pitch_ = pitch;
	UpdateDirectionState();

	aspectRatio_ = aspectRatio;
	fov_ = fov;
	nearZ_ = nearZ;
	farZ_ = farZ;
	projection_ = GameMath::Mat4x4::Perspective(fov_, aspectRatio_, nearZ_, farZ_);

	bIsInitialized_ = true;
}

FreeEulerCamera3D::~FreeEulerCamera3D()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void FreeEulerCamera3D::Tick(float deltaSeconds)
{
	bool bIsUpdateState = false;
	if (appPtr->GetMousePress(Mouse::LEFT) == Press::HELD)
	{
		GameMath::Vec2i prev = appPtr->GetPrevMousePos();
		GameMath::Vec2i curr = appPtr->GetCurrMousePos();

		float xoffset = static_cast<float>(curr.x - prev.x);
		float yoffset = static_cast<float>(prev.y - curr.y);

		xoffset *= 0.01f;
		yoffset *= 0.01f;

		yaw_ += xoffset;
		pitch_ += yoffset;

		pitch_ = GameMath::Clamp<float>(pitch_, -GameMath::PI_DIV_2 + 0.1f, +GameMath::PI_DIV_2 - 0.1f);
		bIsUpdateState = true;
	}

	std::array<std::pair<Key, GameMath::Vec3f>, 4> keyDirections =
	{
		std::pair{ Key::KEY_W,       direction_ },
		std::pair{ Key::KEY_S,      -direction_ },
		std::pair{ Key::KEY_A, -rightDirection_ },
		std::pair{ Key::KEY_D,  rightDirection_ },
	};

	for (const auto& keyDirection : keyDirections)
	{
		if (appPtr->GetKeyPress(keyDirection.first) == Press::HELD)
		{
			position_ += keyDirection.second * deltaSeconds * speed_;
			bIsUpdateState = true;
		}
	}

	if (bIsUpdateState)
	{
		UpdateDirectionState();
	}
}

void FreeEulerCamera3D::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}

void FreeEulerCamera3D::UpdateDirectionState()
{
	GameMath::Vec3f direction;
	direction.x = GameMath::Cos(yaw_) * GameMath::Cos(pitch_);
	direction.y = GameMath::Sin(pitch_);
	direction.z = GameMath::Sin(yaw_) * GameMath::Cos(pitch_);

	direction_ = GameMath::Vec3f::Normalize(direction);
	rightDirection_ = GameMath::Vec3f::Normalize(GameMath::Vec3f::Cross(direction_, worldUpDirection_));
	upDirection_ = GameMath::Vec3f::Normalize(GameMath::Vec3f::Cross(rightDirection_, direction_));

	view_ = GameMath::Mat4x4::LookAt(position_, position_ + direction_, upDirection_);
}