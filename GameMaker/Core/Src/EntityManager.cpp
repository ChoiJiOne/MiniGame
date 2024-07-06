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

void EntityManager::Destroy(const IEntity* entity)
{
	int32_t entityID = -1;
	for (uint32_t index = 0; index < cacheSize_; ++index)
	{
		IEntity* cache = cache_[index].get();
		if (entity == cache)
		{
			entityID = static_cast<int32_t>(index);
			break;
		}
	}

	if (entityID != -1 && cache_[entityID])
	{
		if (cache_[entityID]->IsInitialized())
		{
			cache_[entityID]->Release();
		}

		cache_[entityID].reset();
		usage_[entityID] = false;
	}
}