#pragma once

#include "Macro.h"

class Camera2D;

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

	void Startup(); /** IApp 내부에서만 호출해야 합니다. */
	void Shutdown(); /** IApp 내부에서만 호출해야 합니다. */

private:
	Camera2D* uiCamera_ = nullptr;
};