#pragma once

#include <array>
#include <memory>

#include "IManager.h"
#include "IResource.h"


namespace GameMaker
{
class ResourceManager : public IManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(ResourceManager);

	static ResourceManager& Get();

	virtual void Startup() override;
	virtual void Shutdown() override;

	template <typename TResource, typename... Args>
	TResource* Create(Args&&... args)
	{
		if (!(0 <= cacheSize_ && cacheSize_ < MAX_RESOURCE_SIZE))
		{
			return nullptr;
		}

		int32_t resourceID = -1;
		for (uint32_t index = 0; index < cacheSize_; ++index)
		{
			if (!cache_[index] && !usage_[index])
			{
				resourceID = static_cast<int32_t>(index);
				break;
			}
		}

		if (resourceID == -1)
		{
			resourceID = cacheSize_++;
		}

		usage_[resourceID] = true;
		cache_[resourceID] = std::make_unique<TResource>(args...);

		return reinterpret_cast<TResource*>(cache_[resourceID].get());
	}

	void Destroy(const IResource* resource);


private:
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(ResourceManager);

private:
	static const uint32_t MAX_RESOURCE_SIZE = 200;

	uint32_t cacheSize_ = 0;
	std::array<std::unique_ptr<IResource>, MAX_RESOURCE_SIZE> cache_;
	std::array<bool, MAX_RESOURCE_SIZE> usage_;
};

}