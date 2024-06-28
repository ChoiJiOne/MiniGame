#pragma once

#include "Macro.h"


namespace GameMaker
{
/**
 * @brief 매니저 인터페이스입니다.
 * 
 * @note 
 * - 접미사로 매니저(Manager)라고 명명될 클래스는 반드시 이 인터페이스를 상속받아야 합니다.
 * - 이 클래스를 상속받은 매니저 클래스는 싱글턴(Singleton)으로 구현되어야 합니다.
 */
class IManager
{
	/**
	 * @brief 매니저 인터페이스의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(IManager);


	/**
	 * @brief 매니저 인터페이스의 사용을 시작합니다.
	 */
	virtual void Startup() = 0;


	/**
	 * @brief 매니저 인터페이스의 사용을 종료합니다.
	 *
	 * @note
	 * - 프로그램 종료 전에 반드시 이 메서드를 호출해야 합니다.
	 * - 하위 클래스에서는 이 메서드를 통해 리소스를 정리해야 합니다.
	 */
	virtual void Shutdown() = 0;


protected:
	/**
	 * @brief 매니저 인터페이스의 디폴트 생성자와 가상 소멸자입니다.
	 */
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(IManager);


protected:
	/**
	 * @brief 매니저 인터페이스의 사용이 시작되었는지 확인합니다.
	 */
	bool bIsStartup_ = false;
};

}