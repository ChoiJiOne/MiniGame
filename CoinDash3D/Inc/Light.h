#pragma once

#include "Mat.h"
#include "Vec.h"

#include "IEntity.h"

class Camera;


/**
 * @brief 월드 상의 라이팅 엔티티입니다.
 */
class Light : public IEntity
{
public:
	/**
	 * @brief 라이팅 엔티티의 생성자입니다.
	 * 
	 * @param camera 라이팅 위치의 기준이 될 카메라 엔티티입니다.
	 */
	Light(Camera* camera);


	/**
	 * @brief 라이팅 엔티티의 가상 소멸자입니다.
	 */
	virtual ~Light();


	/**
	 * @brief 라이팅 엔티티의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Light);


	/**
	 * @brief 라이팅 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds);


	/**
	 * @brief 라이팅 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release();


	/**
	 * @brief 월드 상의 라이트 위치 값을 얻습니다.
	 * 
	 * @return 월드 상의 라이트 위치 값을 반환합니다.
	 */
	const Vec3f& GetPosition() const { return position_; }


	/**
	 * @brief 라이트 방향 값을 얻습니다.
	 * 
	 * @return 라이트 방향 값을 반환합니다.
	 */
	const Vec3f& GetDirection() const { return direction_; }


	/**
	 * @brief 라이트 공간의 행렬을 얻습니다.
	 * 
	 * @return 라이트 공간의 행렬을 반환합니다.
	 */
	const Mat4x4& GetLightSpaceMatrix() const { return lightSpaceMatrix_; }


	/**
	 * @brief 라이트의 RGB색상 값을 얻습니다.
	 * 
	 * @return 라이트의 RGB 색상 값을 반환합니다.
	 */
	const Vec3f& GetColor() const { return color_; }


private:
	/**
	 * @brief 라이트의 위치입니다.
	 */
	Vec3f position_;


	/**
	 * @brief 라이트의 방향입니다.
	 */
	Vec3f direction_;


	/**
	 * @brief 라이트의 시점의 시야 행렬입니다.
	 */
	Mat4x4 view_;


	/**
	 * @brief 라이트 시점의 직교 투영 행렬입니다.
	 */
	Mat4x4 projection_;


	/**
	 * @brief 라이트 공간의 행렬입니다.
	 */
	Mat4x4 lightSpaceMatrix_;


	/**
	 * @brief 라이트의 색상입니다.
	 */
	Vec3f color_;


	/**
	 * @brief 라이팅 위치의 기준이 될 카메라 엔티티입니다.
	 */
	Camera* camera_ = nullptr;
};