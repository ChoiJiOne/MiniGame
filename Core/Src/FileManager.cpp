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

std::vector<uint8_t> FileManager::ReadFile(const std::string& path)
{
	HANDLE file = ::CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	WINDOWS_ASSERT(file != INVALID_HANDLE_VALUE, "failed to open %s", path.c_str());

	DWORD fileSize = ::GetFileSize(file, nullptr);
	std::vector<uint8_t> buffer(fileSize);

	DWORD bytesRead;
	WINDOWS_ASSERT(::ReadFile(file, buffer.data(), fileSize, &bytesRead, nullptr), "failed to read %s", path.c_str());
	WINDOWS_ASSERT(::CloseHandle(file), "failed to close %s", path.c_str());

	return buffer;
}

std::vector<uint8_t> FileManager::ReadFile(const std::wstring& path)
{
	HANDLE file = ::CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	WINDOWS_ASSERT(file != INVALID_HANDLE_VALUE, L"failed to open %s", path.c_str());

	DWORD fileSize = ::GetFileSize(file, nullptr);
	std::vector<uint8_t> buffer(fileSize);

	DWORD bytesRead;
	WINDOWS_ASSERT(::ReadFile(file, buffer.data(), fileSize, &bytesRead, nullptr), L"failed to read %s", path.c_str());
	WINDOWS_ASSERT(::CloseHandle(file), L"failed to close %s", path.c_str());

	return buffer;
}

void FileManager::WriteFile(const std::string& path, const std::vector<uint8_t>& buffer)
{
	HANDLE file = ::CreateFileA(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	WINDOWS_ASSERT(file != INVALID_HANDLE_VALUE, "failed to create %s", path.c_str());

	DWORD writeByteSize = 0;
	WINDOWS_ASSERT(::WriteFile(file, buffer.data(), static_cast<DWORD>(buffer.size()), &writeByteSize, nullptr), "failed to write %s", path.c_str());
	WINDOWS_ASSERT(::CloseHandle(file), "failed to close %s", path.c_str());
}

void FileManager::WriteFile(const std::wstring& path, const std::vector<uint8_t>& buffer)
{
	HANDLE file = ::CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	WINDOWS_ASSERT(file != INVALID_HANDLE_VALUE, L"failed to create %s", path.c_str());

	DWORD writeByteSize = 0;
	WINDOWS_ASSERT(::WriteFile(file, buffer.data(), static_cast<DWORD>(buffer.size()), &writeByteSize, nullptr), L"failed to write %s", path.c_str());
	WINDOWS_ASSERT(::CloseHandle(file), L"failed to close %s", path.c_str());
}

void FileManager::MakeDirectory(const std::string& path)
{
	WINDOWS_ASSERT(CreateDirectoryA(path.c_str(), nullptr), "failed to create %s", path.c_str());
}

void FileManager::MakeDirectory(const std::wstring& path)
{
	WINDOWS_ASSERT(CreateDirectoryW(path.c_str(), nullptr), L"failed to create %s", path.c_str());
}

bool FileManager::IsValidPath(const std::string& path)
{
	return PathFileExistsA(path.c_str());
}

bool FileManager::IsValidPath(const std::wstring& path)
{
	return PathFileExistsW(path.c_str());
}
