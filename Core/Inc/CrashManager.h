#pragma once

#include <functional>
#include <string>

#include "Macro.h"

class CrashManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(CrashManager);

	static CrashManager& Get();

	std::wstring GetCurrentSystemTimeAsString();
	std::wstring GetCrashDumpPath();
	void SetCrashCallbackEvent(const std::function<void()>& callback) { crashCallbackEvent_ = callback; }

private:
	CrashManager() = default;
	virtual ~CrashManager() {}
	
private:
	std::function<void()> crashCallbackEvent_ = nullptr;
};