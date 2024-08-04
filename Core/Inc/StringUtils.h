#pragma once

#include <vector>
#include <string>
#include <cstdarg>


namespace GameMaker
{
class StringUtils
{
public:
	static std::string PrintF(const char* format, ...);
	static std::wstring PrintF(const wchar_t* format, ...);
	static std::vector<std::string> Split(const std::string& text, const std::string& delimiter);
	static std::vector<std::wstring> Split(const std::wstring& text, const std::wstring& delimiter);
	static std::wstring Convert(const std::string& text);
	static std::string Convert(const std::wstring& text);
	static std::string ToLower(const std::string& text);
	static std::wstring ToLower(const std::wstring& text);
	static std::string ToUpper(const std::string& text);
	static std::wstring ToUpper(const std::wstring& text);
	static bool ToInt(const std::string& integer, int32_t& outInteger);
	static bool ToInt(const std::wstring& integer, int32_t& outInteger);
	static bool ToFloat(const std::string& floating, float& outFloating);
	static bool ToFloat(const std::wstring& floating, float& outFloating);

private:
	static const uint32_t MAX_STRING_BUFFER_SIZE = 1024;

	static char charBuffer_[MAX_STRING_BUFFER_SIZE];
	static wchar_t wcharBuffer_[MAX_STRING_BUFFER_SIZE];
};

}