#include "Assertion.h"
#include "TextUI.h"
#include "TTFont.h"

TextUI::TextUI(const Layout& layout)
	: layout_(layout)
{
	layout_.font->MeasureText(layout_.text, textSize_.x, textSize_.y);
	textPos_ = layout_.textCenterPos + Vec2f(-textSize_.x * 0.5f, +textSize_.y * 0.5f);

	bIsInitialized_ = true;
}

TextUI::~TextUI()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void TextUI::Tick(float deltaSeconds)
{
}

void TextUI::Release()
{
	CHECK(bIsInitialized_);

	bIsInitialized_ = false;
}

void TextUI::SetText(const std::wstring& text)
{
	layout_.text = text;

	layout_.font->MeasureText(layout_.text, textSize_.x, textSize_.y);
	textPos_ = layout_.textCenterPos + Vec2f(-textSize_.x * 0.5f, +textSize_.y * 0.5f);
}
