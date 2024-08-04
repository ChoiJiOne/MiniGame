#pragma once

#include <array>
#include <memory>

#include "IEntity.h"
#include "IManager.h"


namespace GameMaker
{
class EntityManager : public IManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(EntityManager);
	
	static EntityManager& Get();

	virtual void Startup() override;
	virtual void Shutdown() override;

	template <typename TEntity, typename... Args>
	TEntity* Create(Args&&... args)
	{
		if (!(0 <= cacheSize_ && cacheSize_ < MAX_ENTITY_SIZE))
		{
			return nullptr;
		}

		int32_t entityID = -1;
		for (uint32_t index = 0; index < cacheSize_; ++index)
		{
			if (!cache_[index] && !usage_[index])
			{
				entityID = static_cast<int32_t>(index);
				break;
			}
		}

		if (entityID == -1)
		{
			entityID = cacheSize_++;
		}

		usage_[entityID] = true;
		cache_[entityID] = std::make_unique<TEntity>(args...);

		return reinterpret_cast<TEntity*>(cache_[entityID].get());
	}
	
	void Destroy(const IEntity* entity);

private:
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(EntityManager);

private:
	static const uint32_t MAX_ENTITY_SIZE = 200;

	uint32_t cacheSize_ = 0;
	std::array<std::unique_ptr<IEntity>, MAX_ENTITY_SIZE> cache_;
	std::array<bool, MAX_ENTITY_SIZE> usage_;
};

}