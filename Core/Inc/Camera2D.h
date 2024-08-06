#pragma once

#include "GameMath.h"
#include "IEntity.h"


namespace GameMaker
{
class Camera2D : public IEntity
{
public:
	Camera2D() = default;
	virtual ~Camera2D() {}

	DISALLOW_COPY_AND_ASSIGN(Camera2D);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Release() = 0;

	const Vec2f& GetCenter() const { return center_; }
	const float& GetWidth() const { return width_; }
	const float& GetHeight() const { return height_; }
	const Mat4x4& GetOrtho() const { return ortho_; }

protected:
	Mat4x4 CalculateOrtho(const Vec2f& center, float width, float height, float zNear = -1.0f, float zFar = 1.0f)
	{
		return Mat4x4::Ortho(
			-width * 0.5f + center.x, +width * 0.5f + center.x,
			-height * 0.5f + center.y, +height * 0.5f + center.y,
			zNear, zFar
		);
	}

protected:
	Vec2f center_;
	float width_ = 0.0f;
	float height_ = 0.0f;
	Mat4x4 ortho_;
};

}