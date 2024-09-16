#pragma once

#include "GameMath.h"
#include "IEntity.h"

class Camera2D : public IEntity
{
public:
	Camera2D() = default;
	virtual ~Camera2D() {}

	DISALLOW_COPY_AND_ASSIGN(Camera2D);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Release() = 0;

	const GameMath::Vec2f& GetCenter() const { return center_; }
	const GameMath::Vec2f& GetSize() const { return size_; }
	const float& GetWidth() const { return size_.x; }
	const float& GetHeight() const { return size_.y; }
	const GameMath::Mat4x4& GetOrtho() const { return ortho_; }

protected:
	GameMath::Mat4x4 CalculateOrtho(const GameMath::Vec2f& center, const GameMath::Vec2f& size, float zNear = -1.0f, float zFar = 1.0f)
	{
		return GameMath::Mat4x4::Ortho(-size.x * 0.5f + center.x, +size.x * 0.5f + center.x, -size.y * 0.5f + center.y, +size.y * 0.5f + center.y, zNear, zFar);
	}

protected:
	GameMath::Vec2f center_;
	GameMath::Vec2f size_;
	GameMath::Mat4x4 ortho_;
};