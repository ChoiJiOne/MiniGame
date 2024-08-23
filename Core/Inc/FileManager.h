#pragma once

#include <cstdint>
#include <string>
#include <vector>

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

	const char* GetWorkingDirectoryA() const { return &workingDirectoryA_[0]; }
	const wchar_t* GetWorkingDirectoryW() const { return &workingDirectoryW_[0]; }

	std::vector<uint8_t> ReadFile(const std::string& path);
	std::vector<uint8_t> ReadFile(const std::wstring& path);

private:
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(FileManager);

private:
	static const uint32_t MAX_STRING_SIZE = 1024;

	char workingDirectoryA_[MAX_STRING_SIZE];
	wchar_t workingDirectoryW_[MAX_STRING_SIZE];

	char stringBufferA_[MAX_STRING_SIZE];
	wchar_t stringBufferW_[MAX_STRING_SIZE];
};

}