#pragma once

#include <array>
#include <memory>

#include "IManager.h"
#include "IResource.h"


namespace GameMaker
{
/**
 * @brief 엔진의 리소스를 관리하는 매니저입니다.
 * 
 * @note 이 클래스는 싱글턴입니다.
 */
class ResourceManager : public IManager
{
public:
	/**
	 * @brief 리소스 매니저의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(ResourceManager);


	/**
	 * @brief 리소스 매니저의 참조자를 얻습니다.
	 *
	 * @return 리소스 매니저의 참조자를 반환합니다.
	 */
	static ResourceManager& Get();


	/**
	 * @brief 리소스 매니저의 사용을 시작합니다.
	 *
	 * @note 이 메서드는 한 번만 호출되어야 합니다.
	 */
	virtual void Startup() override;


	/**
	 * @brief 리소스 매니저의 사용을 종료합니다.
	 *
	 * @note
	 * - 애플리케이션 종료 전에 이 메서드를 반드시 호출하여 내부 리소스를 정리해야 합니다.
	 * - 이 메서드는 반드시 한 번만 호출되어야 합니다.
	 */
	virtual void Shutdown() override;


	/**
	 * @brief 리소스를 생성합니다.
	 *
	 * @param args 리소스의 생성자에 전달한 인자들입니다.
	 *
	 * @return 생성된 리소스의 포인터를 반환합니다. 리소스 생성에 실패하면 nullptr를 반환합니다.
	 */
	template <typename TResource, typename... Args>
	TResource* Create(Args&&... args)
	{
		if (!(0 <= cacheSize_ && cacheSize_ < MAX_RESOURCE_SIZE))
		{
			return nullptr;
		}

		int32_t resourceID = -1;
		for (uint32_t index = 0; index < cacheSize_; ++index)
		{
			if (!cache_[index] && !usage_[index])
			{
				resourceID = static_cast<int32_t>(index);
				break;
			}
		}

		if (resourceID == -1)
		{
			resourceID = cacheSize_++;
		}

		usage_[resourceID] = true;
		cache_[resourceID] = std::make_unique<TResource>(args...);

		return reinterpret_cast<TResource*>(cache_[resourceID].get());
	}


	/**
	 * @brief 리소스 매니저가 관리하는 리소스를 삭제합니다.
	 *
	 * @param resource 삭제할 리소스 포인터입니다.
	 */
	void Destroy(const IResource* resource)
	{
		int32_t resourceID = -1;
		for (uint32_t index = 0; index < cacheSize_; ++index)
		{
			IResource* cacheResource = cache_[index].get();
			if (resource == cacheResource)
			{
				resourceID = static_cast<int32_t>(index);
				break;
			}
		}

		if (resourceID != -1 && cache_[resourceID])
		{
			if (cache_[resourceID]->IsInitialized())
			{
				cache_[resourceID]->Release();
			}

			cache_[resourceID].reset();
			usage_[resourceID] = false;
		}
	}


private:
	/**
	 * @brief 리소스 매니저에 디폴트 생성자와 빈 가상 소멸자를 삽입합니다.
	 */
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(ResourceManager);


private:
	/**
	 * @brief 생성할 수 있는 최대 리소스 수입니다.
	 */
	static const uint32_t MAX_RESOURCE_SIZE = 200;


	/**
	 * @brief 캐시 크기입니다.
	 */
	uint32_t cacheSize_ = 0;


	/**
	 * @brief 리소스의 캐시입니다.
	 */
	std::array<std::unique_ptr<IResource>, MAX_RESOURCE_SIZE> cache_;


	/**
	 * @brief 리소스의 사용 여부입니다.
	 */
	std::array<bool, MAX_RESOURCE_SIZE> usage_;
};

}