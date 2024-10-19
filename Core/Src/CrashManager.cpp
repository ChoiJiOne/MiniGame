#include <windows.h>
#include <dbghelp.h>
#include <pathcch.h>
#include <Shlwapi.h>

#include "Assertion.h"
#include "CrashManager.h"
#include "GameTimer.h"
#include "GameUtils.h"

CrashManager CrashManager::instance_;

static std::function<void()> crashCallbackEvent_ = nullptr;
static std::wstring crashDumpFile_;

std::wstring GetCrashDumpPath()
{
	static wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(nullptr, exePath, MAX_PATH);

	std::wstring minidumpPath(exePath);
	std::wstring basePath = GameUtils::GetBasePath(minidumpPath);

	return GameUtils::PrintF(L"%s\\Crash\\", basePath.c_str());
}

LONG WINAPI DetectApplicationCrash(EXCEPTION_POINTERS* ep)
{
	std::wstring currSystemTime = GameTimer::GetCurrentSystemTimeAsWString();
	std::wstring crashDumpPath = GetCrashDumpPath();

	if (!GameUtils::IsValidPath(crashDumpPath))
	{
		GameUtils::MakeDirectory(crashDumpPath);
	}

	crashDumpFile_ = GameUtils::PrintF(L"%sWindows-%s-Minidump.dmp", crashDumpPath.c_str(), currSystemTime.c_str());
	HANDLE file = ::CreateFileW(crashDumpFile_.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	_MINIDUMP_EXCEPTION_INFORMATION exception;
	exception.ThreadId = ::GetCurrentThreadId();
	exception.ExceptionPointers = ep;
	exception.ClientPointers = FALSE;

	HANDLE process = ::GetCurrentProcess();
	DWORD processID = ::GetCurrentProcessId();
	::MiniDumpWriteDump(process, processID, file, MiniDumpWithFullMemory, &exception, nullptr, nullptr);

	::CloseHandle(file);

	if (crashCallbackEvent_)
	{
		crashCallbackEvent_();
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

CrashManager& CrashManager::GetRef()
{
	return instance_;
}

CrashManager* CrashManager::GetPtr()
{
	return &instance_;
}

void CrashManager::SetCrashCallbackEvent(const std::function<void()>& callback)
{
	crashCallbackEvent_ = callback;
}

const wchar_t* CrashManager::GetCrashDumpFileNamePtr()
{
	return crashDumpFile_.c_str();
}

const std::wstring& CrashManager::GetCrashDumpFileName()
{
	return crashDumpFile_;
}