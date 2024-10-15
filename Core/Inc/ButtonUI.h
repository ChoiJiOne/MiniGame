#pragma once

#include <functional>
#include <map>

#include "Collision2D.h"
#include "IApp.h"

#include "IEntityUI.h"

class TTFont;

class ButtonUI : public IEntityUI
{
public:
	struct Layout
	{
		Vec4f textColor;
		Vec4f disableColor;
		Vec4f enableColor;
		Vec4f pressColor;
		Vec4f releaseColor;

		Vec2f center;
		Vec2f size;
		Mouse mouse;

		TTFont* font;
		std::wstring text;
		float side;
	};

public:
	ButtonUI(const Layout& layout, const std::function<void()>& clickEvent);
	virtual ~ButtonUI();

	DISALLOW_COPY_AND_ASSIGN(ButtonUI);

	virtual void Tick(float deltaSeconds) override;
	virtual void Release() override;

private:
	friend class UIManager;

	enum class State : int32_t
	{
		DISABLED = 0x00,
		ENABLED  = 0x01,
		PRESSED  = 0x02,
		RELEASED = 0x03,
	};

	State GetState(const Press& press, const State& state);
	bool IsDetectMouseCursor();

private:
	IApp* app_ = nullptr;

	Layout layout_;
	Rect2D bound_;
	State state_ = State::DISABLED;

	Vec2f textPos_;

	std::function<void()> clickEvent_ = nullptr;

	std::map<State, Vec4f> stateColors_;
};