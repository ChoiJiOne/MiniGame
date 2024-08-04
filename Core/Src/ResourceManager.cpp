#include "Assertion.h"
#include "ResourceManager.h"

using namespace GameMaker;

ResourceManager& ResourceManager::Get()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::Startup()
{
	CHECK(!bIsStartup_);

	cacheSize_ = 0;
	bIsStartup_ = true;
}

void ResourceManager::Shutdown()
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

void ResourceManager::Destroy(const IResource* resource)
{
	int32_t resourceID = -1;
	for (uint32_t index = 0; index < cacheSize_; ++index)
	{
		IResource* cacheResource = cache_[index].get();
		if (resource == cacheResource)
		{
			resourceID = static_cast<int32_t>(index);
			break;
		}
	}

	if (resourceID != -1 && cache_[resourceID])
	{
		if (cache_[resourceID]->IsInitialized())
		{
			cache_[resourceID]->Release();
		}

		cache_[resourceID].reset();
		usage_[resourceID] = false;
	}
}