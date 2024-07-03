#include "Assertion.h"
#include "AudioManager.h"
#include "GameMath.h"
#include "Sound.h"

using namespace GameMaker;

Sound::Sound(const std::string& path)
{
	static ma_engine* engine = nullptr;
	if (!engine)
	{
		engine = AudioManager::Get().GetEngine();
	}

	resource_ = std::make_unique<ma_sound>();
	ma_result result = ma_sound_init_from_file(engine, path.c_str(), 0, nullptr, nullptr, resource_.get());
	CHECK(result == MA_SUCCESS);

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

	ma_sound* sound = resource_.get();
	ma_sound_uninit(sound);

	resource_.reset();
	bIsInitialized_ = false;
}

void Sound::SetVolume(float volume)
{
	volume = Clamp<float>(volume, 0.0f, 1.0f);
	ma_sound_set_volume(resource_.get(), volume);
}

float Sound::GetVolume()
{
	return ma_sound_get_volume(resource_.get());
}

void Sound::SetLooping(bool bIsLoop)
{
	ma_sound_set_looping(resource_.get(), static_cast<ma_bool32>(bIsLoop));
}

bool Sound::IsLooping()
{
	return ma_sound_is_looping(resource_.get()) == MA_TRUE;
}

void Sound::Play()
{
	ma_sound_start(resource_.get());
}

bool Sound::IsPlaying()
{
	return ma_sound_is_playing(resource_.get()) == MA_TRUE;
}

bool Sound::IsDone()
{

	return ma_sound_at_end(resource_.get()) == MA_TRUE;
}

void Sound::Stop()
{
	ma_sound_stop(resource_.get());
}

bool Sound::IsStopping()
{
	return ma_sound_at_end(resource_.get()) != MA_TRUE && ma_sound_is_playing(resource_.get()) != MA_TRUE;
}

void Sound::Reset()
{
	ma_sound_seek_to_pcm_frame(resource_.get(), 0);
}