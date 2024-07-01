#pragma once

#include <cstdint>

#include "IResource.h"


namespace GameMaker
{
/**
 * @brief 텍스처 리소스 인터페이스입니다.
 */
class ITexture : public IResource
{
public:
	/**
	 * @brief 텍스처 리소스 인터페이스의 디폴트 생성자입니다.
	 */
	ITexture() = default;


	/**
	 * @brief 텍스처 리소스 인터페이스의 가상 소멸자입니다.
	 */
	virtual ~ITexture() {}


	/**
	 * @brief 텍스처 리소스 인터페이스 내부 리소스를 할당 해제합니다.
	 *
	 * @note 텍스처 리소스 인터페이스를 상속받는 하위 클래스에서 반드시 구현해야 합니다.
	 */
	virtual void Release() = 0;


	/**
	 * @brief 텍스처를 활성화합니다.
	 *
	 * @param unit 활성화 할 텍스처 유닛입니다.
	 */
	virtual void Active(uint32_t unit) const = 0;
};

}