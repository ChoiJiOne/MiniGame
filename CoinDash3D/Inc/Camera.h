#pragma once

#include "Camera3D.h"


/**
 * @brief 월드 상의 카메라 엔티티입니다.
 */
class Camera : public GameMaker::Camera3D
{
public:
	/**
	 * @brief 카메라의 움직임 방향입니다.
	 */
	enum class EMovement
	{
		FORWARD  = 0x00,
		BACKWARD = 0x01,
		LEFT     = 0x02,
		RIGHT    = 0x03,
	};


public:
	/**
	 * @brief 카메라 엔티티의 생성자입니다.
	 */
	Camera();


	/**
	 * @brief 카메라 엔티티의 가상 소멸자입니다.
	 */
	virtual ~Camera();


	/**
	 * @brief 카메라 엔티티의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Camera);


	/**
	 * @brief 카메라 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds);


	/**
	 * @brief 카메라 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release();


	/**
	 * @brief 스크린에 대응하는 직교 투영 행렬을 얻습니다.
	 *
	 * @return 스크린에 대응하는 직교 투영 행렬을 반환합니다.
	 */
	const GameMaker::Mat4x4& GetScreenOrtho() const { return screenOrtho_; }


	/**
	 * @brief 카메라 엔티티의 활성화 여부를 설정합니다.
	 *
	 * @param active 카메라 엔티티의 활성화 여부입니다.
	 */
	void SetActive(bool active) { bIsActive_ = active; }


private:
	/**
	 * @brief 카메라의 상태를 업데이트합니다.
	 */
	void UpdateState();


private:
	/**
	 * @brief 스크린에 대응하는 직교 투영 행렬입니다.
	 */
	GameMaker::Mat4x4 screenOrtho_;


	/**
	 * @brief 카메라의 움직임 속도입니다.
	 */
	float speed_ = 2.5f;


	/**
	 * @brief Y축 오일러 회전 각도입니다.
	 */
	float yaw_ = 0.0f;


	/**
	 * @brief X축 오일러 회전 각도입니다.
	 */
	float pitch_ = 0.0f;


	/**
	 * @brief 카메라가 활성화되었는지 확인합니다.
	 */
	bool bIsActive_ = true;
};