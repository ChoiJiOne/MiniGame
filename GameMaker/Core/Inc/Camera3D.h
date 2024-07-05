#pragma once

#include "GameMath.h"
#include "IEntity.h"


namespace GameMaker
{
/**
 * @brief 3D 카메라 엔티티의 인터페이스입니다.
 */
class Camera3D : public IEntity
{
public:
	/**
	 * @brief 3D 카메라의 기본 생성자입니다.
	 */
	Camera3D() = default;


	/**
	 * @brief 카메라 엔티티의 가상 소멸자입니다.
	 */
	virtual ~Camera3D() {}


	/**
	 * @brief 3D 카메라의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Camera3D);


	/**
	 * @brief 카메라 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds) = 0;


	/**
	 * @brief 카메라 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release() = 0;

	
	/**
	 * @brief 카메라의 월드 상 위치를 얻습니다.
	 * 
	 * @return 카메라의 월드 상 위치를 반환합니다.
	 */
	const Vec3f& GetPosition() const { return position_; }


	/**
	 * @brief 카메라의 방향을 얻습니다.
	 * 
	 * @return 카메라의 방향을 반환합니다.
	 */
	const Vec3f& GetDirection() const { return direction_; }


	/**
	 * @brief 카메라의 위 방향을 얻습니다.
	 * 
	 * @return 카메라의 위 방향을 반환합니다.
	 */
	const Vec3f& GetUpDirection() const { return upDirection_; }


	/**
	 * @brief 카메라의 오른쪽 방향을 얻습니다.
	 * 
	 * @return 카메라의 오른쪽 방향을 반환합니다.
	 */
	const Vec3f& GetRightDirection() const { return rightDirection_; }
	

	/**
	 * @brief 월드 상의 카메라 위 방향을 얻습니다.
	 * 
	 * @return 월드 상의 카메라 위 방향을 반환합니다.
	 */
	const Vec3f& GetWorldupDirection() const { return worldUpDirection_; }


	/**
	 * @breif 하향식 보기 필드 각도(라디안)를 얻습니다.
	 * 
	 * @return 하향식 보기 필드 각도(라디안)를 반환합니다.
	 */
	const float& GetFov() const { return fov_; }


	/**
	 * @brief 뷰 공간 X:Y의 가로 세로 비율을 얻습니다.
	 * 
	 * @return 뷰 공간 X:Y의 가로 세로 비율을 반환합니다.
	 */
	const float& GetAspectRatio() const { return aspectRatio_; }


	/**
	 * @brief 가까운 클리핑 평면까지의 거리를 얻습니다.
	 * 
	 * @return 가까운 클리핑 평면까지의 거리를 반환합니다.
	 */
	const float& GetNearZ() const { return nearZ_; }


	/**
	 * @brief 원거리 클리핑 평면까지의 거리를 얻습니다.
	 * 
	 * @return 원거리 클리핑 평면까지의 거리를 반환합니다.
	 */
	const float& GetFarZ() const { return farZ_; }


	/**
	 * @brief 카메라의 뷰 행렬을 얻습니다.
	 * 
	 * @return 카메라의 뷰 행렬을 반환합니다.
	 */
	const Mat4x4& GetView() const { return view_; }


	/**
	 * @brief 카메라의 투영 행렬을 얻습니다.
	 *
	 * @return 카메라의 투영 행렬을 반환합니다.
	 */
	const Mat4x4& GetProjection() const { return projection_; }


protected:
	/**
	 * @brief 카메라의 월드 상 위치입니다.
	 */
	Vec3f position_;


	/**
	 * @brief 카메라의 방향입니다.
	 */
	Vec3f direction_;


	/**
	 * @brief 카메라의 위 방향입니다.
	 */
	Vec3f upDirection_;


	/**
	 * @brief 카메라의 오른쪽 방향입니다.
	 */
	Vec3f rightDirection_;


	/**
	 * @brief 월드 상의 카메라 위 방향입니다.
	 */
	Vec3f worldUpDirection_ = Vec3f(0.0f, 1.0f, 0.0f);


	/**
	 * @brief 하향식 보기 필드 각도(라디안)입니다.
	 */
	float fov_ = 0.0f;


	/**
	 * @brief 뷰 공간 X:Y의 가로 세로 비율입니다.
	 */
	float aspectRatio_ = 0.0f;


	/**
	 * @brief 가까운 클리핑 평면까지의 거리입니다. 0보다 커야 합니다.
	 */
	float nearZ_ = 0.0f;


	/**
	 * @brief 원거리 클리핑 평면까지의 거리입니다. 0보다 커야 합니다.
	 */
	float farZ_ = 0.0f;


	/**
	 * @brief 뷰 행렬입니다.
	 */
	Mat4x4 view_;


	/**
	 * @brief 투영 행렬입니다.
	 */
	Mat4x4 projection_;
};

}