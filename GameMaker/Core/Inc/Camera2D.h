#pragma once

#include "GameMath.h"
#include "IEntity.h"


namespace GameMaker
{
/**
 * @brief 2D 카메라 엔티티의 인터페이스입니다.
 */
class Camera2D : public IEntity
{
public:
	/**
	 * @brief 2D 카메라의 기본 생성자입니다.
	 */
	Camera2D() = default;


	/**
	 * @brief 카메라 엔티티의 가상 소멸자입니다.
	 */
	virtual ~Camera2D() {}


	/**
	 * @brief 2D 카메라의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Camera2D);


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
	 * @brief 2D 카메라의 월드 상 중심 좌표를 얻습니다.
	 * 
	 * @return 2D 카메라의 월드 상 중심 좌표를 반환합니다.
	 */
	const Vec2f& GetCenter() const { return center_; }


	/**
	 * @brief 2D 카메라의 가로 영역 크기를 얻습니다.
	 * 
	 * @return 2D 카메라의 가로 영역 크기를 반환합니다.
	 */
	const float& GetWidth() const { return width_; }


	/**
	 * @brief 2D 카메라의 세로 영역 크기를 얻습니다.
	 * 
	 * @return 2D 카메라의 세로 영역 크기를 반환합니다.
	 */
	const float& GetHeight() const { return height_; }


	/**
	 * @brief 2D 카메라의 직교 투영 행렬을 얻습니다.
	 * 
	 * @return 2D 카메라의 직교 투영 행렬을 반환합니다.
	 */
	const Mat4x4& GetOrtho() const { return ortho_; }


protected:
	/**
	 * @brief 2D 카메라의 월드 상 중심 좌표입니다.
	 */
	Vec2f center_;


	/**
	 * @brief 2D 카메라의 가로 영역 크기입니다.
	 */
	float width_ = 0.0f;


	/**
	 * @brief 2D 카메라의 세로 영역 크기입니다.
	 */
	float height_ = 0.0f;


	/**
	 * @brief 2D 카메라의 직교 투영 행렬입니다.
	 */
	Mat4x4 ortho_;
};

}