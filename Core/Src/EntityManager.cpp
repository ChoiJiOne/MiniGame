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
		Unregister(entity);

		if (entities_[entityID]->IsInitialized())
		{
			entities_[entityID]->Release();
		}

		entities_[entityID].reset();
		usage_[entityID] = false;
	}
}

void EntityManager::Register(const std::string& name, IEntity* entity)
{
	auto it = entityCache_.find(name);
	ASSERT(it == entityCache_.end(), "Already register '%s'", name.c_str());

	entityCache_.insert({ name, entity });
}

bool EntityManager::IsRegistration(const std::string& name)
{
	return entityCache_.find(name) != entityCache_.end();
}

void EntityManager::Unregister(const std::string& name)
{
	auto it = entityCache_.find(name);
	ASSERT(it != entityCache_.end(), "Can't find '%s' in EntityManager.", name.c_str());

	entityCache_.erase(it);
}

void EntityManager::Unregister(const IEntity* entity)
{
	std::string name;
	for (const auto& entityCache : entityCache_)
	{
		if (entityCache.second == entity)
		{
			name = entityCache.first;
		}
	}

	if (!name.empty())
	{
		Unregister(name);
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