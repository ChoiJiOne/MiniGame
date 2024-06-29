#pragma once

#include <cstdint>
#include <string>
#include <vector>


namespace GameMaker
{
/**
 * @brief 파일 및 디렉토리 기능을 제공합니다.
 * 
 * @note 이 클래스의 멤버 변수 및 메서드는 모두 정적(static) 타입입니다.
 */
class FileUtils
{
public:
	/**
	 * @brief 파일을 읽고 버퍼에 저장합니다.
	 *
	 * @param path 파일의 경로입니다.
	 * 
	 * @return 파일 버퍼 벡터를 반환합니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createfilea
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-readfile
	 */
	static std::vector<uint8_t> ReadFile(const std::string& path);


	/**
	 * @brief 파일을 읽고 버퍼에 저장합니다.
	 *
	 * @param path 파일의 경로입니다.
	 * 
	 * @return 파일 버퍼 벡터를 반환합니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createfilea
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-readfile
	 */
	static std::vector<uint8_t> ReadFile(const std::wstring& path);


	/**
	 * @brief 버퍼를 파일에 씁니다.
	 *
	 * @param path 버퍼를 쓸 파일의 경로입니다.
	 * @param buffer 파일에 쓸 버퍼입니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createfilea
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-writefile
	 */
	static void WriteFile(const std::string& path, const std::vector<uint8_t>& buffer);


	/**
	 * @brief 버퍼를 파일에 씁니다.
	 *
	 * @param path 버퍼를 쓸 파일의 경로입니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createfilew
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-writefile
	 */
	static void WriteFile(const std::wstring& path, const std::vector<uint8_t>& buffer);


	/**
	 * @brief 디렉토리를 생성합니다.
	 *
	 * @param path 디렉토리를 생성할 경로입니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/winbase/nf-winbase-createdirectory
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createdirectoryw
	 */
	static void MakeDirectory(const std::string& path);


	/**
	 * @brief 디렉토리를 생성합니다.
	 *
	 * @param path 디렉토리를 생성할 경로입니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/winbase/nf-winbase-createdirectory
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createdirectoryw
	 */
	static void MakeDirectory(const std::wstring& path);


	/**
	 * @brief 파일 혹은 디렉토리가 유효한지 확인합니다.
	 *
	 * @param path 유효한지 확인할 파일 혹은 디렉토리 경로입니다.
	 *
	 * @return 파일 혹은 디렉토리가 유효하다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	static bool IsValidPath(const std::string& path);


	/**
	 * @brief 파일 혹은 디렉토리가 유효한지 확인합니다.
	 *
	 * @param path 유효한지 확인할 파일 혹은 디렉토리 경로입니다.
	 *
	 * @return 파일 혹은 디렉토리가 유효하다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	static bool IsValidPath(const std::wstring& path);


	/**
	 * @brief 경로에서 파일이나 디렉토리 이름을 제외한 경로를 얻습니다.
	 *
	 * @param path 파일이나 디렉토리 경로입니다.
	 *
	 * @return 파일이나 디렉토리 이름을 제외한 경로를 반환합니다.
	 *
	 * @example
	 * - 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "D:\\AAA\\BBB\\" 입니다.
	 * - 디렉토리 경로가 "D:\\AAA\\BBB\\CCC"라면, 반환하는 값은 "D:\\AAA\\BBB\\" 입니다.
	 */
	static std::string GetBasePath(const std::string& path);


	/**
	 * @brief 경로에서 파일이나 디렉토리 이름을 제외한 경로를 얻습니다.
	 *
	 * @param path 파일이나 디렉토리 경로입니다.
	 *
	 * @return 파일이나 디렉토리 이름을 제외한 경로를 반환합니다.
	 *
	 * @example
	 * - 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "D:\\AAA\\BBB\\" 입니다.
	 * - 디렉토리 경로가 "D:\\AAA\\BBB\\CCC"라면, 반환하는 값은 "D:\\AAA\\BBB\\" 입니다.
	 */
	static std::wstring GetBasePath(const std::wstring& path);


	/**
	 * @brief 전체 파일이나 디렉토리 경로에서 베이스 경로를 삭제합니다.
	 *
	 * @param path 베이스 경로를 삭제할 전체 파일이나 디렉토리 경로입니다.
	 *
	 * @return 베이스 경로가 삭제된 파일 경로를 반환합니다.
	 *
	 * @example
	 * - 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "CCC.a" 입니다.
	 * - 디렉토리 경로가 "D:\\AAA\\BBB\\CCC"라면, 반환하는 값은 "CCC" 입니다.
	 */
	static std::string RemoveBasePath(const std::string& path);


	/**
	 * @brief 전체 파일이나 디렉토리 경로에서 베이스 경로를 삭제합니다.
	 *
	 * @param path 베이스 경로를 삭제할 전체 파일이나 디렉토리 경로입니다.
	 *
	 * @return 베이스 경로가 삭제된 파일 경로를 반환합니다.
	 *
	 * @example
	 * - 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "CCC.a" 입니다.
	 * - 디렉토리 경로가 "D:\\AAA\\BBB\\CCC"라면, 반환하는 값은 "CCC" 입니다.
	 */
	static std::wstring RemoveBasePath(const std::wstring& path);


	/**
	 * @brief 전체 파일 경로에서 파일 확장자를 얻습니다.
	 *
	 * @param path 파일 확장자를 얻을 전체 파일 경로입니다.
	 *
	 * @return 전체 파일 경로의 파일 확장자를 반환합니다.
	 *
	 * @example
	 * - 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "a" 입니다.
	 * - 파일 경로가 "D:/AAA/BBB/CCC.a"라면, 반환하는 값은 "a" 입니다.
	 */
	static std::string GetFileExtension(const std::string& path);


	/**
	 * @brief 전체 파일 경로에서 파일 확장자를 얻습니다.
	 *
	 * @param path 파일 확장자를 얻을 전체 파일 경로입니다.
	 *
	 * @return 전체 파일 경로의 파일 확장자를 반환합니다.
	 *
	 * @example
	 * - 파일 경로가 "D:\\AAA\\BBB\\CCC.a"라면, 반환하는 값은 "a" 입니다.
	 * - 파일 경로가 "D:/AAA/BBB/CCC.a"라면, 반환하는 값은 "a" 입니다.
	 */
	static std::wstring GetFileExtension(const std::wstring& path);
};

}