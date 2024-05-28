#include "Assertion.h"

#include "GameModule.h"

bool GameModule::bIsInit = false;
uint32_t GameModule::cacheSize = 0;
std::array<std::unique_ptr<IEntity>, GameModule::MAX_ENTITY_COUNT> GameModule::cache;
std::array<bool, GameModule::MAX_ENTITY_COUNT> GameModule::usage;

GameModule::Errors GameModule::Init()
{
	if (bIsInit)
	{
		return Errors::ERR_PREV_INIT;
	}

	bIsInit = true;
	return Errors::OK;
}

GameModule::Errors GameModule::Uninit()
{
	if (!bIsInit)
	{
		return Errors::ERR_NOT_INIT;
	}

	for (std::size_t index = 0; index < cacheSize; ++index)
	{
		if (cache[index])
		{
			if (cache[index]->IsInitialized())
			{
				cache[index]->Release();
			}

			cache[index].reset();
		}
	}

	bIsInit = false;
	return Errors::OK;
}