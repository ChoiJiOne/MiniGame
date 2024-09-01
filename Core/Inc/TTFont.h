#pragma once

#include <memory>
#include <string>
#include <vector>

#include "GameMath.h"
#include "ITexture.h"

struct Glyph
{
	int32_t codePoint;
	GameMath::Vec2i pos0;
	GameMath::Vec2i pos1;
	float xoff;
	float yoff;
	float xoff2;
	float yoff2;
	float xadvance;
};

/** 트루 타입 폰트는 .ttf 만 지원합니다. */
class TTFont : public ITexture
{
public:
	explicit TTFont(const std::string& path, int32_t beginCodePoint, int32_t endCodePoint, float fontSize);
	virtual ~TTFont();

	DISALLOW_COPY_AND_ASSIGN(TTFont);

	virtual void Release() override;
	virtual void Active(uint32_t unit) const override;

	const Glyph& GetGlyph(int32_t codePoint) const;
	int32_t GetAtlasSize() const { return atlasSize_; }
	bool IsValidCodePoint(int32_t codePoint) const;
	void MeasureText(const std::wstring& text, float& outWidth, float& outHeight) const;

private:
	std::shared_ptr<uint8_t[]> CreateGlyphAtlasBitmap(const std::vector<uint8_t>& buffer, int32_t beginCodePoint, int32_t endCodePoint, float fontSize, std::vector<Glyph>& outGlyphs, int32_t& outAtlasSize);
	uint32_t CreateGlyphAtlasResource(const std::shared_ptr<uint8_t[]>& bitmap, const int32_t& atlasSize);

private:
	int32_t beginCodePoint_ = 0;
	int32_t endCodePoint_ = 0;
	int32_t atlasSize_ = 0;
	std::vector<Glyph> glyphs_;
	uint32_t atlasID_ = 0;
};