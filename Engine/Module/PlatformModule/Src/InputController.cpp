#pragma warning(push)
#pragma warning(disable: 6031)

#include "Assertion.h"

#include "InputController.h"
#include "PlatformModule.h"

std::array<uint8_t, InputController::KEY_BUFFER_SIZE> InputController::prevKeyboardState;
std::array<uint8_t, InputController::KEY_BUFFER_SIZE> InputController::currKeyboardState;
CursorPos InputController::prevCursorPosition;
CursorPos InputController::currCursorPosition;
uint32_t InputController::windowEventActionSize;
std::array<InputController::WindowEventAction, InputController::MAX_EVENT_ACTION_SIZE> InputController::windowEventActions;

EPressState InputController::GetKeyPressState(const EKey& keyCode)
{
	EPressState pressState = EPressState::NONE;

	if (IsPressKey(prevKeyboardState.data(), keyCode))
	{
		if (IsPressKey(currKeyboardState.data(), keyCode))
		{
			pressState = EPressState::HELD;
		}
		else
		{
			pressState = EPressState::RELEASED;
		}
	}
	else
	{
		if (IsPressKey(currKeyboardState.data(), keyCode))
		{
			pressState = EPressState::PRESSED;
		}
		else
		{
			pressState = EPressState::NONE;
		}
	}

	return pressState;
}

WindowEventID InputController::AddWindowEventAction(const EWindowEvent& windowEvent, const std::function<void()>& eventAction, bool bIsActive)
{
	CHECK(0 <= windowEventActionSize && windowEventActionSize < MAX_EVENT_ACTION_SIZE);

	WindowEventID windowEventID = -1;
	for (int32_t index = 0; index < windowEventActionSize; ++index)
	{
		if (windowEventActions[index].windowEvent == EWindowEvent::NONE)
		{
			windowEventID = static_cast<WindowEventID>(index);
			break;
		}
	}

	if (windowEventID == -1)
	{
		windowEventID = windowEventActionSize++;
	}

	windowEventActions[windowEventID].windowEvent = windowEvent;
	windowEventActions[windowEventID].windowEventAction = eventAction;
	windowEventActions[windowEventID].bIsActive = bIsActive;

	return windowEventID;
}

void InputController::DeleteWindowEventAction(const WindowEventID& windowEventID)
{
	CHECK(0 <= windowEventID && windowEventID < MAX_EVENT_ACTION_SIZE);

	windowEventActions[windowEventID].windowEvent = EWindowEvent::NONE;
	windowEventActions[windowEventID].windowEventAction = nullptr;
	windowEventActions[windowEventID].bIsActive = false;
}

void InputController::SetActiveWindowEventAction(const WindowEventID& windowEventID, bool bIsActive)
{
	CHECK(0 <= windowEventID && windowEventID < MAX_EVENT_ACTION_SIZE);
	windowEventActions[windowEventID].bIsActive = bIsActive;
}

void InputController::Tick()
{
	std::copy(currKeyboardState.begin(), currKeyboardState.end(), prevKeyboardState.begin());
	CHECK(GetKeyboardState(currKeyboardState.data()));

	prevCursorPosition = currCursorPosition;
	currCursorPosition = GetCurrentCursorPosition();
}

void InputController::ExecuteWindowEventAction(const EWindowEvent& windowEvent)
{
	for (int32_t index = 0; index < windowEventActionSize; ++index)
	{
		if (windowEventActions[index].windowEvent == windowEvent && windowEventActions[index].bIsActive)
		{
			windowEventActions[index].windowEventAction();
		}
	}
}

bool InputController::IsPressKey(const uint8_t* keyBufferPtr, const EKey& keyCode)
{
	return (keyBufferPtr[static_cast<int32_t>(keyCode)] & 0x80);
}

CursorPos InputController::GetCurrentCursorPosition()
{
	POINT cursorPos;

	if (GetCursorPos(&cursorPos))
	{
		HWND hwnd = PlatformModule::GetWindowHandle();
		if (ScreenToClient(hwnd, &cursorPos))
		{
			return CursorPos{ static_cast<int32_t>(cursorPos.x), static_cast<int32_t>(cursorPos.y) };
		}
	}

	return CursorPos{ 0, 0 };
}

#pragma warning(pop)