#pragma warning(push)
#pragma warning(disable: 26451)

#include <glad/glad.h>
#include <stb_rect_pack.h>
#include <stb_truetype.h>

#include "Assertion.h"
#include "GameUtils.h"
#include "TTFont.h"

TTFont::TTFont(const std::string& path, int32_t beginCodePoint, int32_t endCodePoint, float fontSize)
	: beginCodePoint_(beginCodePoint)
	, endCodePoint_(endCodePoint)
	, fontSize_(fontSize)
{
	std::vector<uint8_t> buffer = GameUtils::ReadFile(path);

	stbtt_fontinfo info;
	const uint8_t* bufferPtr = reinterpret_cast<const uint8_t*>(buffer.data());
	ASSERT((stbtt_InitFont(&info, bufferPtr, stbtt_GetFontOffsetForIndex(bufferPtr, 0)) != 0), "Failed to initialize stb_truetype font.");

	std::shared_ptr<uint8_t[]> bitmap = CreateGlyphAtlasBitmap(buffer);
	atlasID_ = CreateGlyphAtlasResource(bitmap);

	bIsInitialized_ = true;
}

TTFont::~TTFont()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void TTFont::Release()
{
	CHECK(bIsInitialized_);

	GL_CHECK(glDeleteTextures(1, &atlasID_));

	bIsInitialized_ = false;
}

void TTFont::Active(uint32_t unit) const
{
	GL_CHECK(glActiveTexture(GL_TEXTURE0 + unit));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, atlasID_));
}

const Glyph& TTFont::GetGlyph(int32_t codePoint) const
{
	CHECK(IsValidCodePoint(codePoint));

	int32_t index = codePoint - beginCodePoint_;
	return glyphs_[index];
}

bool TTFont::IsValidCodePoint(int32_t codePoint) const
{
	return (beginCodePoint_ <= codePoint) && (codePoint <= endCodePoint_);
}

void TTFont::MeasureText(const std::wstring& text, float& outWidth, float& outHeight) const
{
	float width = 0;
	float minY = +1.0f;
	float maxY = -1.0f;

	for (uint32_t index = 0; index < text.size(); ++index)
	{
		const Glyph& glyph = GetGlyph(static_cast<int32_t>(text[index]));

		if (index == text.size() - 1)
		{
			width += static_cast<float>(glyph.pos1.x - glyph.pos0.x) + glyph.xoff;
		}
		else
		{
			width += glyph.xadvance;
		}

		float y0 = glyph.yoff;
		float y1 = static_cast<float>(glyph.pos1.y - glyph.pos0.y) + 2.0f * glyph.yoff;

		minY = GameMath::Min<float>(y0, minY);
		maxY = GameMath::Max<float>(y1, maxY);
	}

	outWidth = width;
	outHeight = static_cast<float>(GameMath::Abs(maxY - minY));
}

std::shared_ptr<uint8_t[]> TTFont::CreateGlyphAtlasBitmap(const std::vector<uint8_t>& buffer)
{
	std::size_t glyphSize = static_cast<std::size_t>(endCodePoint_ - beginCodePoint_ + 1);

	std::vector<stbtt_packedchar> packedchars(glyphSize);
	glyphs_ = std::vector<Glyph>(glyphSize);

	atlasWidth_ = 16;
	atlasHeight_ = 16;

	stbtt_pack_context packContext;
	std::shared_ptr<uint8_t[]> bitmap = nullptr;

	while (atlasWidth_ <= 4096 || atlasHeight_ <= 4096)
	{
		bitmap = std::make_unique<uint8_t[]>(atlasWidth_ * atlasHeight_);

		int32_t success = stbtt_PackBegin(&packContext, bitmap.get(), atlasWidth_, atlasHeight_, 0, 1, nullptr);
		stbtt_PackSetOversampling(&packContext, 1, 1);
		
		success = stbtt_PackFontRange(&packContext, buffer.data(), 0, fontSize_, beginCodePoint_, static_cast<int>(packedchars.size()), packedchars.data());
		if (success)
		{
			stbtt_PackEnd(&packContext);
			break;
		}
		else
		{
			stbtt_PackEnd(&packContext);
			bitmap.reset();

			if (atlasWidth_ == atlasHeight_)
			{
				atlasWidth_ *= 2;
			}
			else
			{
				atlasHeight_ *= 2;
			}
		}
	}

	ASSERT(bitmap != nullptr, "Failed to create glyph atlas from stb_truetype.");
	for (std::size_t index = 0; index < packedchars.size(); ++index)
	{
		glyphs_[index].codePoint = static_cast<int32_t>(index + beginCodePoint_);
		glyphs_[index].pos0 = Vec2i(packedchars[index].x0, packedchars[index].y0);
		glyphs_[index].pos1 = Vec2i(packedchars[index].x1, packedchars[index].y1);
		glyphs_[index].xoff = packedchars[index].xoff;
		glyphs_[index].yoff = packedchars[index].yoff;
		glyphs_[index].xoff2 = packedchars[index].xoff2;
		glyphs_[index].yoff2 = packedchars[index].yoff2;
		glyphs_[index].xadvance = packedchars[index].xadvance;
	}

	return bitmap;
}

uint32_t TTFont::CreateGlyphAtlasResource(const std::shared_ptr<uint8_t[]>& bitmap)
{
	uint32_t atlasID;

	GL_CHECK(glGenTextures(1, &atlasID));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, atlasID));

	GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	const void* bufferPtr = reinterpret_cast<const void*>(&bitmap[0]);
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth_, atlasHeight_, 0, GL_RED, GL_UNSIGNED_BYTE, bufferPtr));

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

	return atlasID;
}

#pragma warning(pop)