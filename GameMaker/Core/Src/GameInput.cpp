#include <SDL2/SDL.h>

#include "Assertion.h"
#include "GameEngine.h"
#include "GameInput.h"

using namespace GameMaker;

GameInput::KeyboardState GameInput::prevKeyboardState_;
GameInput::KeyboardState GameInput::currKeyboardState_;
GameInput::MouseState GameInput::prevMouseState_;
GameInput::MouseState GameInput::currMouseState_;
uint32_t GameInput::windowEventActionSize_ = 0;
std::array<GameInput::WindowEventAction, GameInput::MAX_EVENT_ACTION_SIZE> GameInput::windowEventActions_;

EPressState GameInput::GetKeyPressState(const EKey& key)
{
	EPressState pressState = EPressState::NONE;

	if (IsPressKey(prevKeyboardState_, key))
	{
		if (IsPressKey(currKeyboardState_, key))
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
		if (IsPressKey(currKeyboardState_, key))
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

EPressState GameInput::GetMousePressState(const EMouse& mouse)
{
	EPressState pressState = EPressState::NONE;

	if (IsPressMouse(prevMouseState_, mouse))
	{
		if (IsPressMouse(currMouseState_, mouse))
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
		if (IsPressMouse(currMouseState_, mouse))
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

WindowEventID GameInput::AddWindowEventAction(const EWindowEvent& windowEvent, const std::function<void()>& eventAction, bool bIsActive)
{
	CHECK(0 <= windowEventActionSize_ && windowEventActionSize_ < MAX_EVENT_ACTION_SIZE);

	WindowEventID windowEventID = -1;
	for (int32_t index = 0; index < windowEventActionSize_; ++index)
	{
		if (windowEventActions_[index].windowEvent == EWindowEvent::NONE)
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

void GameInput::DeleteWindowEventAction(const WindowEventID& windowEventID)
{
	CHECK(0 <= windowEventID && windowEventID < MAX_EVENT_ACTION_SIZE);

	windowEventActions_[windowEventID].windowEvent = EWindowEvent::NONE;
	windowEventActions_[windowEventID].windowEventAction = nullptr;
	windowEventActions_[windowEventID].bIsActive = false;
}

void GameInput::SetActiveWindowEventAction(const WindowEventID& windowEventID, bool bIsActive)
{
	CHECK(0 <= windowEventID && windowEventID < MAX_EVENT_ACTION_SIZE);
	windowEventActions_[windowEventID].bIsActive = bIsActive;
}

void GameInput::Tick()
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			GameEngine::SetQuitLoop(true);
		}

		EWindowEvent windowEvent = static_cast<EWindowEvent>(e.window.event);

		for (std::size_t index = 0; index < windowEventActionSize_; ++index)
		{
			if (windowEvent == windowEventActions_[index].windowEvent && windowEventActions_[index].bIsActive)
			{
				windowEventActions_[index].windowEventAction();
			}
		}
	}

	const void* currKeyboardState = reinterpret_cast<const void*>(SDL_GetKeyboardState(nullptr));

	std::memcpy(prevKeyboardState_.keybordState.data(), currKeyboardState_.keybordState.data(), KeyboardState::BUFFER_SIZE);
	std::memcpy(currKeyboardState_.keybordState.data(), currKeyboardState, KeyboardState::BUFFER_SIZE);

	prevMouseState_ = currMouseState_;
	currMouseState_.state = SDL_GetMouseState(&currMouseState_.position.x, &currMouseState_.position.y);
}

void GameInput::ExecuteWindowEventAction(const EWindowEvent& windowEvent)
{
	for (int32_t index = 0; index < windowEventActionSize_; ++index)
	{
		if (windowEventActions_[index].windowEvent == windowEvent && windowEventActions_[index].bIsActive)
		{
			windowEventActions_[index].windowEventAction();
		}
	}
}

bool GameInput::IsPressKey(const KeyboardState& keyboardState, const EKey& key)
{
	return keyboardState.keybordState.at(static_cast<int32_t>(key)) == 0 ? false : true;
}

bool GameInput::IsPressMouse(const MouseState& mouseState, const EMouse& mouse)
{
	return (mouseState.state & static_cast<uint32_t>(mouse)) == 0 ? false : true;
}
