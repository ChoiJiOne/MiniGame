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

void InputManager::Startup()
{
}
