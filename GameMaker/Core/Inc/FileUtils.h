#pragma once

#include <cstdint>
#include <string>
#include <vector>


namespace GameMaker
{
class FileUtils
{
public:
	static std::vector<uint8_t> ReadFile(const std::string& path);
	static std::vector<uint8_t> ReadFile(const std::wstring& path);
	static void WriteFile(const std::string& path, const std::vector<uint8_t>& buffer);
	static void WriteFile(const std::wstring& path, const std::vector<uint8_t>& buffer);
	static void MakeDirectory(const std::string& path);
	static void MakeDirectory(const std::wstring& path);
	static bool IsValidPath(const std::string& path);
	static bool IsValidPath(const std::wstring& path);

	/** 
	 * 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "D:\\AAA\\BBB\\" 입니다.
	 * 디렉토리 경로가 "D:\\AAA\\BBB\\CCC"라면, 반환하는 값은 "D:\\AAA\\BBB\\" 입니다.
	 */
	static std::string GetBasePath(const std::string& path);

	/**
	 * 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "D:\\AAA\\BBB\\" 입니다.
	 * 디렉토리 경로가 "D:\\AAA\\BBB\\CCC"라면, 반환하는 값은 "D:\\AAA\\BBB\\" 입니다.
	 */
	static std::wstring GetBasePath(const std::wstring& path);

	/**
	 * 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "CCC.a" 입니다.
	 * 디렉토리 경로가 "D:\\AAA\\BBB\\CCC"라면, 반환하는 값은 "CCC" 입니다.
	 */
	static std::string RemoveBasePath(const std::string& path);

	/**
	 * 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "CCC.a" 입니다.
	 * 디렉토리 경로가 "D:\\AAA\\BBB\\CCC"라면, 반환하는 값은 "CCC" 입니다.
	 */
	static std::wstring RemoveBasePath(const std::wstring& path);

	/**
	 * 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "a" 입니다.
	 * 파일 경로가 "D:/AAA/BBB/CCC.a"라면, 반환하는 값은 "a" 입니다.
	 */
	static std::string GetFileExtension(const std::string& path);

	/**
	 * 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "a" 입니다.
	 * 파일 경로가 "D:/AAA/BBB/CCC.a"라면, 반환하는 값은 "a" 입니다.
	 */
	static std::wstring GetFileExtension(const std::wstring& path);
};

}