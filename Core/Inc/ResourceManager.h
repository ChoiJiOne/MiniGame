#pragma once

#include <array>
#include <memory>
#include <map>

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
		if (!(0 <= size_ && size_ < MAX_RESOURCE_SIZE))
		{
			return nullptr;
		}

		int32_t resourceID = -1;
		for (uint32_t index = 0; index < size_; ++index)
		{
			if (!resources_[index] && !usage_[index])
			{
				resourceID = static_cast<int32_t>(index);
				break;
			}
		}

		if (resourceID == -1)
		{
			resourceID = size_++;
		}

		usage_[resourceID] = true;
		resources_[resourceID] = std::make_unique<TResource>(args...);

		return reinterpret_cast<TResource*>(resources_[resourceID].get());
	}

	void Destroy(const IResource* resource);

	void Register(const std::string& name, IResource* resource);
	bool IsRegistration(const std::string& name);
	void Unregister(const std::string& name);

	template <typename TResource>
	TResource* GetByName(const std::string& name)
	{
		auto it = resourceCache_.find(name);
		if (it == resourceCache_.end())
		{
			return nullptr;
		}

		return reinterpret_cast<TResource*>(it->second);
	}

private:
	friend class IApp;

	ResourceManager() = default;
	virtual ~ResourceManager() {}
	
	void Unregister(const IResource* resource); /** 이 메서드는 내부에서만 사용해야 합니다. */

	void Cleanup(); /** 이 메서드는 IApp에서만 접근해야 합니다. */

private:
	static const uint32_t MAX_RESOURCE_SIZE = 200;

	uint32_t size_ = 0;
	std::array<std::unique_ptr<IResource>, MAX_RESOURCE_SIZE> resources_;
	std::array<bool, MAX_RESOURCE_SIZE> usage_;

	std::map<std::string, IResource*> resourceCache_;
};