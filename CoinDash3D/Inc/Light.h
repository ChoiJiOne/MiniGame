#pragma once

#include "Mat.h"
#include "Vec.h"

#include "IEntity.h"


/**
 * @brief 라이트 엔티티입니다.
 *
 * @note 이 라이트는 위치를 가지지만, 직선광입니다.
 */
class Light : public IEntity
{
public:
	/**
	 * @brief 라이트 엔티티의 생성자입니다.
	 */
	Light();


	/**
	 * @brief 라이트 엔티티의 가상 소멸자입니다.
	 */
	virtual ~Light();


	/**
	 * @brief 라이트 엔티티의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Light);


	/**
	 * @brief 라이트 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds);


	/**
	 * @brief 라이트 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release();


	/**
	 * @brief 라이트의 위치를 얻습니다.
	 *
	 * @return 라이트의 위치를 반환합니다.
	 */
	const Vec3f& GetPosition() const { return position_; }


	/**
	 * @brief 라이트의 방향을 얻습니다.
	 *
	 * @return 라이트의 방향을 얻습니다.
	 */
	const Vec3f& GetDirection() const { return direction_; }


	/**
	 * @brief 라이트의 색상을 얻습니다.
	 *
	 * @return 라이트의 색상을 반환합니다.
	 */
	const Vec3f& GetColor() const { return color_; }


	/**
	 * @brief 라이트의 뷰 행렬을 얻습니다.
	 *
	 * @return 라이트의 뷰 행렬을 반환합니다.
	 */
	const Mat4x4& GetView() const { return view_; }


	/**
	 * @brief 라이트의 투영 행렬을 얻습니다.
	 *
	 * @return 라이트의 투영 행렬을 반환합니다.
	 */
	const Mat4x4& GetProjection() const { return projection_; }


private:
	/**
	 * @brief 라이트의 위치입니다.
	 */
	Vec3f position_ = Vec3f(0.0f, 0.0f, 0.0f);


	/**
	 * @brief 라이트의 방향입니다.
	 */
	Vec3f direction_ = Vec3f(0.0f, 0.0f, 0.0f);


	/**
	 * @brief 라이트의 월드 업 벡터입니다.
	 */
	Vec3f worldUp_ = Vec3f(0.0f, 1.0f, 0.0f);


	/**
	 * @brief 라이트의 색상입니다.
	 */
	Vec3f color_ = Vec3f(1.0f, 1.0f, 1.0f);


	/**
	 * @brief 라이트의 뷰 행렬입니다.
	 */
	Mat4x4 view_;


	/**
	 * @brief 라이트의 투영 행렬입니다.
	 */
	Mat4x4 projection_;
};