#pragma once

#include "ITexture2D.h"


/**
 * @brief 셰도우 맵입니다.
 *
 * @note
 * - 세도우 맵의 가로/세로 크기는 동일합니다.
 * - 셰도우 맵의 크기는 2^n 형식으로 되어 있습니다.
 */
class ShadowMap : public ITexture2D
{
public:
	/**
	 * @brief 셰도우 맵의 크기입니다.
	 */
	enum class ESize
	{
		Size_256x256   = 256,
		Size_512x512   = 512,
		Size_1024x1024 = 1024,
	};


public:
	/**
	 * @brief 셰도우 맵의 생성자입니다.
	 *
	 * @param size 셰도우 맵의 크기입니다.
	 */
	ShadowMap(const ESize& size);


	/**
	 * @brief 셰도우 맵의 가상 소멸자입니다.
	 */
	virtual ~ShadowMap();


	/**
	 * @brief 셰도우 맵의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(ShadowMap);


	/**
	 * @brief 셰도우 맵 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 셰도우 맵을 활성화합니다.
	 *
	 * @param unit 활성화 할 텍스처 유닛입니다.
	 */
	virtual void Active(uint32_t unit) const override;


	/**
	 * @brief 셰도우 맵의 깊이 값을 초기화합니다.
	 */
	void Clear();


	/**
	 * @brief 셰도우 맵을 바인딩합니다.
	 */
	void Bind();


	/**
	 * @brief 셰도우 맵의 바인딩을 해제합니다.
	 */
	void Unbind();


	/**
	 * @brief 셰도우 맵의 크기를 얻습니다.
	 *
	 * @return 셰도우 맵의 크기를 반환합니다.
	 */
	uint32_t GetSize() const { return size_; }


private:
	/**
	 * @brief 셰도우 맵의 크기입니다.
	 */
	uint32_t size_;


	/**
	 * @brief 셰도우 맵의 프레임 버퍼 ID값입니다.
	 */
	uint32_t shadowMapFBO_ = 0;


	/**
	 * @brief 셰도우 맵의 깊이값 버퍼 ID값입니다.
	 */
	uint32_t shadowMapID_ = 0;
};