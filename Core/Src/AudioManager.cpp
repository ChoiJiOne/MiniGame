#include <array>

#include <mimalloc.h>
#include <miniaudio.h>

#include "Assertion.h"
#include "AudioManager.h"

std::unique_ptr<ma_engine> audioEngine_ = nullptr;
ma_engine* audioEnginePtr_ = nullptr;

static const uint32_t MAX_SOUND_RESOURCE_SIZE = 30;
static int32_t size_ = 0;
static std::array<ma_sound, MAX_SOUND_RESOURCE_SIZE> sounds_;
static std::array<bool, MAX_SOUND_RESOURCE_SIZE> usage_;

AudioManager& AudioManager::Get()
{
	static AudioManager instance;
	return instance;
}

void* AudioMalloc(size_t sz, void* pUserData)
{
	(void)(pUserData); /** 무시. 사용하지 않음 */
	return mi_malloc(sz);
}

void* AudioRealloc(void* p, size_t sz, void* pUserData)
{
	(void)(pUserData); /** 무시. 사용하지 않음 */
	return mi_realloc(p, sz);
}

void AudioFree(void* p, void* pUserData)
{
	(void)(pUserData); /** 무시. 사용하지 않음 */
	return mi_free(p);
}

void AudioManager::Startup()
{
	size_ = 0;

	audioEngine_ = std::make_unique<ma_engine>();
	audioEnginePtr_ = audioEngine_.get();

	ma_allocation_callbacks allocationCallbacks;
	allocationCallbacks.pUserData = nullptr;
	allocationCallbacks.onMalloc = AudioMalloc;
	allocationCallbacks.onRealloc = AudioRealloc;
	allocationCallbacks.onFree = AudioFree;

	ma_engine_config config;
	ZeroMemory(&config, sizeof(ma_engine_config));

	config.allocationCallbacks = allocationCallbacks;

	ma_result result = ma_engine_init(&config, audioEnginePtr_);
	ASSERT(result == MA_SUCCESS, "Failed to initialize miniaudio engine.");
}

void AudioManager::Shutdown()
{
	for (uint32_t index = 0; index < size_; ++index)
	{
		if (usage_[index])
		{
			ma_sound_uninit(&sounds_[index]);
			usage_[index] = false;
		}
	}

	ma_engine_uninit(audioEngine_.get());
	audioEngine_.reset();

	size_ = 0;
}

void* AudioManager::CreateSound(const std::string& path)
{
	if (!(0 <= size_ && size_ < MAX_SOUND_RESOURCE_SIZE))
	{
		return nullptr;
	}

	int32_t soundID = -1;
	for (uint32_t index = 0; index < size_; ++index)
	{
		if (!usage_[index])
		{
			soundID = static_cast<int32_t>(index);
			break;
		}
	}

	if (soundID == -1)
	{
		soundID = size_++;
	}

	usage_[soundID] = true;

	ma_sound* sound = &sounds_[soundID];
	ma_result result = ma_sound_init_from_file(audioEnginePtr_, path.c_str(), 0, nullptr, nullptr, sound);
	
	if (result != MA_SUCCESS)
	{
		usage_[soundID] = false;
		sound = nullptr;
	}
	
	return reinterpret_cast<void*>(sound);
}

void AudioManager::DestroySound(void* sound)
{
	ASSERT(sound != nullptr, "Invalid sound resource pointer.");

	int32_t soundID = -1;
	for (uint32_t index = 0; index < size_; ++index)
	{
		if (sound == &sounds_[index])
		{
			soundID = static_cast<int32_t>(index);
			break;
		}
	}

	if ((0 <= soundID && soundID < MAX_SOUND_RESOURCE_SIZE) && usage_[soundID])
	{
		ma_sound_uninit(&sounds_[soundID]);
		usage_[soundID] = false;
	}
}
