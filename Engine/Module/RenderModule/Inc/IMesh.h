#pragma once

#include <cstdint>

#include "IResource.h"


/**
 * @brief 메시 리소스 인터페이스입니다.
 */
class IMesh : public IResource
{
public:
	/**
	 * @brief 메시 리소스 인터페이스의 디폴트 생성자입니다.
	 */
	IMesh() = default;


	/**
	 * @brief 메시 리소스 인터페이스의 가상 소멸자입니다.
	 */
	virtual ~IMesh() {}


	/**
	 * @brief 메시 리소스 인터페이스 내부 리소스를 할당 해제합니다.
	 * 
	 * @note 메시 리소스 인터페이스를 상속받는 하위 클래스에서 반드시 구현해야 합니다.
	 */
	virtual void Release() = 0;


	/**
	 * @brief 메시 리소스 인터페이스를 파이프라인에 바인딩합니다.
	 */
	virtual void Bind() = 0;


	/**
	 * @brief 바인딩된 메시 리소스 인터페이스를 바인딩 해제합니다.
	 */
	virtual void Unbind() = 0;


	/**
	 * @brief 메시의 인덱스 수를 얻습니다.
	 *
	 * @return 메시의 인덱스 수를 반환합니다.
	 */
	virtual uint32_t GetIndexCount() const = 0;


	/**
	 * @brief 메시의 정점 수를 얻습니다.
	 * 
	 * @return 메시의 정점 수를 반환합니다.
	 */
	virtual uint32_t GetVertexCount() const = 0;
};