#pragma once

#include "Vec.h"

#include "ITexture2D.h"


/**
 * @brief 2D 체크 보드 텍스처입니다.
 *
 * @note
 * - 체크 보드의 크기는 반드시 2^n 입니다.
 * - 체크 보드의 크기는 외부에서 특수한 값으로만 지정할 수 있습니다.
 */
class Checkboard : public ITexture2D
{
public:
	/**
	 * @brief 체크 보드의 크기입니다.
	 */
	enum class ESize
	{
		Size_2x2 = 2,
		Size_4x4 = 4,
		Size_8x8 = 8,
		Size_16x16 = 16,
		Size_32x32 = 32,
		Size_64x64 = 64,
		Size_128x128 = 128,
		Size_256x256 = 256,
		Size_512x512 = 512,
		Size_1024x1024 = 1024,
		Size_2048x2048 = 2048,
	};


public:
	/**
	 * @brief 체크 보드의 생성자입니다.
	 *
	 * @param boardSize 타일맵 전체의 크기입니다.
	 * @param size 체크 보드 하나의 크기입니다.
	 * @param color0 타일의 첫 번째 패턴 색상입니다.
	 * @param color1 타일의 두 번째 패턴 색상입니다.
	 */
	Checkboard(const ESize& boardSize, const ESize& size, const Vec4f& color0 = Vec4f(1.0f, 1.0f, 1.0f, 1.0f), const Vec4f& color1 = Vec4f(0.0f, 0.0f, 0.0f, 1.0f));


	/**
	 * @brief 체크 보드의 가상 소멸자입니다.
	 */
	virtual ~Checkboard();


	/**
	 * @brief 체크 보드의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Checkboard);


	/**
	 * @brief 체크 보드 내부 리소스를 할당 해제합니다.
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
	 * @brief 체크 보드의 가로 크기입니다.
	 *
	 * @note 체크 보드의 크기는 반드시 2^n 입니다.
	 */
	int32_t width_ = 0;


	/**
	 * @brief 체크 보드의 세로 크기입니다.
	 *
	 * @note 체크 보드의 크기는 반드시 2^n 입니다.
	 */
	int32_t height_ = 0;


	/**
	 * @brief 텍스처 리소스의 ID 값입니다.
	 */
	uint32_t textureID_ = 0;
};