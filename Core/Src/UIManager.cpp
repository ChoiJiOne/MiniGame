#include "Assertion.h"
#include "UIManager.h"

UIManager& UIManager::Get()
{
	static UIManager instance;
	return instance;
}
