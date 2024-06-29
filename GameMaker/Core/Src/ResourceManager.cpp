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