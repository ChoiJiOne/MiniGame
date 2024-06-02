#include "Character.h"
#include "Light.h"

Light::Light(Character* character)
	: character_(character)
	, distance_(10.0f)
{
	position_ = GetPositionFromCharacter();
	direction_ = GetDirectionFromCharacter();

	view_ = Mat4x4::LookAt(position_, position_ + direction_, worldUp_);
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
	position_ = GetPositionFromCharacter();
	direction_ = GetDirectionFromCharacter();

	view_ = Mat4x4::LookAt(position_, position_ + direction_, worldUp_);
	projection_ = Mat4x4::Ortho(-10.0f, +10.0f, -10.0f, +10.0f, 0.1f, 100.0f);
}

void Light::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}

Vec3f Light::GetPositionFromCharacter()
{
	Vec3f position;
	position.y = distance_ * MathModule::Sin(45.0f);
	position.z = distance_ * MathModule::Cos(45.0f);

	position += character_->GetTransform().position;
	return position;
}

Vec3f Light::GetDirectionFromCharacter()
{
	Vec3f position;
	position.y = distance_ * MathModule::Sin(45.0f);
	position.z = distance_ * MathModule::Cos(45.0f);

	return Vec3f::Normalize(-position);
}
