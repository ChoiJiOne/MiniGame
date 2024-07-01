#pragma once

#include <memory>
#include <string>
#include <vector>

#include "GameMath.h"
#include "IResource.h"


namespace GameMaker
{
/**
 * @brief 텍스처 아틀라스에 표시될 글리프입니다.
 */
struct Glyph
{
	int32_t codePoint;
	Vec2i position0;
	Vec2i position1;
	float xoffset;
	float yoffset;
	float xoffset2;
	float yoffset2;
	float xadvance;
};


/**
 * @brief 트루 타입 폰트 파일을 로딩하고 글리프 아틀라스를 생성 및 관리합니다.
 *
 * @note 트루 타입 폰트 파일의 확장자는 .ttf 만 지원합니다.
 */
class TTFont : public IResource
{
public:
	/**
	 * @brief 트루 타입 폰트 파일을 로딩하고 글리프 아틀라스를 생성합니다.
	 *
	 * @param path 트루 타입 폰트 파일의 경로입니다.
	 * @param beginCodePoint 글리프 텍스처 아틀라스의 코드 포인트 시작점입니다.
	 * @param endCodePoint 글리프 텍스처 아틀라스의 코드 포인트 끝점입니다.
	 * @param fontSize 폰트의 크기입니다.
	 *
	 * @note
	 * - 글리프 텍스처 아틀라스 내에서 시작과 끝의 코드 포인트를 포함합니다.
	 * - 트루 타입 폰트 파일의 확장자는 .ttf 만 지원합니다.
	 */
	explicit TTFont(const std::string& path, int32_t beginCodePoint, int32_t endCodePoint, float fontSize);


	/**
	 * @brief 트루 타입 폰트의 가상 소멸자입니다.
	 *
	 * @note 해제를 수행하기 위해서는 반드시 Release 메서드를 호출해야 합니다.
	 */
	virtual ~TTFont();


	/**
	 * @brief 트루 타입 폰트 클래스의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(TTFont);


	/**
	 * @brief 트루 타입 폰트 클래스 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 글리프 텍스처 아틀라스의 ID를 얻습니다.
	 *
	 * @return 글리프 텍스처 아틀라스의 ID 값을 반환합니다.
	 */
	uint32_t GetGlyphAtlasID() const { return glyphAtlasID_; }


	/**
	 * @brief 코드 포인트에 대응하는 문자 위치 및 크기 정보를 얻습니다.
	 *
	 * @param codePoint 위치 및 정보를 알고 싶은 코드 포인트 값입니다.
	 *
	 * @return 코드 포인트에 대응하는 문자의 위치 및 크기 정보를 반환합니다.
	 */
	const Glyph& GetGlyph(int32_t codePoint) const;


	/**
	 * @brief 텍스처 아틀라스의 크기를 얻습니다.
	 *
	 * @return 텍스처 아틀라스의 크기를 반환합니다.
	 */
	int32_t GetGlyphAtlasSize() const { return glyphAtlasSize_; }


	/**
	 * @brief 코드 포인트가 유효한지 확인합니다.
	 *
	 * @param codePoint 유효성 검사를 수행할 코드 포인트 값입니다.
	 *
	 * @return 코드 포인트가 유효하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsValidCodePoint(int32_t codePoint) const;


	/**
	 * @brief 텍스트의 크기를 측정합니다.
	 *
	 * @note 측정 단위는 픽셀 단위입니다.
	 *
	 * @param text 측정할 텍스트입니다.
	 * @param outWidth 측정한 텍스트의 가로 크기입니다.
	 * @param outHeight 측정한 텍스트의 세로 크기입니다.
	 */
	void MeasureText(const std::wstring& text, float& outWidth, float& outHeight) const;


private:
	/**
	 * @brief 글리프 텍스처 아틀라스 비트맵을 생성합니다.
	 *
	 * @note 글리프 텍스처 아틀라스의 가로 세로 크기는 같습니다.
	 *
	 * @param buffer 트루 타입 폰트 리소스 버퍼입니다.
	 * @param beginCodePoint 문자 텍스처 아틀라스의 코드 포인트 시작점입니다.
	 * @param endCodePoint 문자 텍스처 아틀라스의 코드 포인트 끝점입니다.
	 * @param fontSize 폰트의 크기입니다.
	 * @param outGlyphs 텍스처 아틀라스 상의 코드 포인트에 대응하는 문자 위치 및 크기 정보입니다.
	 * @param outGlyphAtlasSize 글리프 텍스처 아틀라스 크기입니다.
	 *
	 * @return 생성된 글리프 텍스처 아틀라스 비트맵을 반환합니다.
	 */
	std::shared_ptr<uint8_t[]> GenerateGlyphAtlasBitmap(
		const std::vector<uint8_t>& buffer,
		int32_t beginCodePoint,
		int32_t endCodePoint,
		float fontSize,
		std::vector<Glyph>& outGlyphs,
		int32_t& outGlyphAtlasSize
	);


	/**
	 * @brief 텍스처 아틀라스 비트맵으로 부터 텍스처 리소스를 생성합니다.
	 *
	 * @param bitmap 텍스처 아틀라스 비트맵입니다.
	 * @param atlasSize 텍스처 아틀라스 비트맵의 가로 세로 크기입니다
	 *
	 * @return 텍스처 리소스의 ID 값을 반화합니다.
	 */
	uint32_t CreateGlyphAtlasFromBitmap(const std::shared_ptr<uint8_t[]>& bitmap, const int32_t& glyphAtlasSize);


private:
	/**
	 * @brief 문자 텍스처 아틀라스의 코드 포인트 시작점입니다.
	 *
	 * @note 텍스처 아틀라스는 시작점을 포함합니다.
	 */
	int32_t beginCodePoint_ = 0;


	/**
	 * @brief 문자 텍스처 아틀라스의 코드 포인트 끝점입니다.
	 *
	 * @note 텍스처 아틀라스는 끝점을 포함합니다.
	 */
	int32_t endCodePoint_ = 0;


	/**
	 * @brief 글리프 텍스처 아틀라스의 크기입니다.
	 */
	int32_t glyphAtlasSize_ = 0;


	/**
	 * @brief 글리프 텍스처 아틀라스 상의 코드 포인트에 대응하는 문자 위치 및 크기 정보입니다.
	 */
	std::vector<Glyph> glyphs_;


	/**
	 * @brief 글리프 텍스처 아틀라스의 ID입니다.
	 */
	uint32_t glyphAtlasID_ = 0;
};

}