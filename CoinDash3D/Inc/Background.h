#pragma once

#include "IEntity.h"

class Skybox;


/**
 * @brief 인게임 백그라운드 엔티티입니다.
 */
class Background : public IEntity
{
public:
	/**
	 * @brief 인게임 백그라운드 엔티티의 생성자입니다.
	 */
	Background();


	/**
	 * @brief 인게임 백그라운드 엔티티의 가상 소멸자입니다.
	 */
	virtual ~Background();


	/**
	 * @brief 인게임 백그라운드 엔티티의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Background);


	/**
	 * @brief 인게임 백그라운드 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 인게임 백그라운드 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 백그라운드의 스카이 박스 리소스를 얻습니다.
	 * 
	 * @return 백그라운드의 스카이 박스 리소스를 반환합니다.
	 */
	const Skybox* GetSkybox() const { return skybox_; }


private:
	/**
	 * @brief 백그라운드의 스카이 박스 리소스입니다.
	 */
	Skybox* skybox_ = nullptr;
};