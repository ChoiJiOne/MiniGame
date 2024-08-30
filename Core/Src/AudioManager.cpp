#include <mimalloc.h>
#include <miniaudio.h>

#include "Assertion.h"
#include "AudioManager.h"

std::unique_ptr<ma_engine> audioEngine_ = nullptr;

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
	audioEngine_ = std::make_unique<ma_engine>();

	ma_allocation_callbacks allocationCallbacks;
	allocationCallbacks.pUserData = nullptr;
	allocationCallbacks.onMalloc = AudioMalloc;
	allocationCallbacks.onRealloc = AudioRealloc;
	allocationCallbacks.onFree = AudioFree;

	ma_engine_config config;
	config.allocationCallbacks = allocationCallbacks;

	ma_result result = ma_engine_init(&config, audioEngine_.get());
	ASSERT(result == MA_SUCCESS, "Failed to initialize miniaudio engine.");
}

void AudioManager::Shutdown()
{
	ma_engine_uninit(audioEngine_.get());
	audioEngine_.reset();
}