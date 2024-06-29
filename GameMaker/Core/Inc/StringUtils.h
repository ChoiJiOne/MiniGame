#pragma once

#include <vector>
#include <string>
#include <cstdarg>


/**
 * @brief 문자열의 확장 기능을 제공합니다.
 *
 * @note 문자열은 std::string, std::wstring 표준 문자열 기준입니다.
 */
namespace StringUtils
{
	/**
	 * @brief 형식화된 문자열을 반환합니다.
	 *
	 * @note
	 * - 이 메서드는 snprintf와 같은 함수를 대체하기 위한 것입니다.
	 * - 문자열의 길이는 최대 1024 입니다.
	 *
	 * @param format 형식 문자열입니다.
	 * @param ... 문자열의 형식에 대응하는 가변인자입니다.
	 *
	 * @return 형식화된 표준 문자열을 반환합니다.
	 *
	 * @see https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l?view=msvc-170
	 */
	std::string PrintF(const char* format, ...);


	/**
	 * @brief 형식화된 문자열을 반환합니다.
	 *
	 * @note
	 * - 이 메서드는 snprintf와 같은 함수를 대체하기 위한 것입니다.
	 * - 문자열의 길이는 최대 1024 입니다.
	 *
	 * @param format 형식 문자열입니다.
	 * @param ... 문자열의 형식에 대응하는 가변인자입니다.
	 *
	 * @return 형식화된 표준 문자열을 반환합니다.
	 *
	 * @see https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l?view=msvc-170
	 */
	std::wstring PrintF(const wchar_t* format, ...);


	/**
	 * @brief 특정 문자열을 기준으로 텍스트를 분리합니다.
	 *
	 * @param text 분리할 대상 문자열입니다.
	 * @param delimiter 분리를 위한 기준 문자열입니다.
	 *
	 * @return 분리된 문자열을 담고 있는 벡터(std::vector)를 반환합니다.
	 */
	std::vector<std::string> Split(const std::string& text, const std::string& delimiter);


	/**
	 * @brief 특정 문자열을 기준으로 텍스트를 분리합니다.
	 *
	 * @param text 분리할 대상 문자열입니다.
	 * @param delimiter 분리를 위한 기준 문자열입니다.
	 *
	 * @return 분리된 문자열을 담고 있는 벡터(std::vector)입니다.
	 */
	std::vector<std::wstring> Split(const std::wstring& text, const std::wstring& delimiter);


	/**
	 * @brief char 문자열을 wchar_t 문자열로 변환합니다.
	 *
	 * @see https://en.cppreference.com/w/cpp/string/multibyte/mbstowcs
	 *
	 * @param text 변환할 표준 텍스트입니다.
	 *
	 * @return 변환된 문자열을 반환합니다.
	 */
	std::wstring Convert(const std::string& text);


	/**
	 * @brief wchar_t 문자열을 char 문자열로 변환합니다.
	 *
	 * @see https://en.cppreference.com/w/cpp/string/multibyte/wcstombs
	 *
	 * @param text 변환할 표준 텍스트입니다.
	 *
	 * @return 변환된 문자열을 반환합니다.
	 */
	std::string Convert(const std::wstring& text);


	/**
	 * @brief 문자열의 알파벳을 모두 소문자로 변환합니다.
	 *
	 * @param text 변환할 문자열입니다.
	 *
	 * @return 문자열 내의 알파벳이 모두 소문자로 변환된 전체 문자열을 반환합니다.
	 */
	std::string ToLower(const std::string& text);


	/**
	 * @brief 문자열의 알파벳을 모두 소문자로 변환합니다.
	 *
	 * @param text 변환할 문자열입니다.
	 *
	 * @return 문자열 내의 알파벳이 모두 소문자로 변환된 전체 문자열을 반환합니다.
	 */
	std::wstring ToLower(const std::wstring& text);


	/**
	 * @brief 문자열의 알파벳을 모두 대문자로 변환합니다.
	 *
	 * @param text 변환할 문자열입니다.
	 *
	 * @return 문자열 내의 알파벳이 모두 대문자로 변환된 전체 문자열을 반환합니다.
	 */
	std::string ToUpper(const std::string& text);


	/**
	 * @brief 문자열의 알파벳을 모두 대문자로 변환합니다.
	 *
	 * @param text 변환할 문자열입니다.
	 *
	 * @return 문자열 내의 알파벳이 모두 대문자로 변환된 전체 문자열을 반환합니다.
	 */
	std::wstring ToUpper(const std::wstring& text);


	/**
	 * @brief 정수로 구성된 문자열을 10진수 정수 값으로 변환합니다.
	 *
	 * @param integer 정수로 구성된 문자열입니다.
	 * @param outInteger 변환된 정수 값을 저장할 참조 변수입니다.
	 *
	 * @return 변환에 성공하면 true, 그렇지 않으면 false를 반환합니다.
	 *
	 * @see https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-170
	 */
	bool ToInt(const std::string& integer, int32_t& outInteger);


	/**
	 * @brief 정수로 구성된 문자열을 10진수 정수 값으로 변환합니다.
	 *
	 * @param integer 정수로 구성된 문자열입니다.
	 * @param outInteger 변환된 정수 값을 저장할 참조 변수입니다.
	 *
	 * @return 변환에 성공하면 true, 그렇지 않으면 false를 반환합니다.
	 *
	 * @see https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-170
	 */
	bool ToInt(const std::wstring& integer, int32_t& outInteger);


	/**
	 * @brief 부동 소수점으로 구성된 문자열을 부동 소수점 수 값으로 변환합니다.
	 *
	 * @param floating 부동 소수점으로 구성된 문자열입니다.
	 * @param outFloating 변환된 부동 소수점 수 값을 저장할 참조 변수입니다.
	 *
	 * @return 변환에 성공하면 true, 그렇지 않으면 false를 반환합니다.
	 *
	 * @see https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/strtof-strtof-l-wcstof-wcstof-l?view=msvc-170
	 */
	bool ToFloat(const std::string& floating, float& outFloating);


	/**
	 * @brief 부동 소수점으로 구성된 문자열을 부동 소수점 수 값으로 변환합니다.
	 *
	 * @param floating 부동 소수점으로 구성된 문자열입니다.
	 * @param outFloating 변환된 부동 소수점 수 값을 저장할 참조 변수입니다.
	 *
	 * @return 변환에 성공하면 true, 그렇지 않으면 false를 반환합니다.
	 *
	 * @see https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/strtof-strtof-l-wcstof-wcstof-l?view=msvc-170
	 */
	bool ToFloat(const std::wstring& floating, float& outFloating);
};