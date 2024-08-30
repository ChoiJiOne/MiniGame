#pragma once

#include <array>
#include <memory>

#include "IResource.h"

/** 리소스 매니저는 싱글턴 클래스입니다. */
class ResourceManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(ResourceManager);

	static ResourceManager& Get();

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
			if (!resources_[index] && !usage_[index])
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
		resources_[resourceID] = std::make_unique<TResource>(args...);

		return reinterpret_cast<TResource*>(resources_[resourceID].get());
	}

	void Destroy(const IResource* resource);

private:
	friend class IApp;

	ResourceManager() = default;
	virtual ~ResourceManager() {}
	
	void Cleanup(); /** 이 메서드는 IApp에서만 접근해야 합니다. */

private:
	static const uint32_t MAX_RESOURCE_SIZE = 200;

	uint32_t cacheSize_ = 0;
	std::array<std::unique_ptr<IResource>, MAX_RESOURCE_SIZE> resources_;
	std::array<bool, MAX_RESOURCE_SIZE> usage_;
};