#include <algorithm>
#include <Shlwapi.h>
#include <windows.h>

#include "Assertion.h"
#include "GameUtils.h"

static const uint32_t MAX_STRING_BUFFER_SIZE = 1024;
static char charBuffer_[MAX_STRING_BUFFER_SIZE];
static wchar_t wcharBuffer_[MAX_STRING_BUFFER_SIZE];

std::string GameUtils::PrintF(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int32_t size = vsnprintf(charBuffer_, MAX_STRING_BUFFER_SIZE, format, args);
	va_end(args);

	return std::string(charBuffer_, size);
}

std::wstring GameUtils::PrintF(const wchar_t* format, ...)
{
	va_list args;
	va_start(args, format);
	int32_t size = _vsnwprintf_s(wcharBuffer_, MAX_STRING_BUFFER_SIZE, format, args);
	va_end(args);

	return std::wstring(wcharBuffer_, size);
}

std::vector<std::string> GameUtils::Split(const std::string& text, const std::string& delimiter)
{
	std::vector<std::string> tokens;
	std::size_t position = 0ULL;
	std::string remain = text;

	while ((position = remain.find(delimiter)) != std::string::npos)
	{
		tokens.push_back(remain.substr(0, position));
		remain.erase(0, position + delimiter.length());
	}

	tokens.push_back(remain);
	return tokens;
}

std::vector<std::wstring> GameUtils::Split(const std::wstring& text, const std::wstring& delimiter)
{
	std::vector<std::wstring> tokens;
	std::size_t position = 0ULL;
	std::wstring remain = text;

	while ((position = remain.find(delimiter)) != std::string::npos)
	{
		tokens.push_back(remain.substr(0, position));
		remain.erase(0, position + delimiter.length());
	}

	tokens.push_back(remain);
	return tokens;
}

std::wstring GameUtils::Convert(const std::string& text)
{
	std::size_t size = std::mbstowcs(wcharBuffer_, text.c_str(), MAX_STRING_BUFFER_SIZE);
	return std::wstring(wcharBuffer_, size);
}

std::string GameUtils::Convert(const std::wstring& text)
{
	std::size_t size = std::wcstombs(charBuffer_, text.c_str(), MAX_STRING_BUFFER_SIZE);
	return std::string(charBuffer_, size);
}

std::string GameUtils::ToLower(const std::string& text)
{
	std::string lower = text;
	std::transform(lower.begin(), lower.end(), lower.begin(), std::tolower);
	return lower;
}

std::wstring GameUtils::ToLower(const std::wstring& text)
{
	std::wstring lower = text;
	std::transform(lower.begin(), lower.end(), lower.begin(), std::tolower);
	return lower;
}

std::string GameUtils::ToUpper(const std::string& text)
{
	std::string upper = text;
	std::transform(upper.begin(), upper.end(), upper.begin(), std::toupper);
	return upper;
}

std::wstring GameUtils::ToUpper(const std::wstring& text)
{
	std::wstring upper = text;
	std::transform(upper.begin(), upper.end(), upper.begin(), std::toupper);
	return upper;
}

bool GameUtils::ToInt(const std::string& integer, int32_t& outInteger)
{
	const char* strPtr = integer.c_str();
	char* endPtr = nullptr;
	const int32_t radix = 10;

	const long ans = std::strtol(strPtr, &endPtr, radix);
	if (strPtr == endPtr || ans < INT_MIN || ans > INT_MAX)
	{
		return false;
	}

	outInteger = static_cast<int32_t>(ans);
	return true;
}

bool GameUtils::ToInt(const std::wstring& integer, int32_t& outInteger)
{
	const wchar_t* strPtr = integer.c_str();
	wchar_t* endPtr = nullptr;
	const int32_t radix = 10;

	const long ans = std::wcstol(strPtr, &endPtr, radix);
	if (strPtr == endPtr || ans < INT_MIN || ans > INT_MAX)
	{
		return false;
	}

	outInteger = static_cast<int32_t>(ans);
	return true;
}

bool GameUtils::ToFloat(const std::string& floating, float& outFloating)
{
	const char* strPtr = floating.c_str();
	char* endPtr = nullptr;

	const float ans = std::strtof(strPtr, &endPtr);
	if (strPtr == endPtr)
	{
		return false;
	}

	outFloating = ans;
	return true;
}

