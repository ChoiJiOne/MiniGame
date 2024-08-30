#include <miniaudio.h>

#include "Assertion.h"
#include "AudioManager.h"
#include "GameMath.h"
#include "Sound.h"

Sound::Sound(const std::string& path)
{
	resourcePtr_ = AudioManager::Get().CreateSound(path);
	ASSERT(resourcePtr_ != nullptr, "Failed to load %s sound resource.", path.c_str());

	bIsInitialized_ = true;
}

Sound::~Sound()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Sound::Release()
{
	CHECK(bIsInitialized_);

	AudioManager::Get().DestroySound(resourcePtr_);

	bIsInitialized_ = false;
}

void Sound::SetVolume(float volume)
{
	volume = GameMath::Clamp<float>(volume, 0.0f, 1.0f);
	ma_sound_set_volume(reinterpret_cast<ma_sound*>(resourcePtr_), volume);
}

float Sound::GetVolume()
{
	return ma_sound_get_volume(reinterpret_cast<ma_sound*>(resourcePtr_));
}

void Sound::SetLooping(bool bIsLoop)
{
	ma_sound_set_looping(reinterpret_cast<ma_sound*>(resourcePtr_), static_cast<ma_bool32>(bIsLoop));
}

bool Sound::IsLooping()
{
	return ma_sound_is_looping(reinterpret_cast<ma_sound*>(resourcePtr_)) == MA_TRUE;
}

void Sound::Play()
{
	ma_sound_start(reinterpret_cast<ma_sound*>(resourcePtr_));
}

bool Sound::IsPlaying()
{
	return ma_sound_is_playing(reinterpret_cast<ma_sound*>(resourcePtr_)) == MA_TRUE;
}

bool Sound::IsDone()
{

	return ma_sound_at_end(reinterpret_cast<ma_sound*>(resourcePtr_)) == MA_TRUE;
}

void Sound::Stop()
{
	ma_sound_stop(reinterpret_cast<ma_sound*>(resourcePtr_));
}

bool Sound::IsStopping()
{
	return ma_sound_at_end(reinterpret_cast<ma_sound*>(resourcePtr_)) != MA_TRUE && ma_sound_is_playing(reinterpret_cast<ma_sound*>(resourcePtr_)) != MA_TRUE;
}

void Sound::Reset()
{
	ma_sound_seek_to_pcm_frame(reinterpret_cast<ma_sound*>(resourcePtr_), 0);
}