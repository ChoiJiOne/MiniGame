#pragma once

#include <vector>
#include <string>
#include <cstdarg>

namespace GameUtils
{

/** 문자열 관련 기능. */
std::string PrintF(const char* format, ...);
std::wstring PrintF(const wchar_t* format, ...);
std::vector<std::string> Split(const std::string& text, const std::string& delimiter);
std::vector<std::wstring> Split(const std::wstring& text, const std::wstring& delimiter);
std::wstring Convert(const std::string& text);
std::string Convert(const std::wstring& text);
std::string ToLower(const std::string& text);
std::wstring ToLower(const std::wstring& text);
std::string ToUpper(const std::string& text);
std::wstring ToUpper(const std::wstring& text);
bool ToInt(const std::string& integer, int32_t& outInteger);
bool ToInt(const std::wstring& integer, int32_t& outInteger);
bool ToFloat(const std::string& floating, float& outFloating);
bool ToFloat(const std::wstring& floating, float& outFloating);

/** 파일 관련 기능. */
std::vector<uint8_t> ReadFile(const std::string& path);
std::vector<uint8_t> ReadFile(const std::wstring& path);
void WriteFile(const std::string& path, const std::vector<uint8_t>& buffer);
void WriteFile(const std::wstring& path, const std::vector<uint8_t>& buffer);
void MakeDirectory(const std::string& path);
void MakeDirectory(const std::wstring& path);
bool IsValidPath(const std::string& path);
bool IsValidPath(const std::wstring& path);
std::string GetBasePath(const std::string& path);
std::wstring GetBasePath(const std::wstring& path);
std::string RemoveBasePath(const std::string& path);
std::wstring RemoveBasePath(const std::wstring& path); 
std::string GetFileExtension(const std::string& path);
std::wstring GetFileExtension(const std::wstring& path);

}