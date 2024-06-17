#include "GeometryRenderer2D.h"
#include "TextRenderer.h"
#include "TTFont.h"

#include "Character.h"
#include "StatusViewer.h"
#include "StringUtils.h"

StatusViewer::StatusViewer(Character* character, GeometryRenderer2D* geometryRenderer, TextRenderer* textRenderer, TTFont* font)
	: character_(character)
	, geometryRenderer_(geometryRenderer)
	, textRenderer_(textRenderer)
	, font_(font)
{
	backgroundColor_ = Vec4f(0.0f, 0.0f, 0.0f, 0.5f);
	center_ = Vec2f(700.0f, 50.0f);
	width_ = 160.0f;
	height_ = 80.0f;
	side_ = 10.0f;

	textColor_ = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	currentCoinCount_ = character_->GetCoinCount();
	currentCoinCountText_ = StringUtils::PrintF(L"COIN: %3d", currentCoinCount_);
	currentCoinCountTextPosition_ = Vec2f(640.0f, 20.0f);
	currentRemainTime_ = static_cast<int32_t>(character_->GetRemainTime());
	currentRemainTimeText_ = StringUtils::PrintF(L"TIME: %3d", currentRemainTime_);
	currentRemainTimeTextPosition_ = Vec2f(640.0f, 60.0f);
	countDownPosition_ = center_ + Vec2f(0.0f, height_ * 0.7f);
	countDownWidth_ = width_;
	countDownHeight_ = 24.0f;
	countDownBackgroundColor_ = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	countDownColor_ = Vec4f(0.0f, 0.0f, 1.0f, 1.0f);

	bIsInitialized_ = true;
}

StatusViewer::~StatusViewer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void StatusViewer::Tick(float deltaSeconds)
{
	int32_t currentCoinCount = character_->GetCoinCount();
	int32_t currentRemainTime = static_cast<int32_t>(character_->GetRemainTime());

	if (currentCoinCount != currentCoinCount_)
	{
		currentCoinCount_ = currentCoinCount;
		currentCoinCountText_ = StringUtils::PrintF(L"COIN: %3d", currentCoinCount_);
	}

	if (currentRemainTime != currentRemainTime_)
	{
		currentRemainTime_ = currentRemainTime;
		currentRemainTimeText_ = StringUtils::PrintF(L"TIME: %3d", currentRemainTime_);
	}
}

void StatusViewer::Render()
{
	geometryRenderer_->DrawRoundRectangle2D(center_, width_, height_, side_, 0.0f, backgroundColor_);
	textRenderer_->DrawText2D(font_, currentCoinCountText_, currentCoinCountTextPosition_, EStartPivot::LEFT_TOP, textColor_);

	Vec4f remainTimeTextColor = textColor_;
	Vec4f countDownColor = countDownColor_;
	if (currentRemainTime_ <= 5.0f)
	{
		remainTimeTextColor = Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
		countDownColor = remainTimeTextColor;
	}
	textRenderer_->DrawText2D(font_, currentRemainTimeText_, currentRemainTimeTextPosition_, EStartPivot::LEFT_TOP, remainTimeTextColor);

	float maxRemainTime = character_->GetMaxRemainTime();
	float rate = character_->GetRemainTime() / maxRemainTime;
	geometryRenderer_->DrawHorizonProgressBar2D(countDownPosition_, countDownWidth_, countDownHeight_, rate, countDownColor, countDownBackgroundColor_);
}

void StatusViewer::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}
