#include "Assertion.h"
#include "Atlas2D.h"
#include "SpriteAnim2D.h"

SpriteAnim2D::SpriteAnim2D(Atlas2D* atlas, const std::vector<std::string>& clipNames, bool bIsDuration, float animationTime)
	: atlas_(atlas)
	, bIsDuration_(bIsDuration)
	, animationTime_(animationTime)
	, clips_(clipNames.size() + 1)
{
	for (uint32_t index = 0; index < clips_.size() - 1; ++index)
	{
		float rate = static_cast<float>(index) / static_cast<float>(clips_.size());

		clips_[index].name = clipNames[index];
		clips_[index].time = animationTime_ * rate;
	}

	clips_.back().name = clips_.front().name;
	clips_.back().time = animationTime_;

	bIsInitialized_ = true;
}

SpriteAnim2D::~SpriteAnim2D()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void SpriteAnim2D::Release()
{
	CHECK(bIsInitialized_);

	atlas_ = nullptr;

	bIsInitialized_ = false;
}

void SpriteAnim2D::Update(float deltaSeconds)
{
	time_ += deltaSeconds;
	if (time_ >= animationTime_)
	{
		if (bIsDuration_)
		{
			time_ -= animationTime_;
		}
		else
		{
			time_ = animationTime_;
			return;
		}
	}

	for (uint32_t index = 0; index < clips_.size() - 1; ++index)
	{
		if (clips_[index].time <= time_ && time_ <= clips_[index + 1].time)
		{
			currentIndex_ = index;
			break;
		}
	}

	if (!clips_.empty())
	{
		time_ = GameMath::Clamp<float>(time_, clips_.front().time, clips_.back().time);
	}
}

void SpriteAnim2D::Reset()
{
	currentIndex_ = 0;
	time_ = 0.0f;
}
