#pragma once

#include "Vec.h"

#include "ITexture.h"


/**
 * @brief 베이스 컬러 맵입니다.
 * 
 * @note
 * - 텍스처의 크기는 반드시 2^n 단위입니다.
 * - 텍스처의 크기는 내부의 열거형으로만 설정할 수 있습니다.
 * - 이 텍스처는 단색입니다.
 */
class BaseColorMap : public ITexture
{
public:
	/**
	 * @brief 텍스처의 크기입니다.
	 */
	enum class ESize
	{
		Size_2x2       = 2,
		Size_4x4       = 4,
		Size_8x8       = 8,
		Size_16x16     = 16,
		Size_32x32     = 32,
		Size_64x64     = 64,
		Size_128x128   = 128,
		Size_256x256   = 256,
		Size_512x512   = 512,
		Size_1024x1024 = 1024,
		Size_2048x2048 = 2048,
	};


public:
	/**
	 * @brief 베이스 컬러 맵의 생성자입니다.
	 * 
	 * @param size 베이스 컬러 맵의 크기입니다.
	 * @param baseColor 베이스 컬러 맵의 색상입니다.
	 */
	BaseColorMap(const ESize& size, const Vec4f& baseColor);


	/**
	 * @brief 베이스 컬러 맵의 가상 소멸자입니다.
	 */
	virtual ~BaseColorMap();


	/**
	 * @brief 베이스 컬러 맵의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(BaseColorMap);


	/**
	 * @brief 베이스 컬러 맵 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 텍스처를 활성화합니다.
	 *
	 * @param unit 활성화 할 텍스처 유닛입니다.
	 */
	virtual void Active(uint32_t unit) const override;


private:
	/**
	 * @brief 베이스 컬러 맵의 가로 크기입니다.
	 */
	int32_t width_ = 0;


	/**
	 * @brief 베이스 컬러 맵의 세로 크기입니다.
	 */
	int32_t height_ = 0;


	/**
	 * @brief 텍스처 리소스의 ID 값입니다.
	 */
	uint32_t textureID_ = 0;
};