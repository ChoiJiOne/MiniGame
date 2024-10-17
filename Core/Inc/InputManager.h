#pragma once

#include "GameMath.h"
#include "Macro.h"

/** 입력 처리를 수행하는 매니저는 싱글턴입니다. */
class InputManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(InputManager);

	static InputManager& GetRef();
	static InputManager* GetPtr();

private:
	friend class IApp;

	InputManager() = default;
	virtual ~InputManager() {}

	void Startup(); /** IApp 내부에서만 사용하는 메서드입니다. */

private:
	static InputManager instance_;
};