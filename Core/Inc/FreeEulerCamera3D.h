#pragma once

#include "Camera3D.h"

class RenderStateManager;
class InputManager;

/** 마우스로 움직이는 것이 가능한 3D 오일러 카메라 */
class FreeEulerCamera3D : public Camera3D
{
public:
	enum class EMovement
	{
		FORWARD = 0x00,
		BACKWARD = 0x01,
		LEFT = 0x02,
		RIGHT = 0x03,
	};

public:
	/** 모든 각도는 라디안 각도 기준입니다. */
	FreeEulerCamera3D(const Vec3f& position, float yaw, float pitch, float fov, float nearZ, float farZ);
	FreeEulerCamera3D(const Vec3f& position, float yaw, float pitch, float aspectRatio, float fov, float nearZ, float farZ);
	virtual ~FreeEulerCamera3D();

	DISALLOW_COPY_AND_ASSIGN(FreeEulerCamera3D);

	virtual void Tick(float deltaSeconds) override;
	virtual void Release() override;

private:
	void UpdateDirectionState();

private:
	static RenderStateManager* renderStateMgr_;
	static InputManager* inputMgr_;

	float speed_ = 2.5f;
	float yaw_ = 0.0f; /** y축 오일러 회전 각도 */
	float pitch_ = 0.0f; /** x축 오일러 회전 각도 */
};