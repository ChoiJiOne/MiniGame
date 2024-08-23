#pragma once

#include "IManager.h"

namespace GameMaker
{
class FileManager : public IManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(FileManager);

	static FileManager& Get();

	virtual void Startup() override;
	virtual void Shutdown() override;

private:
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(FileManager);
};

}