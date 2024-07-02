#include "Assertion.h"
#include "EntityManager.h"

using namespace GameMaker;

EntityManager& EntityManager::Get()
{
	static EntityManager instance;
	return instance;
}

void EntityManager::Startup()
{
	CHECK(!bIsStartup_);

	cacheSize_ = 0;
	bIsStartup_ = true;
}

void EntityManager::Shutdown()
{
	CHECK(bIsStartup_);

	for (std::size_t index = 0; index < cacheSize_; ++index)
	{
		if (cache_[index])
		{
			if (cache_[index]->IsInitialized())
			{
				cache_[index]->Release();
			}

			cache_[index].reset();
			usage_[index] = false;
		}
	}

	bIsStartup_ = false;
}