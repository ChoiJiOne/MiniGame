#pragma once

#include <functional>
#include <string>

#include "Macro.h"

/** 크래시 매니저는 싱글턴 클래스입니다. */
class CrashManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(CrashManager);

	static CrashManager& Get();

	void SetCrashCallbackEvent(const std::function<void()>& callback);
	const wchar_t* GetCrashDumpFileNamePtr();
	const std::wstring& GetCrashDumpFileName();

private:
	CrashManager() = default;
	virtual ~CrashManager() {}
};