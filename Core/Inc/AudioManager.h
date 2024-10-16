#pragma once

#include <array>
#include <memory>
#include <string>

#include "Macro.h"

/** 오디오 매니저는 싱글턴입니다. */
class AudioManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(AudioManager);

	static AudioManager& GetRef();
	static AudioManager* GetPtr();

private:
	friend class IApp;
	friend class Sound;

	AudioManager() = default;
	virtual ~AudioManager() {}

	void Startup(); /** IApp 내부에서만 호출해야 합니다. */
	void Shutdown(); /** IApp 내부에서만 호출해야 합니다. */

	void* CreateSound(const std::string& path);
	void DestroySound(void* sound);

private:
	static AudioManager instance_;
};