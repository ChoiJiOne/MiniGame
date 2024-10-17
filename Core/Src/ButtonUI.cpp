#include "Assertion.h"
#include "ButtonUI.h"
#include "InputManager.h"
#include "RenderStateManager.h"
#include "TTFont.h"

InputManager* ButtonUI::inputMgr_ = nullptr;
RenderStateManager* ButtonUI::renderStateMgr_ = nullptr;

ButtonUI::ButtonUI(const Layout& layout, const std::function<void()>& clickEvent)
	: layout_(layout)
	, clickEvent_(clickEvent)
{
	if (!inputMgr_)
	{
		inputMgr_ = InputManager::GetPtr();
	}

	if (!renderStateMgr_)
	{
		renderStateMgr_ = RenderStateManager::GetPtr();
	}

	bound_ = Rect2D(layout_.center, layout_.size);

	Vec2f textSize;
	layout_.font->MeasureText(layout_.text, textSize.x, textSize.y);

	textPos_ = layout_.center + Vec2f(-textSize.x * 0.5f, +textSize.y * 0.5f);
	stateColors_ =
	{
		{ State::DISABLED, layout_.disableColor },
		{ State::ENABLED,  layout_.enableColor  },
		{ State::PRESSED,  layout_.pressColor   },
		{ State::RELEASED, layout_.releaseColor },
	};

	bIsInitialized_ = true;
}

ButtonUI::~ButtonUI()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void ButtonUI::Tick(float deltaSeconds)
{
	Press press = inputMgr_->GetMousePress(layout_.mouse);
	state_ = GetState(press, state_);

	if (state_ == State::RELEASED && clickEvent_)
	{
		clickEvent_();
	}
}

void ButtonUI::Release()
{
	CHECK(bIsInitialized_);

	bIsInitialized_ = false;
}

ButtonUI::State ButtonUI::GetState(const Press& press, const State& state)
{
	if (!IsDetectMouseCursor())
	{
		if (state == State::PRESSED && press == Press::HELD)
		{
			return State::PRESSED;
		}
		else
		{
			return State::DISABLED;
		}
	}

	if (state == State::DISABLED)
	{
		if (press == Press::HELD || press == Press::PRESSED)
		{
			return State::DISABLED;
		}
		else // press == Press::NONE || press == Press::RELEASED
		{
			return State::ENABLED;
		}
	}
	else if (state == State::ENABLED)
	{
		if (press == Press::HELD || press == Press::PRESSED)
		{
			return State::PRESSED;
		}
		else
		{
			return State::ENABLED;
		}
	}
	else if (state == State::PRESSED)
	{
		if (press == Press::RELEASED)
		{
			return State::RELEASED;
		}
		else if (press == Press::HELD || press == Press::PRESSED)
		{
			return State::PRESSED;
		}
		else // press == Press::NONE
		{
			return State::ENABLED;
		}
	}
	else // state == State::RELEASED
	{
		if (press == Press::NONE || press == Press::RELEASED)
		{
			return State::ENABLED;
		}
		else // press == Press::HELD || press == Press::PRESSED
		{
			return State::PRESSED;
		}
	}

	return State::DISABLED;
}

bool ButtonUI::IsDetectMouseCursor()
{
	Vec2i currPos = inputMgr_->GetCurrMousePos();

	Vec2f screenSize;
	renderStateMgr_->GetScreenSize<float>(screenSize.x, screenSize.y);

	Point2D mouseCursor;
	mouseCursor.center.x = -screenSize.x * 0.5f + static_cast<float>(currPos.x);
	mouseCursor.center.y = +screenSize.y * 0.5f - static_cast<float>(currPos.y);

	return bound_.Intersect(&mouseCursor);
}