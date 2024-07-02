#pragma once

#include <array>
#include <memory>

#include "IEntity.h"
#include "IManager.h"


namespace GameMaker
{
/**
 * @brief 게임 내의 엔티티를 관리하는 매니저입니다.
 * 
 * @note 이 클래스는 싱글턴입니다.
 */
class EntityManager : public IManager
{
public:
	/**
	 * @brief 엔티티 매니저의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(EntityManager);


	/**
	 * @brief 엔티티 매니저의 참조자를 얻습니다.
	 *
	 * @return 엔티티 매니저의 참조자를 반환합니다.
	 */
	static EntityManager& Get();


	/**
	 * @brief 엔티티 매니저의 사용을 시작합니다.
	 *
	 * @note 이 메서드는 한 번만 호출되어야 합니다.
	 */
	virtual void Startup() override;


	/**
	 * @brief 엔티티 매니저의 사용을 종료합니다.
	 *
	 * @note
	 * - 애플리케이션 종료 전에 이 메서드를 반드시 호출하여 내부 엔티티를 정리해야 합니다.
	 * - 이 메서드는 반드시 한 번만 호출되어야 합니다.
	 */
	virtual void Shutdown() override;


	/**
	 * @brief 엔티티를 생성합니다.
	 *
	 * @param args 엔티티의 생성자에 전달한 인자들입니다.
	 *
	 * @return 생성된 엔티티의 포인터를 반환합니다. 엔티티 생성에 실패하면 nullptr를 반환합니다.
	 */
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


	/**
	 * @brief 엔티티 매니저가 관리하는 엔티티를 삭제합니다.
	 *
	 * @param entity 삭제할 엔티티 포인터입니다.
	 */
	void Destroy(const IEntity* entity)
	{
		int32_t entityID = -1;
		for (uint32_t index = 0; index < cacheSize_; ++index)
		{
			IEntity* cache = cache_[index].get();
			if (entity == cache)
			{
				entityID = static_cast<int32_t>(index);
				break;
			}
		}

		if (entityID != -1 && cache_[entityID])
		{
			if (cache_[entityID]->IsInitialized())
			{
				cache_[entityID]->Release();
			}

			cache_[entityID].reset();
			usage_[entityID] = false;
		}
	}


private:
	/**
	 * @brief 엔티티 매니저에 디폴트 생성자와 빈 가상 소멸자를 삽입합니다.
	 */
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(EntityManager);


private:
	/**
	 * @brief 생성할 수 있는 최대 엔티티 수입니다.
	 */
	static const uint32_t MAX_ENTITY_SIZE = 200;


	/**
	 * @brief 캐시 크기입니다.
	 */
	uint32_t cacheSize_ = 0;


	/**
	 * @brief 엔티티의 캐시입니다.
	 */
	std::array<std::unique_ptr<IEntity>, MAX_ENTITY_SIZE> cache_;


	/**
	 * @brief 엔티티의 사용 여부입니다.
	 */
	std::array<bool, MAX_ENTITY_SIZE> usage_;
};

}