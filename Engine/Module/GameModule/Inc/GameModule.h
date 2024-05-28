#include <array>
#include <cstdint>
#include <memory>

#include "IEntity.h"


/**
 * @brief 게임 엔티티를 관리하는 게임 모듈입니다.
 * 
 * @note 이 클래스의 모든 멤버 변수와 메서드는 모두 정적(static) 타입입니다.
 */
class GameModule
{
	/**
	 * @brief 게임 모듈의 에러 처리 목록입니다.
	 */
	enum class Errors
	{
		OK            = 0x00, // 에러가 없습니다.
		FAILED        = 0x01, // 일반적인 에러입니다.
		ERR_PREV_INIT = 0x05, // 이전에 초기화를 수행했습니다.
		ERR_NOT_INIT  = 0x06, // 초기화가 수행되지 않았습니다.
	};


public:
	/**
	 * @brief 게임 모듈을 초기화합니다.
	 * 
	 * @return 초기화에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors Init();


	/**
	 * @brief 게임 모듈의 초기화를 해제합니다.
	 * 
	 * @return 초기화 해제에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors Uninit();


	/**
	 * @brief 엔티티를 생성합니다.
	 *
	 * @param args 엔티티의 생성자에 전달한 인자들입니다.
	 *
	 * @return 생성된 엔티티의 포인터를 반환합니다. 엔티티 생성에 실패하면 nullptr를 반환합니다.
	 */
	template <typename TEntity, typename... Args>
	static TEntity* CreateEntity(Args&&... args)
	{
		if (!(0 <= cacheSize && cacheSize < MAX_ENTITY_COUNT))
		{
			return nullptr;
		}

		int32_t id = -1;
		for (uint32_t index = 0; index < cacheSize; ++index)
		{
			if (!cache[index] && !usage[index])
			{
				id = static_cast<int32_t>(index);
				break;
			}
		}

		if (id == -1)
		{
			id = cacheSize++;
		}

		usage[id] = true;
		cache[id] = std::make_unique<TEntity>(args...);

		return reinterpret_cast<TEntity*>(cache[id].get());
	}


	/**
	 * @brief 엔티티 매니저가 관리하는 엔티티를 삭제합니다.
	 *
	 * @param entity 삭제할 엔티티의 포인터입니다.
	 */
	static void DestroyEntity(const IEntity* entity)
	{
		int32_t id = -1;
		for (uint32_t index = 0; index < cacheSize; ++index)
		{
			IEntity* cacheEntity = cache[index].get();
			if (entity == cacheEntity)
			{
				id = static_cast<int32_t>(index);
				break;
			}
		}

		if (id != -1 && cache[id])
		{
			if (cache[id]->IsInitialized())
			{
				cache[id]->Release();
			}

			usage[id] = false;
			cache[id].reset();
		}
	}


private:
	/**
	 * @brief 게임 모듈이 초기화된 적이 있는지 확인합니다.
	 */
	static bool bIsInit;


	/**
	 * @brief 생성할 수 있는 최대 엔티티 수입니다.
	 */
	static const uint32_t MAX_ENTITY_COUNT = 200;


	/**
	 * @brief 캐시 크기입니다.
	 */
	static uint32_t cacheSize;


	/**
	 * @brief 엔티티의 캐시입니다.
	 */
	static std::array<std::unique_ptr<IEntity>, MAX_ENTITY_COUNT> cache;


	/**
	 * @brief 엔티티의 사용 여부입니다.
	 */
	static std::array<bool, MAX_ENTITY_COUNT> usage;
};