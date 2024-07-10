#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <SDL2/SDL.h>

#include "Assertion.h"
#include "GameEngine.h"
#include "InputManager.h"

using namespace GameMaker;

InputManager& InputManager::Get()
{
	static InputManager instance;
	return instance;
}

void InputManager::Startup()
{
	CHECK(!bIsStartup_);

	windowEventActionSize_ = 0;
	for (uint32_t index = 0; index < MAX_EVENT_ACTION_SIZE; ++index)
	{
		windowEventActions_[index].windowEvent = EWindowEvent::NONE;
		windowEventActions_[index].windowEventAction = nullptr;
		windowEventActions_[index].bIsActive = false;
	}

	bIsStartup_ = true;
}

void InputManager::Shutdown()
{
	CHECK(bIsStartup_);
	
	// 명시적으로 비활성화.
	for (uint32_t index = 0; index < windowEventActionSize_; ++index)
	{
		windowEventActions_[index].windowEvent = EWindowEvent::NONE;
		windowEventActions_[index].windowEventAction = nullptr;
		windowEventActions_[index].bIsActive = false;
	}

	bIsStartup_ = false;
}


EPressState InputManager::GetKeyPressState(const EKey& key)
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

EPressState InputManager::GetMousePressState(const EMouse& mouse)
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

WindowEventID InputManager::AddWindowEventAction(const EWindowEvent& windowEvent, const std::function<void()>& eventAction, bool bIsActive)
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

void InputManager::DeleteWindowEventAction(const WindowEventID& windowEventID)
{
	CHECK(0 <= windowEventID && windowEventID < MAX_EVENT_ACTION_SIZE);

	windowEventActions_[windowEventID].windowEvent = EWindowEvent::NONE;
	windowEventActions_[windowEventID].windowEventAction = nullptr;
	windowEventActions_[windowEventID].bIsActive = false;
}

void InputManager::SetActiveWindowEventAction(const WindowEventID& windowEventID, bool bIsActive)
{
	CHECK(0 <= windowEventID && windowEventID < MAX_EVENT_ACTION_SIZE);
	windowEventActions_[windowEventID].bIsActive = bIsActive;
}

void InputManager::ProcessInputEvents()
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		if (e.type == SDL_QUIT)
		{
			GameEngine::SetShouldCloseWindow(true);
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

void InputManager::ExecuteWindowEventAction(const EWindowEvent& windowEvent)
{
	for (int32_t index = 0; index < windowEventActionSize_; ++index)
	{
		if (windowEventActions_[index].windowEvent == windowEvent && windowEventActions_[index].bIsActive)
		{
			windowEventActions_[index].windowEventAction();
		}
	}
}

bool InputManager::IsPressKey(const KeyboardState& keyboardState, const EKey& key)
{
	return keyboardState.keybordState.at(static_cast<int32_t>(key)) == 0 ? false : true;
}

bool InputManager::IsPressMouse(const MouseState& mouseState, const EMouse& mouse)
{
	return (mouseState.state & static_cast<uint32_t>(mouse)) == 0 ? false : true;
}