#pragma once

#include "IApp.h"
#include "IEntityUI.h"

class ButtonUI;
class Camera2D;
class TTFont;

/** UI 매니저는 싱글턴입니다. */
class UIManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(UIManager);

	static UIManager& Get();

	ButtonUI* Create(const std::string& path, const Mouse& mouse, TTFont* font, const std::function<void()>& clickEvent);

	void BatchTickUIEntity(IEntityUI** entities, uint32_t count, float deltaSeconds);
	void BatchRenderUIEntity(IEntityUI** entities, uint32_t count);

private:
	friend class IApp;

	UIManager() = default;
	virtual ~UIManager() {}

	void Startup(); /** IApp 내부에서만 호출해야 합니다. */
	void Shutdown(); /** IApp 내부에서만 호출해야 합니다. */

private:
	Camera2D* uiCamera_ = nullptr;
};