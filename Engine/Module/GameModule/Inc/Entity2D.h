#pragma once

#include "IEntity.h"


/**
 * @brief 2D 엔티티의 인터페이스입니다.
 */
class Entity2D : public IEntity
{
public:
	/**
	 * @brief 2D 엔티티 인터페이스의 디폴트 생성자입니다.
	 */
	Entity2D() = default;


	/**
	 * @brief 2D 엔티티 인터페이스의 가상 소멸자입니다.
	 */
	virtual ~Entity2D() {}


	/**
	 * @brief 2D 엔티티 인터페이스의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Entity2D);


	/**
	 * @brief 2D 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 *
	 * @note
	 * - 2D 엔티티 인터페이스를 상속받는 하위 클래스에서 반드시 구현해야 합니다.
	 * - 델타 시간 값은 초단위입니다.
	 */
	virtual void Tick(float deltaSeconds) = 0;


	/**
	 * @brief 2D 엔티티를 렌더링합니다.
	 * 
	 * @note 2D 엔티티 인터페이스를 상속받는 하위 클래스에서 반드시 구현해야 합니다.
	 */
	virtual void Render() = 0;


	/**
	 * @brief 2D 엔티티 내의 리소스를 할당 해제합니다.
	 *
	 * @note 2D 엔티티 인터페이스를 상속받는 하위 클래스에서 반드시 구현해야 합니다.
	 */
	virtual void Release() = 0;
};