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
	void WriteFile(const std::string& path, const std::vector<uint8_t>& buffer);
	void WriteFile(const std::wstring& path, const std::vector<uint8_t>& buffer);
	void MakeDirectory(const std::string& path);
	void MakeDirectory(const std::wstring& path);
	bool IsValidPath(const std::string& path);
	bool IsValidPath(const std::wstring& path);

	/**
	 * 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "D:\\AAA\\BBB\\" 입니다.
	 * 디렉토리 경로가 "D:\\AAA\\BBB\\CCC"라면, 반환하는 값은 "D:\\AAA\\BBB\\" 입니다.
	 */
	std::string GetBasePath(const std::string& path);
	std::wstring GetBasePath(const std::wstring& path);

private:
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(FileManager);

private:
	static const uint32_t MAX_STRING_SIZE = 1024;

	char workingDirectoryA_[MAX_STRING_SIZE];
	wchar_t workingDirectoryW_[MAX_STRING_SIZE];
};

}