#pragma once

#include "GameMath.h"
#include "IEntity.h"

class Camera3D : public IEntity
{
public:
	Camera3D() = default;
	virtual ~Camera3D() {}

	DISALLOW_COPY_AND_ASSIGN(Camera3D);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Release() = 0;

	const GameMath::Vec3f& GetPosition() const { return position_; }
	const GameMath::Vec3f& GetDirection() const { return direction_; }
	const GameMath::Vec3f& GetUpDirection() const { return upDirection_; }
	const GameMath::Vec3f& GetRightDirection() const { return rightDirection_; }
	const GameMath::Vec3f& GetWorldupDirection() const { return worldUpDirection_; }
	const float& GetFov() const { return fov_; }
	const float& GetAspectRatio() const { return aspectRatio_; }
	const float& GetNearZ() const { return nearZ_; }
	const float& GetFarZ() const { return farZ_; }
	const GameMath::Mat4x4& GetView() const { return view_; }
	const GameMath::Mat4x4& GetProjection() const { return projection_; }

protected:
	GameMath::Vec3f position_;
	GameMath::Vec3f direction_;
	GameMath::Vec3f upDirection_;
	GameMath::Vec3f rightDirection_;
	GameMath::Vec3f worldUpDirection_ = GameMath::Vec3f(0.0f, 1.0f, 0.0f);
	float fov_ = 0.0f;
	float aspectRatio_ = 0.0f;
	float nearZ_ = 0.0f;
	float farZ_ = 0.0f;
	GameMath::Mat4x4 view_;
	GameMath::Mat4x4 projection_;
};