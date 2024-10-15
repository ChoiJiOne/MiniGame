#pragma once

#include "Macro.h"

/** UI 매니저는 싱글턴입니다. */
class UIManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(UIManager);

	static UIManager& Get();

private:
	friend class IApp;

	UIManager() = default;
	virtual ~UIManager() {}
};