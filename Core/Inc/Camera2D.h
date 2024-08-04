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
	Vec2f center_;
	float width_ = 0.0f;
	float height_ = 0.0f;
	Mat4x4 ortho_;
};

}