#include <windows.h>
#include <dbghelp.h>
#include <pathcch.h>
#include <Shlwapi.h>

#include "CrashUtils.h"
#include "FileUtils.h"
#include "StringUtils.h"

using namespace GameMaker;

LONG WINAPI CrashUtils::DetectApplicationCrash(EXCEPTION_POINTERS* ep)
{
	std::wstring systemTime = GetCurrentSystemTimeAsString();
	std::wstring minidumpPath = GetMinidumpPath();

	if (!FileUtils::IsValidPath(minidumpPath))
	{
		FileUtils::MakeDirectory(minidumpPath);
	}

	std::wstring path = StringUtils::PrintF(L"%sWindows-%s-Minidump.dmp", minidumpPath.c_str(), systemTime.c_str());
	if (!CreateMinidumpFile(path, ep))
	{
		std::wstring writeErrorPath = StringUtils::PrintF(L"%sWindows-%s-WriteError.txt", minidumpPath.c_str(), systemTime.c_str());
		
		std::string message = "Failed to write minidump file.";
		std::vector<uint8_t> messageBuffer(message.begin(), message.end());

		FileUtils::WriteFile(writeErrorPath, messageBuffer);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

std::wstring CrashUtils::GetCurrentSystemTimeAsString()
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	int32_t year = static_cast<int32_t>(systemTime.wYear);
	int32_t month = static_cast<int32_t>(systemTime.wMonth);
	int32_t day = static_cast<int32_t>(systemTime.wDay);
	int32_t hour = static_cast<int32_t>(systemTime.wHour);
	int32_t minute = static_cast<int32_t>(systemTime.wMinute);
	int32_t second = static_cast<int32_t>(systemTime.wSecond);

	return StringUtils::PrintF(L"%4d-%02d-%02d-%02d-%02d-%02d", year, month, day, hour, minute, second);
}

std::wstring CrashUtils::GetMinidumpPath()
{
	static wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(nullptr, exePath, MAX_PATH);

	std::wstring minidumpPath(exePath);
	std::wstring basePath = FileUtils::GetBasePath(minidumpPath);

	return StringUtils::PrintF(L"%s\\Crash\\", basePath.c_str());
}

// https://learn.microsoft.com/ko-kr/windows/win32/api/winnt/ns-winnt-exception_pointers
// https://learn.microsoft.com/ko-kr/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump
bool CrashUtils::CreateMinidumpFile(const std::wstring& path, EXCEPTION_POINTERS* ep)
{
	HANDLE file = ::CreateFileW(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	_MINIDUMP_EXCEPTION_INFORMATION exception;
	exception.ThreadId = ::GetCurrentThreadId();
	exception.ExceptionPointers = ep;
	exception.ClientPointers = FALSE;

	HANDLE process = ::GetCurrentProcess();
	DWORD processID = ::GetCurrentProcessId();
	bool bResult = ::MiniDumpWriteDump(process, processID, file, MiniDumpWithFullMemory, &exception, nullptr, nullptr);

	::CloseHandle(file);

	return bResult;
}