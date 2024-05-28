#include <windows.h>
#include <dbghelp.h>
#include <pathcch.h>
#include <Shlwapi.h>
#include <string>

#include "CrashModule.h"

bool CrashModule::bIsInitialize = false;
char CrashModule::lastErrorMessage[MAX_BUFFER_SIZE];
wchar_t CrashModule::dumpPath[MAX_BUFFER_SIZE];
LPTOP_LEVEL_EXCEPTION_FILTER CrashModule::topLevelExceptionFilter;
std::function<void()> CrashModule::crashCallback = nullptr;

CrashModule::Errors CrashModule::Init(const std::function<void()>& callback)
{
	if (bIsInitialize)
	{
		SetLastErrorMessage("Already initialize platform module.");
		return Errors::ERR_PREV_INIT;
	}

	topLevelExceptionFilter = SetUnhandledExceptionFilter(DetectApplicationCrash);
	crashCallback = callback;

	std::wstring path;
	Errors result = GetDumpFilePath(path);
	if (result != Errors::OK)
	{
		return result;
	}

	if (!PathFileExistsW(path.c_str()) && !CreateDirectoryW(path.c_str(), nullptr))
	{
		SetLastErrorMessage();
		return Errors::ERR_API;
	}

	if (wcsncpy_s(dumpPath, MAX_BUFFER_SIZE, path.c_str(), path.size()))
	{
		SetLastErrorMessage("Failed to write dump path.");
		return Errors::FAILED;
	}

	bIsInitialize = true;
	return Errors::OK;
}

CrashModule::Errors CrashModule::Uninit()
{
	if (!bIsInitialize)
	{
		SetLastErrorMessage("Haven't initialized or have already been uninitialized.");
		return Errors::ERR_NOT_INIT;
	}

	SetUnhandledExceptionFilter(topLevelExceptionFilter);

	bIsInitialize = false;
	return Errors::OK;
}

const char* CrashModule::GetLastErrorMessage()
{
	return lastErrorMessage;
}

std::wstring CrashModule::PrintF(const wchar_t* format, ...)
{
	static wchar_t buffer[MAX_BUFFER_SIZE];

	va_list args;
	va_start(args, format);
	int32_t size = _vsnwprintf_s(buffer, MAX_BUFFER_SIZE, format, args);
	va_end(args);

	return std::wstring(buffer, size);
}

void CrashModule::SetLastErrorMessage()
{
	DWORD errorCode = static_cast<DWORD>(GetLastError());
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lastErrorMessage, MAX_BUFFER_SIZE, nullptr);
}

void CrashModule::SetLastErrorMessage(const std::string& message)
{
	std::copy(message.begin(), message.end(), lastErrorMessage);
}

std::wstring CrashModule::GetCurrentSystemTimeAsString()
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	int32_t year = static_cast<int32_t>(systemTime.wYear);
	int32_t month = static_cast<int32_t>(systemTime.wMonth);
	int32_t day = static_cast<int32_t>(systemTime.wDay);
	int32_t hour = static_cast<int32_t>(systemTime.wHour);
	int32_t minute = static_cast<int32_t>(systemTime.wMinute);
	int32_t second = static_cast<int32_t>(systemTime.wSecond);

	return PrintF(L"%4d-%02d-%02d-%02d-%02d-%02d", year, month, day, hour, minute, second);
}

CrashModule::Errors CrashModule::WriteTextFile(const std::wstring& path, const std::string& text)
{
	HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file == INVALID_HANDLE_VALUE)
	{
		SetLastErrorMessage();
		return Errors::ERR_CREATE;
	}

	DWORD writeByteSize = 0;
	if (!WriteFile(file, text.data(), static_cast<DWORD>(text.size()), &writeByteSize, nullptr))
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

CrashModule::Errors CrashModule::CreateMinidumpFile(const std::wstring& path, EXCEPTION_POINTERS* exceptionPtr)
{
	HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file == INVALID_HANDLE_VALUE)
	{
		SetLastErrorMessage();
		return Errors::ERR_CREATE;
	}

	_MINIDUMP_EXCEPTION_INFORMATION exception;
	exception.ThreadId = GetCurrentThreadId();
	exception.ExceptionPointers = exceptionPtr;
	exception.ClientPointers = FALSE;

	if (!MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpWithFullMemory, &exception, nullptr, nullptr))
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

CrashModule::Errors CrashModule::GetDumpFilePath(std::wstring& outDumpFilePath)
{
	static wchar_t exePath[MAX_BUFFER_SIZE];
	if (!GetModuleFileNameW(nullptr, exePath, MAX_BUFFER_SIZE))
	{
		SetLastErrorMessage();
		return Errors::ERR_API;
	}

	if (FAILED(PathCchRemoveFileSpec(exePath, MAX_BUFFER_SIZE)))
	{
		SetLastErrorMessage();
		return Errors::ERR_API;
	}

	outDumpFilePath = PrintF(L"%s\\Crash\\", exePath);
	return Errors::OK;
}

LONG WINAPI CrashModule::DetectApplicationCrash(EXCEPTION_POINTERS* exceptionPtr)
{
	std::wstring systemTime = GetCurrentSystemTimeAsString();
	std::wstring minidumpPath = PrintF(L"%sWindows-%s-Minidump.dmp", dumpPath, systemTime.c_str());

	if (CreateMinidumpFile(minidumpPath, exceptionPtr) != Errors::OK)
	{
		std::wstring writeErrorPath = PrintF(L"%sWindows-%s-WriteError.txt", dumpPath, systemTime.c_str());
		WriteTextFile(writeErrorPath, GetLastErrorMessage());
	}

	if (crashCallback)
	{
		crashCallback();
	}

	return EXCEPTION_EXECUTE_HANDLER;
}