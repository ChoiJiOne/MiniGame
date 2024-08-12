#include "Assertion.h"
#include "Camera2D.h"
#include "GameUI.h"
#include "TTFont.h"

using namespace GameMaker;

ButtonUI::ButtonUI(const UILayout& layout, const EMouse& mouseButton, const std::function<void()>& clickEvent, bool bIsActive)
	: EntityUI(layout, bIsActive)
	, mouseButton_(mouseButton)
	, clickEvent_(clickEvent)
{
	bIsInitialized_ = true;
}

ButtonUI::~ButtonUI()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void ButtonUI::Tick(float deltaSeconds)
{
	if (!bIsActive_)
	{
		return;
	}
}

void ButtonUI::Release()
{
	CHECK(bIsInitialized_);
	bIsInitialized_ = false;
}