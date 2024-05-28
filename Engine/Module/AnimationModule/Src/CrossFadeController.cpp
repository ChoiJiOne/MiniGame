#include "CrossFadeController.h"
#include "Pose.h"

CrossFadeController::CrossFadeController(const Skeleton& skeleton)
{
	SetSkeleton(skeleton);
}

void CrossFadeController::SetSkeleton(const Skeleton& skeleton)
{
	skeleton_ = skeleton;
	pose_ = skeleton_.GetRestPose();
	bWasSetSkeleton_ = true;
}

void CrossFadeController::Play(Clip* target)
{
	targets_.clear();

	clip_ = target;
	pose_ = skeleton_.GetRestPose();
	time_ = target->GetStartTime();
}

void CrossFadeController::FadeTo(Clip* target, float fadeTime)
{
	if (clip_ == nullptr)
	{
		Play(target);
		return;
	}

	if (targets_.size() >= 1)
	{
		if (targets_.back().clip == target)
		{
			return;
		}
	}
	else
	{
		if (clip_ == target)
		{
			return;
		}
	}

	targets_.push_back(CrossFadeTarget(target, skeleton_.GetRestPose(), fadeTime));
}

void CrossFadeController::Update(float deltaSeconds)
{
	if (!clip_ || !bWasSetSkeleton_)
	{
		return;
	}

	uint32_t numTargets = static_cast<uint32_t>(targets_.size());
	for (uint32_t index = 0; index < numTargets; ++index)
	{
		if (targets_[index].elapsed >= targets_[index].duration)
		{
			clip_ = targets_[index].clip;
			time_ = targets_[index].time;
			pose_ = targets_[index].pose;

			targets_.erase(targets_.begin() + index);
			break;
		}
	}

	numTargets = static_cast<uint32_t>(targets_.size());
	pose_ = skeleton_.GetRestPose();
	time_ = clip_->Sample(pose_, time_ + deltaSeconds);

	for (uint32_t index = 0; index < numTargets; ++index)
	{
		CrossFadeTarget& target = targets_[index];
		target.time = target.clip->Sample(target.pose, target.time + deltaSeconds);
		target.elapsed += deltaSeconds;

		float t = target.elapsed / target.duration;
		if (t > 1.0f)
		{
			t = 1.0f;
		}

		Pose::Blend(pose_, pose_, target.pose, t, -1);
	}
}