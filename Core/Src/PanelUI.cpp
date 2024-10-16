#include "Assertion.h"
#include "PanelUI.h"
#include "TTFont.h"

PanelUI::PanelUI(const Layout& layout)
	: layout_(layout)
{
	layout_.font->MeasureText(layout_.text, textSize_.x, textSize_.y);
	textPos_ = layout_.center + Vec2f(-textSize_.x * 0.5f, +textSize_.y * 0.5f);

	bIsInitialized_ = true;
}

PanelUI::~PanelUI()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void PanelUI::Tick(float deltaSeconds)
{
}

void PanelUI::Release()
{
	CHECK(bIsInitialized_);

	bIsInitialized_ = false;
}

void PanelUI::SetText(const std::wstring& text)
{
	layout_.text = text;

	layout_.font->MeasureText(layout_.text, textSize_.x, textSize_.y);
	textPos_ = layout_.center + Vec2f(-textSize_.x * 0.5f, +textSize_.y * 0.5f);
}