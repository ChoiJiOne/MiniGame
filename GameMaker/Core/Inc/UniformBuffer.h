#pragma once

#include <cstdint>

#include "IResource.h"


namespace GameMaker
{
/**
 * @brief 유니폼 버퍼입니다.
 */
class UniformBuffer : public IResource
{
public:
	/**
	 * @brief 유니폼 버퍼의 사용 형식입니다.
	 *
	 * @see https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBufferData.xml
	 */
	enum class EUsage
	{
		STREAM  = 0x88E0,
		STATIC  = 0x88E4,
		DYNAMIC = 0x88E8,
	};


public:
	/**
	 * @brief 유니폼 버퍼를 생성합니다.
	 *
	 * @param byteSize 유니폼 버퍼의 바이트 크기입니다.
	 * @param usage 유니폼 버퍼의 사용 형식입니다.
	 */
	UniformBuffer(uint32_t byteSize, const EUsage& usage);


	/**
	 * @brief 유니폼 버퍼를 생성합니다.
	 *
	 * @param bufferPtr 초기화할 버퍼의 포인터입니다.
	 * @param byteSize 유니폼 버퍼의 바이트 크기입니다.
	 * @param usage 유니폼 버퍼의 사용 형식입니다.
	 */
	UniformBuffer(const void* bufferPtr, uint32_t byteSize, const EUsage& usage);


	/**
	 * @brief 유니폼 버퍼를 할당 해제합니다.
	 */
	virtual ~UniformBuffer();


	/**
	 * @brief 유니폼 버퍼의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(UniformBuffer);


	/**
	 * @brief 유니폼 버퍼의 내부에서 할당된 리소스의 할당을 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 유니폼 버퍼를 바인딩합니다.
	 */
	void Bind();


	/**
	 * @brief 유니폼 버퍼를 지정한 바인딩 포인트에 바인딩합니다.
	 *
	 * @param slot 유니폼 버퍼를 바인딩할 슬롯입니다.
	 *
	 * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBufferBase.xhtml
	 */
	void Bind(const uint32_t slot);


	/**
	 * @brief 유니폼 버퍼의 바인딩을 해제합니다.
	 */
	void Unbind();


	/**
	 * @brief 유니폼 버퍼에 데이터를 씁니다.
	 *
	 * @param bufferPtr 버퍼에 쓸 데이터의 포인터입니다.
	 * @param bufferSize 버퍼에 쓸 데이터의 크기입니다.
	 */
	void SetBufferData(const void* bufferPtr, uint32_t bufferSize);


private:
	/**
	 * @brief 유니폼 버퍼 오브젝트 ID입니다.
	 */
	uint32_t uniformBufferID_ = 0;


	/**
	 * @brief 유니폼 버퍼의 바이트 크기입니다.
	 */
	uint32_t byteSize_ = 0;


	/**
	 * @brief 유니폼 버퍼의 사용 형식입니다.
	 */
	EUsage usage_ = EUsage::STATIC;
};

}