#pragma warning(push)
#pragma warning(disable: 26812)

#include <algorithm>
#include <windows.h>

#include <miniaudio.h>

#include "Assertion.h"

#include "AudioModule.h"

bool AudioModule::bIsInitialize;
std::unique_ptr<ma_engine> AudioModule::engine = nullptr;
uint32_t AudioModule::cacheSize = 0;
std::array<std::unique_ptr<ma_sound>, AudioModule::MAX_SOUND_RESOURCE_SIZE> AudioModule::cache;

AudioModule::Errors AudioModule::Init()
{
	if (bIsInitialize)
	{
		return Errors::FAILED;
	}

	engine = std::make_unique<ma_engine>();
	ma_result result = ma_engine_init(nullptr, engine.get());
	if (result != MA_SUCCESS)
	{
		return static_cast<Errors>(result);
	}

	bIsInitialize = true;
	return Errors::OK;
}

void AudioModule::Uninit()
{
	for (std::size_t index = 0; index < cacheSize; ++index)
	{
		if (cache[index])
		{
			ma_sound* sound = cache[index].get();
			ma_sound_uninit(sound);

			cache[index].reset();
		}
	}

	if (engine)
	{
		ma_engine_uninit(engine.get());
		engine.reset();
	}
}

SoundID AudioModule::CreateSound(const std::string& path)
{
	CHECK(0 <= cacheSize && cacheSize < MAX_SOUND_RESOURCE_SIZE);

	SoundID soundID = -1;
	for (int32_t index = 0; index < cacheSize; ++index)
	{
		if (!cache[index])
		{
			soundID = static_cast<SoundID>(index);
			break;
		}
	}

	if (soundID == -1)
	{
		soundID = static_cast<SoundID>(cacheSize++);
	}

	cache[soundID] = std::make_unique<ma_sound>();
	ma_result result = ma_sound_init_from_file(engine.get(), path.c_str(), 0, nullptr, nullptr, cache[soundID].get());
	CHECK(result == MA_SUCCESS);

	return soundID;
}

void AudioModule::DestroySound(const SoundID& soundID)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);

	if (cache[soundID])
	{
		ma_sound* sound = cache[soundID].get();
		ma_sound_uninit(sound);

		cache[soundID].reset();
	}
}

void AudioModule::SetSoundVolume(const SoundID& soundID, float volume)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);
	ma_sound* sound = cache[soundID].get();

	volume = std::clamp<float>(volume, 0.0f, 1.0f);
	ma_sound_set_volume(sound, volume);
}

float AudioModule::GetSoundVolume(const SoundID& soundID)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);
	ma_sound* sound = cache[soundID].get();

	return ma_sound_get_volume(sound);
}

void AudioModule::SetSoundLooping(const SoundID& soundID, bool bIsLoop)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);
	ma_sound* sound = cache[soundID].get();

	ma_sound_set_looping(sound, static_cast<ma_bool32>(bIsLoop));
}

bool AudioModule::IsSoundLooping(const SoundID& soundID)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);
	ma_sound* sound = cache[soundID].get();

	return ma_sound_is_looping(sound) == MA_TRUE;
}

void AudioModule::PlaySound(const SoundID& soundID)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);
	ma_sound* sound = cache[soundID].get();

	ma_sound_start(sound);
}

bool AudioModule::IsSoundPlaying(const SoundID& soundID)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);
	ma_sound* sound = cache[soundID].get();

	return ma_sound_is_playing(sound) == MA_TRUE;
}

bool AudioModule::IsDoneSound(const SoundID& soundID)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);
	ma_sound* sound = cache[soundID].get();

	return ma_sound_at_end(sound) == MA_TRUE;
}

void AudioModule::StopSound(const SoundID& soundID)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);
	ma_sound* sound = cache[soundID].get();

	ma_sound_stop(sound);
}

bool AudioModule::IsSoundStopping(const SoundID& soundID)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);
	ma_sound* sound = cache[soundID].get();

	return ma_sound_at_end(sound) != MA_TRUE && ma_sound_is_playing(sound) != MA_TRUE;
}

void AudioModule::ResetSound(const SoundID& soundID)
{
	CHECK(0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE);
	ma_sound* sound = cache[soundID].get();

	ma_sound_seek_to_pcm_frame(sound, 0);
}

#pragma warning(pop)