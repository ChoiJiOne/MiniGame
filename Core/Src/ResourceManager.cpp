#include "Assertion.h"
#include "ResourceManager.h"

ResourceManager ResourceManager::instance_;

ResourceManager& ResourceManager::GetRef()
{
	return instance_;
}

ResourceManager* ResourceManager::GetPtr()
{
	return &instance_;
}

void ResourceManager::Destroy(const IResource* resource)
{
	int32_t resourceID = -1;
	for (uint32_t index = 0; index < size_; ++index)
	{
		IResource* resourcePtr = resources_[index].get();
		if (resource == resourcePtr)
		{
			resourceID = static_cast<int32_t>(index);
			break;
		}
	}

	if (resourceID != -1 && resources_[resourceID])
	{
		Unregister(resource);

		if (resources_[resourceID]->IsInitialized())
		{
			resources_[resourceID]->Release();
		}

		resources_[resourceID].reset();
		usage_[resourceID] = false;
	}
}

void ResourceManager::Register(const std::string& name, IResource* resource)
{
	auto it = resourceCache_.find(name);
	ASSERT(it == resourceCache_.end(), "Already register '%s'", name.c_str());

	resourceCache_.insert({ name, resource });
}

bool ResourceManager::IsRegistration(const std::string& name)
{
	return resourceCache_.find(name) != resourceCache_.end();
}

void ResourceManager::Unregister(const std::string& name)
{
	auto it = resourceCache_.find(name);
	ASSERT(it != resourceCache_.end(), "Can't find '%s' in ResourceManager.", name.c_str());

	resourceCache_.erase(it);
}

void ResourceManager::Unregister(const IResource* resource)
{
	std::string name;
	for (const auto& resourceCache : resourceCache_)
	{
		if (resourceCache.second == resource)
		{
			name = resourceCache.first;
		}
	}

	if (!name.empty())
	{
		Unregister(name);
	}
}

void ResourceManager::Cleanup()
{
	for (std::size_t index = 0; index < size_; ++index)
	{
		if (resources_[index])
		{
			if (resources_[index]->IsInitialized())
			{
				resources_[index]->Release();
			}

			resources_[index].reset();
			usage_[index] = false;
		}
	}

	size_ = 0;
}