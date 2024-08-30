#include "Assertion.h"
#include "ResourceManager.h"

ResourceManager& ResourceManager::Get()
{
	static ResourceManager instance;
	return instance;
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
		if (resources_[resourceID]->IsInitialized())
		{
			resources_[resourceID]->Release();
		}

		resources_[resourceID].reset();
		usage_[resourceID] = false;
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
