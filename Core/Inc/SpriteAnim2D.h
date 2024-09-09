#pragma once

#include <string>
#include <vector>

#include "IResource.h"

class Atlas2D;

class SpriteAnim2D : public IResource
{
public:
	SpriteAnim2D(Atlas2D* atlas, const std::vector<std::string>& clipNames, bool bIsDuration, float animationTime);
	virtual ~SpriteAnim2D();

	DISALLOW_COPY_AND_ASSIGN(SpriteAnim2D);

	virtual void Release() override;

	void Update(float deltaSeconds);

	Atlas2D* GetAtlas() { return atlas_; }

	const std::string& GetCurrentClip() const { return clips_.at(currentIndex_).name; }

	bool IsDuration() const { return bIsDuration_; }
	float GetTime() const { return time_; }
	float GetAnimationTime() const { return animationTime_; }

	void Reset();

private:
	struct Clip
	{
		std::string name;
		float time = 0.0f;
	};

private:
	Atlas2D* atlas_ = nullptr;

	std::vector<Clip> clips_;

	uint32_t currentIndex_ = 0;
	bool bIsDuration_ = false;
	float time_ = 0.0f;
	float animationTime_ = 0.0f;
};