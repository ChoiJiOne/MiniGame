#pragma once

#include "Macro.h"


namespace GameMaker
{
/**
 * 이 클래스는 매니저 인터페이스 클래스로, 접미사로 매니저(Manager)라고 명명될 클래스는 반드시 이 인터페이스를 상속받아야 합니다.
 * 또한, 이 클래스를 상속받은 매니저 클래스는 싱글턴(Singleton)으로 구현되어야 합니다.
 */
class IManager
{
	DISALLOW_COPY_AND_ASSIGN(IManager);

	virtual void Startup() = 0;
	virtual void Shutdown() = 0;
	
protected:
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(IManager);

protected:
	bool bIsStartup_ = false;
};

}