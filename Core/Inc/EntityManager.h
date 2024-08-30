#pragma once

#include <array>
#include <memory>
#include <map>

#include "IEntity.h"

/** 엔티티 매니저는 싱글턴입니다. */
class EntityManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(EntityManager);

	static EntityManager& Get();

	template <typename TEntity, typename... Args>
	TEntity* Create(Args&&... args)
	{
		if (!(0 <= size_ && size_ < MAX_ENTITY_SIZE))
		{
			return nullptr;
		}

		int32_t entityID = -1;
		for (uint32_t index = 0; index < size_; ++index)
		{
			if (!entities_[index] && !usage_[index])
			{
				entityID = static_cast<int32_t>(index);
				break;
			}
		}

		if (entityID == -1)
		{
			entityID = size_++;
		}

		usage_[entityID] = true;
		entities_[entityID] = std::make_unique<TEntity>(args...);

		return reinterpret_cast<TEntity*>(entities_[entityID].get());
	}

	void Destroy(const IEntity* entity);

	void Register(const std::string& name, IEntity* entity);
	bool IsRegistration(const std::string& name);
	void Unregister(const std::string& name);

	template <typename TEntity>
	TEntity* GetByName(const std::string& name)
	{
		auto it = entityCache_.find(name);
		if (it == entityCache_.end())
		{
			return nullptr;
		}

		return reinterpret_cast<TEntity*>(it->second);
	}

private:
	friend class IApp;

	EntityManager() = default;
	virtual ~EntityManager() {}

	void Unregister(const IEntity* entity);

	void Cleanup();

private:
	static const uint32_t MAX_ENTITY_SIZE = 200;

	uint32_t size_ = 0;
	std::array<std::unique_ptr<IEntity>, MAX_ENTITY_SIZE> entities_;
	std::array<bool, MAX_ENTITY_SIZE> usage_;

	std::map<std::string, IEntity*> entityCache_;
};