#pragma once

#include <cstdint>
#include <string>
#include <vector>


/**
 * @brief 파일 및 디렉토리 기능을 제공합니다.
 */
class FileModule
{
public:
	/**
	 * @brief 파일 모듈의 에러 목록입니다.
	 */
	enum class Errors
	{
		OK         = 0x00, // 에러 없음
		FAILED     = 0x01, // 일반적인 에러
		ERR_CREATE = 0x02, // 파일 생성 실패
		ERR_OPEN   = 0x03, // 파일 열기 실패
		ERR_WRITE  = 0x04, // 파일 쓰기 실패
		ERR_READ   = 0x05, // 파일 읽기 실패
		ERR_CLOSE  = 0x06, // 파일 닫기 실패
		ERR_API    = 0x07, // API 호출 실패
	};


	/**
	 * @brief 파일 모듈의 가장 최근 에러 메시지를 얻습니다.
	 *
	 * @return 파일 모듈의 가장 마지막에 발생한 에러 메시지를 반환합니다.
	 */
	static const char* GetLastErrorMessage();


	/**
	 * @brief 파일을 읽고 버퍼에 저장합니다.
	 *
	 * @param path 파일의 경로입니다.
	 * @param outBuffer 파일의 내용을 저장할 버퍼입니다.
	 *
	 * @return 파일을 읽어 버퍼에 저장하는 데 성공하면 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createfilea
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-readfile
	 */
	static Errors ReadFile(const std::string& path, std::vector<uint8_t>& outBuffer);


	/**
	 * @brief 파일을 읽고 버퍼에 저장합니다.
	 *
	 * @param path 파일의 경로입니다.
	 * @param outBuffer 파일의 내용을 저장할 버퍼입니다.
	 *
	 * @return 파일을 읽어 버퍼에 저장하는 데 성공하면 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createfilea
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-readfile
	 */
	static Errors ReadFile(const std::wstring& path, std::vector<uint8_t>& outBuffer);


	/**
	 * @brief 버퍼를 파일에 씁니다.
	 *
	 * @param path 버퍼를 쓸 파일의 경로입니다.
	 * @param buffer 파일에 쓸 버퍼입니다.
	 *
	 * @return 버퍼를 파일에 쓰는 데 성공하면 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createfilea
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-writefile
	 */
	static Errors WriteFile(const std::string& path, const std::vector<uint8_t>& buffer);


	/**
	 * @brief 버퍼를 파일에 씁니다.
	 *
	 * @param path 버퍼를 쓸 파일의 경로입니다.
	 * @param buffer 파일에 쓸 버퍼입니다.
	 *
	 * @return 버퍼를 파일에 쓰는 데 성공하면 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createfilew
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-writefile
	 */
	static Errors WriteFile(const std::wstring& path, const std::vector<uint8_t>& buffer);


	/**
	 * @brief 디렉토리를 생성합니다.
	 *
	 * @param path 디렉토리를 생성할 경로입니다.
	 *
	 * @return 디렉토리 생성에 성공했다면 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/winbase/nf-winbase-createdirectory
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createdirectoryw
	 */
	static Errors MakeDirectory(const std::string& path);


	/**
	 * @brief 디렉토리를 생성합니다.
	 *
	 * @param path 디렉토리를 생성할 경로입니다.
	 *
	 * @return 디렉토리 생성에 성공했다면 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/winbase/nf-winbase-createdirectory
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createdirectoryw
	 */
	static Errors MakeDirectory(const std::wstring& path);


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


private:
	/**
	 * @brief 에러 메시지를 설정합니다.
	 *
	 * @note 에러가 존재하지 않으면 아무 동작도 수행하지 않습니다.
	 */
	static void SetLastErrorMessage();


private:
	/**
	 * @brief 문자열 버퍼의 최대 크기입니다.
	 */
	static const uint32_t MAX_BUFFER_SIZE = 1024;


	/**
	 * @brief 가장 마지막 에러 메시지를 저장하는 버퍼입니다.
	 */
	static char lastErrorMessage[MAX_BUFFER_SIZE];
};