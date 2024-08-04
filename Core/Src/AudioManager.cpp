#pragma warning(push)
#pragma warning(disable: 26812)

#include "Assertion.h"
#include "AudioManager.h"

using namespace GameMaker;

AudioManager& AudioManager::Get()
{
	static AudioManager instance;
	return instance;
}

void AudioManager::Startup()
{
	CHECK(!bIsStartup_);

	engine_ = std::make_unique<ma_engine>();
	ma_result result = ma_engine_init(nullptr, engine_.get());
	ASSERT(result == MA_SUCCESS, "Failed to initialize audio engine.");

	bIsStartup_ = true;
}

void AudioManager::Shutdown()
{
	CHECK(bIsStartup_);

	if (engine_)
	{
		ma_engine_uninit(engine_.get());
		engine_.reset();
	}

	bIsStartup_ = false;
}

#pragma warning(pop)