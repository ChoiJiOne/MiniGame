#include "Assertion.h"
#include "EntityManager.h"

EntityManager& EntityManager::Get()
{
	static EntityManager instance;
	return instance;
}

void EntityManager::Destroy(const IEntity* entity)
{
	int32_t entityID = -1;
	for (uint32_t index = 0; index < size_; ++index)
	{
		IEntity* entityPtr = entities_[index].get();
		if (entity == entityPtr)
		{
			entityID = static_cast<int32_t>(index);
			break;
		}
	}

	if (entityID != -1 && entities_[entityID])
	{
		if (entities_[entityID]->IsInitialized())
		{
			entities_[entityID]->Release();
		}

		entities_[entityID].reset();
		usage_[entityID] = false;
	}
}

void EntityManager::Cleanup()
{
	for (std::size_t index = 0; index < size_; ++index)
	{
		if (entities_[index])
		{
			if (entities_[index]->IsInitialized())
			{
				entities_[index]->Release();
			}

			entities_[index].reset();
			usage_[index] = false;
		}
	}

	size_ = 0;
}