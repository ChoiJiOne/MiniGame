#pragma once

#include <cstdint>

#include "IResource.h"


/**
 * @brief 인덱스 버퍼입니다.
 */
class IndexBuffer : public IResource
{
public:
	/**
	 * @brief 엔덱스 버퍼를 생성합니다.
	 *
	 * @param bufferPtr 초기화할 버퍼의 포인터입니다.
	 * @param byteSize 정점 버퍼의 바이트 크기입니다.
	 */
	explicit IndexBuffer(const void* bufferPtr, uint32_t indexCount);


	/**
	 * @brief 인덱스 버퍼를 할당 해제합니다.
	 */
	virtual ~IndexBuffer();


	/**
	 * @brief 인덱스 버퍼의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(IndexBuffer);


	/**
	 * @brief 인덱스 버퍼의 내부에서 할당된 리소스의 할당을 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 인덱스 버퍼를 바인딩합니다.
	 */
	void Bind();


	/**
	 * @brief 인덱스 버퍼의 바인딩을 해제합니다.
	 */
	void Unbind();


	/**
	 * @brief 인덱스 버퍼의 크기를 얻습니다.
	 * 
	 * @return 인덱스 버퍼의 크기를 반환합니다.
	 */
	uint32_t GetIndexCount() const { return indexCount_; }


private:
	/**
	 * @brief 인덱스 버퍼 오브젝트 ID입니다.
	 */
	uint32_t indexBufferID_ = 0;


	/**
	 * @brief 인덱스 버퍼의 크기입니다.
	 */
	uint32_t indexCount_ = 0;
};