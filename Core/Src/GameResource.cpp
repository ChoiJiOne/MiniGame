#include <array>
#include <memory>

#include "GameAssert.h"
#include "GameResource.h"

struct ResourceBlock
{
	std::unique_ptr<IResource> resourcePtr = nullptr;
	bool bUsage = false;
};

static const uint32_t MAX_RESOURCE_SIZE = 200;
static uint32_t blockOffset_ = 0;
static std::array<ResourceBlock, MAX_RESOURCE_SIZE> resourceBlocks_;

template <typename TResource, typename... Args>
TResource* GameResource::Create(Args&&... args)
{
	if (!(0 <= blockOffset_ && blockOffset_ < MAX_RESOURCE_SIZE))
	{
		return nullptr;
	}

	int32_t resourceID = -1;
	for (uint32_t index = 0; index < blockOffset_; ++index)
	{
		if (!resourceBlocks_[index].resourcePtr && !resourceBlocks_[index].bUsage)
		{
			resourceID = static_cast<int32_t>(index);
			break;
		}
	}

	if (resourceID == -1)
	{
		resourceID = blockOffset_++;
	}

	resourceBlocks_[resourceID].bUsage = true;
	resourceBlocks_[resourceID].resourcePtr = std::make_unique<TResource>(args...);

	return reinterpret_cast<TResource*>(resourceBlocks_[index].resourcePtr.get());
}

void GameResource::Destroy(const IResource* resource)
{
	int32_t resourceID = -1;
	for (uint32_t index = 0; index < blockOffset_; ++index)
	{
		IResource* resourcePtr = resourceBlocks_[index].resourcePtr.get();
		if (resource == resourcePtr)
		{
			resourceID = static_cast<int32_t>(index);
			break;
		}
	}

	if (resourceID != -1 && resourceBlocks_[resourceID].resourcePtr)
	{
		if (resourceBlocks_[resourceID].resourcePtr->IsInitialized())
		{
			resourceBlocks_[resourceID].resourcePtr->Release();
		}

		resourceBlocks_[resourceID].resourcePtr.reset();
		resourceBlocks_[resourceID].bUsage = false;
	}
}