bool GameUtils::ToFloat(const std::wstring& floating, float& outFloating)
{
	const wchar_t* strPtr = floating.c_str();
	wchar_t* endPtr = nullptr;

	const float ans = std::wcstof(strPtr, &endPtr);
	if (strPtr == endPtr)
	{
		return false;
	}

	outFloating = ans;
	return true;
}

std::vector<uint8_t> GameUtils::ReadFile(const std::string& path)
{
	HANDLE file = ::CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	ASSERT(file != INVALID_HANDLE_VALUE, "%s", GetWinErrorMessage());

	DWORD fileSize = ::GetFileSize(file, nullptr);
	std::vector<uint8_t> buffer(fileSize);

	DWORD bytesRead;
	ASSERT(::ReadFile(file, buffer.data(), fileSize, &bytesRead, nullptr), "%s", GetWinErrorMessage());
	ASSERT(::CloseHandle(file), "%s", GetWinErrorMessage());

	return buffer;
}

std::vector<uint8_t> GameUtils::ReadFile(const std::wstring& path)
{
	HANDLE file = ::CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	ASSERT(file != INVALID_HANDLE_VALUE, "%s", GetWinErrorMessage());

	DWORD fileSize = ::GetFileSize(file, nullptr);
	std::vector<uint8_t> buffer(fileSize);

	DWORD bytesRead;
	ASSERT(::ReadFile(file, buffer.data(), fileSize, &bytesRead, nullptr), "%s", GetWinErrorMessage());
	ASSERT(::CloseHandle(file), "%s", GetWinErrorMessage());;

	return buffer;
}

void GameUtils::WriteFile(const std::string& path, const std::vector<uint8_t>& buffer)
{
	HANDLE file = ::CreateFileA(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	ASSERT(file != INVALID_HANDLE_VALUE, GetWinErrorMessage());

	DWORD writeByteSize = 0;
	ASSERT(::WriteFile(file, buffer.data(), static_cast<DWORD>(buffer.size()), &writeByteSize, nullptr), GetWinErrorMessage());
	ASSERT(::CloseHandle(file), "failed to close %s", GetWinErrorMessage());
}

void GameUtils::WriteFile(const std::wstring& path, const std::vector<uint8_t>& buffer)
{
	HANDLE file = ::CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	ASSERT(file != INVALID_HANDLE_VALUE, GetWinErrorMessage());

	DWORD writeByteSize = 0;
	ASSERT(::WriteFile(file, buffer.data(), static_cast<DWORD>(buffer.size()), &writeByteSize, nullptr), GetWinErrorMessage());
	ASSERT(::CloseHandle(file), "failed to close %s", GetWinErrorMessage());
}

void GameUtils::MakeDirectory(const std::string& path)
{
	ASSERT(CreateDirectoryA(path.c_str(), nullptr), GetWinErrorMessage());
}

void GameUtils::MakeDirectory(const std::wstring& path)
{
	ASSERT(CreateDirectoryW(path.c_str(), nullptr), GetWinErrorMessage());
}

bool GameUtils::IsValidPath(const std::string& path)
{
	return PathFileExistsA(path.c_str());
}

bool GameUtils::IsValidPath(const std::wstring& path)
{
	return PathFileExistsW(path.c_str());
}

std::string GameUtils::GetBasePath(const std::string& path)
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

std::wstring GameUtils::GetBasePath(const std::wstring& path)
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

std::string GameUtils::RemoveBasePath(const std::string& path)
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

std::wstring GameUtils::RemoveBasePath(const std::wstring& path)
{
	std::size_t lastSlash;

	if ((lastSlash = path.rfind(L'/')) != std::wstring::npos)
	{
		return path.substr(lastSlash + 1, std::wstring::npos);
	}
	else if ((lastSlash = path.rfind(L'\\')) != std::wstring::npos)
	{
		return path.substr(lastSlash + 1, std::wstring::npos);
	}
	else
	{
		return path;
	}
}

std::string GameUtils::GetFileExtension(const std::string& path)
{
	std::string filename = RemoveBasePath(path);
	std::size_t offset = filename.rfind('.');

	return (offset == std::string::npos) ? "" : filename.substr(offset + 1);
}

std::wstring GameUtils::GetFileExtension(const std::wstring& path)
{
	std::wstring filename = RemoveBasePath(path);
	std::size_t offset = filename.rfind('.');

	return (offset == std::wstring::npos) ? L"" : filename.substr(offset + 1);
}