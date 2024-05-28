#include "FileModule.h"

#include <shlwapi.h>

char FileModule::lastErrorMessage[MAX_BUFFER_SIZE];

const char* FileModule::GetLastErrorMessage()
{
	return lastErrorMessage;
}

FileModule::Errors FileModule::ReadFile(const std::string& path, std::vector<uint8_t>& outBuffer)
{
	HANDLE file = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (file == INVALID_HANDLE_VALUE)
	{
		SetLastErrorMessage();
		return Errors::ERR_OPEN;
	}

	DWORD fileSize = GetFileSize(file, nullptr);
	outBuffer.resize(fileSize);

	DWORD bytesRead;
	if (!::ReadFile(file, outBuffer.data(), fileSize, &bytesRead, nullptr))
	{
		SetLastErrorMessage();
		return Errors::ERR_READ;
	}

	if (!CloseHandle(file))
	{
		SetLastErrorMessage();
		return Errors::ERR_CLOSE;
	}

	return Errors::OK;
}

FileModule::Errors FileModule::ReadFile(const std::wstring& path, std::vector<uint8_t>& outBuffer)
{
	HANDLE file = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (file == INVALID_HANDLE_VALUE)
	{
		SetLastErrorMessage();
		return Errors::ERR_OPEN;
	}

	DWORD fileSize = GetFileSize(file, nullptr);
	outBuffer.resize(fileSize);

	DWORD bytesRead;
	if (!::ReadFile(file, outBuffer.data(), fileSize, &bytesRead, nullptr))
	{
		SetLastErrorMessage();
		return Errors::ERR_READ;
	}

	if (!CloseHandle(file))
	{
		SetLastErrorMessage();
		return Errors::ERR_CLOSE;
	}

	return Errors::OK;
}

FileModule::Errors FileModule::WriteFile(const std::string& path, const std::vector<uint8_t>& buffer)
{
	HANDLE file = CreateFileA(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file == INVALID_HANDLE_VALUE)
	{
		SetLastErrorMessage();
		return Errors::ERR_CREATE;
	}

	DWORD writeByteSize = 0;
	if (!::WriteFile(file, buffer.data(), static_cast<DWORD>(buffer.size()), &writeByteSize, nullptr))
	{
		SetLastErrorMessage();
		return Errors::ERR_WRITE;
	}

	if (!CloseHandle(file))
	{
		SetLastErrorMessage();
		return Errors::ERR_CLOSE;
	}

	return Errors::OK;
}

FileModule::Errors FileModule::WriteFile(const std::wstring& path, const std::vector<uint8_t>& buffer)
{
	HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file == INVALID_HANDLE_VALUE)
	{
		SetLastErrorMessage();
		return Errors::ERR_CREATE;
	}

	DWORD writeByteSize = 0;
	if (!::WriteFile(file, buffer.data(), static_cast<DWORD>(buffer.size()), &writeByteSize, nullptr))
	{
		SetLastErrorMessage();
		return Errors::ERR_WRITE;
	}

	if (!CloseHandle(file))
	{
		SetLastErrorMessage();
		return Errors::ERR_CLOSE;
	}

	return Errors::OK;
}

FileModule::Errors FileModule::MakeDirectory(const std::string& path)
{
	if (!::CreateDirectoryA(path.c_str(), nullptr))
	{
		SetLastErrorMessage();
		return Errors::ERR_API;
	}

	return Errors::OK;
}

FileModule::Errors FileModule::MakeDirectory(const std::wstring& path)
{
	if (!::CreateDirectoryW(path.c_str(), nullptr))
	{
		SetLastErrorMessage();
		return Errors::ERR_API;
	}

	return Errors::OK;
}

bool FileModule::IsValidPath(const std::string& path)
{
	return PathFileExistsA(path.c_str());
}

bool FileModule::IsValidPath(const std::wstring& path)
{
	return PathFileExistsW(path.c_str());
}

std::string FileModule::GetBasePath(const std::string& path)
{
	std::size_t lastSlash;

	if ((lastSlash = path.rfind('/')) != std::string::npos)
	{
		return path.substr(0, lastSlash + 1);
	}
	else if ((lastSlash = path.rfind('\\')) != std::string::npos)
	{
		return path.substr(0, lastSlash + 1);
	}
	else
	{
		return "";
	}
}

std::wstring FileModule::GetBasePath(const std::wstring& path)
{
	std::size_t lastSlash;

	if ((lastSlash = path.rfind(L'/')) != std::wstring::npos)
	{
		return path.substr(0, lastSlash + 1);
	}
	else if ((lastSlash = path.rfind(L'\\')) != std::wstring::npos)
	{
		return path.substr(0, lastSlash + 1);
	}
	else
	{
		return L"";
	}
}

std::string FileModule::RemoveBasePath(const std::string& path)
{
	std::size_t lastSlash;

	if ((lastSlash = path.rfind('/')) != std::string::npos)
	{
		return path.substr(lastSlash + 1, std::string::npos);
	}
	else if ((lastSlash = path.rfind('\\')) != std::string::npos)
	{
		return path.substr(lastSlash + 1, std::string::npos);
	}
	else
	{
		return path;
	}
}

std::wstring FileModule::RemoveBasePath(const std::wstring& path)
{
	std::size_t lastSlash;

	if ((lastSlash = path.rfind(L'/')) != std::string::npos)
	{
		return path.substr(lastSlash + 1, std::string::npos);
	}
	else if ((lastSlash = path.rfind(L'\\')) != std::string::npos)
	{
		return path.substr(lastSlash + 1, std::string::npos);
	}
	else
	{
		return path;
	}
}

std::string FileModule::GetFileExtension(const std::string& path)
{
	std::string filename = RemoveBasePath(path);
	std::size_t offset = filename.rfind('.');

	return (offset == std::string::npos) ? "" : filename.substr(offset + 1);
}

std::wstring FileModule::GetFileExtension(const std::wstring& path)
{
	std::wstring filename = RemoveBasePath(path);
	std::size_t offset = filename.rfind('.');

	return (offset == std::wstring::npos) ? L"" : filename.substr(offset + 1);
}

void FileModule::SetLastErrorMessage()
{
	DWORD errorCode = static_cast<DWORD>(GetLastError());

	// https://learn.microsoft.com/ko-kr/windows/win32/api/winbase/nf-winbase-formatmessagea
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lastErrorMessage, MAX_BUFFER_SIZE, nullptr);
}