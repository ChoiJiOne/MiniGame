#include <Shlwapi.h>

#include "Assertion.h"
#include "FileManager.h"

using namespace GameMaker;

FileManager& FileManager::Get()
{
	static FileManager instance;
	return instance;
}

void FileManager::Startup()
{
	CHECK(!bIsStartup_);

	WINDOWS_CHECK(GetCurrentDirectoryA(MAX_STRING_SIZE, workingDirectoryA_));
	WINDOWS_CHECK(GetCurrentDirectoryW(MAX_STRING_SIZE, workingDirectoryW_));

	std::strncat(workingDirectoryA_, "\\", 1);
	std::wcsncat(workingDirectoryW_, L"\\", 1);
	
	bIsStartup_ = true;
}

void FileManager::Shutdown()
{
	CHECK(bIsStartup_);

	bIsStartup_ = false;
}
