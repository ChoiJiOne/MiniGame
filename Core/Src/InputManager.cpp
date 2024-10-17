#include <SDL2/SDL.h>

#include "Assertion.h"
#include "InputManager.h"

InputManager InputManager::instance_;

InputManager& InputManager::GetRef()
{
	return instance_;
}

InputManager* InputManager::GetPtr()
{
	return &instance_;
}

Press InputManager::GetKeyPress(const Key& key)
{
	Press press = Press::NONE;

	if (IsPressKey(prevKeyboardState_, key))
	{
		if (IsPressKey(currKeyboardState_, key))
		{
			press = Press::HELD;
		}
		else
		{
			press = Press::RELEASED;
		}
	}
	else
	{
		if (IsPressKey(currKeyboardState_, key))
		{
			press = Press::PRESSED;
		}
		else
		{
			press = Press::NONE;
		}
	}

	return press;
}

Press InputManager::GetMousePress(const Mouse& mouse)
{
	Press press = Press::NONE;

	if (IsPressMouse(prevMouseState_, mouse))
	{
		if (IsPressMouse(currMouseState_, mouse))
		{
			press = Press::HELD;
		}
		else
		{
			press = Press::RELEASED;
		}
	}
	else
	{
		if (IsPressMouse(currMouseState_, mouse))
		{
			press = Press::PRESSED;
		}
		else
		{
			press = Press::NONE;
		}
	}

	return press;
}

WindowEventID InputManager::AddWindowEventAction(const WindowEvent& windowEvent, const std::function<void()>& eventAction, bool bIsActive)
{
	CHECK(0 <= windowEventActionSize_ && windowEventActionSize_ < MAX_EVENT_ACTION_SIZE);

	WindowEventID windowEventID = -1;
	for (int32_t index = 0; index < windowEventActionSize_; ++index)
	{
		if (windowEventActions_[index].windowEvent == WindowEvent::NONE)
		{
			windowEventID = static_cast<WindowEventID>(index);
			break;
		}
	}

	if (windowEventID == -1)
	{
		windowEventID = windowEventActionSize_++;
	}

	windowEventActions_[windowEventID].windowEvent = windowEvent;
	windowEventActions_[windowEventID].windowEventAction = eventAction;
	windowEventActions_[windowEventID].bIsActive = bIsActive;

	return windowEventID;
}

void InputManager::DeleteWindowEventAction(const WindowEventID& windowEventID)
{
	CHECK(0 <= windowEventID && windowEventID < MAX_EVENT_ACTION_SIZE);

	windowEventActions_[windowEventID].windowEvent = WindowEvent::NONE;
	windowEventActions_[windowEventID].windowEventAction = nullptr;
	windowEventActions_[windowEventID].bIsActive = false;
}

void InputManager::SetActiveWindowEventAction(const WindowEventID& windowEventID, bool bIsActive)
{
	CHECK(0 <= windowEventID && windowEventID < MAX_EVENT_ACTION_SIZE);
	windowEventActions_[windowEventID].bIsActive = bIsActive;
}

void InputManager::Startup()
{
	AddWindowEventAction(WindowEvent::RESIZED, [&]() { bIsResize_ = !bIsResize_; }, true);
	AddWindowEventAction(WindowEvent::MINIMIZED, [&]() { bIsMinimize_ = true; }, true);
	AddWindowEventAction(WindowEvent::MAXIMIZED, [&]() { bIsMaximize_ = true; }, true);
	AddWindowEventAction(WindowEvent::ENTER, [&]() { bIsEnterMouse_ = true; });
	AddWindowEventAction(WindowEvent::LEAVE, [&]() { bIsEnterMouse_ = false; });
	AddWindowEventAction(WindowEvent::ENTER, [&]() { bIsEnterMouse_ = true; });
	AddWindowEventAction(WindowEvent::LEAVE, [&]() { bIsEnterMouse_ = false; });
	AddWindowEventAction(WindowEvent::FOCUS_GAINED, [&]() { bIsGainFocus_ = true; });
	AddWindowEventAction(WindowEvent::FOCUS_LOST, [&]() { bIsGainFocus_ = false; });
	AddWindowEventAction(WindowEvent::RESTORED,
		[&]()
		{
			bIsMaximize_ = (bIsMaximize_) ? !bIsMaximize_ : bIsMaximize_;
			bIsMinimize_ = (bIsMinimize_) ? !bIsMinimize_ : bIsMinimize_;
		}
	);
}

bool InputManager::IsPressKey(const KeyboardState& keyboardState, const Key& key)
{
	return keyboardState.keybordState.at(static_cast<int32_t>(key)) == 0 ? false : true;
}

bool InputManager::IsPressMouse(const MouseState& mouseState, const Mouse& mouse)
{
	return (mouseState.state & static_cast<uint32_t>(mouse)) == 0 ? false : true;
}
