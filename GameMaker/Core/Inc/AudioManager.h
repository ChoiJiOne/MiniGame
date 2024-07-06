#pragma once

#include <array>
#include <memory>
#include <string>

#include <miniaudio.h>

#include "IManager.h"


namespace GameMaker
{
class AudioManager : public IManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(AudioManager);
	
	static AudioManager& Get();

	virtual void Startup() override;
	virtual void Shutdown() override;

	ma_engine* GetEngine() { return engine_.get(); }
	
private:
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(AudioManager);
	
private:
	std::unique_ptr<ma_engine> engine_ = nullptr;
};

}