#pragma once

#include <string>
#include <map>


enum class ErrorCode : int32_t
{
	OK                  = 0x00,
	FAILED              = 0x01, /** 일반적인 에러 */
	FAILED_STARTUP      = 0x02,
	FAILED_SHUTDOWN     = 0x03,
	FAILED_SDL          = 0x04, /** SDL2 라이브러리 에러 */
	FAILED_WINDOWS      = 0x05, /** Windows API 에러 */
	FAILED_IMGUI        = 0x06, /** ImGui 에러 */
	FAILED_OPENGL       = 0x07, /** OpenGL 에러 */
	BUFFER_OUT_OF_RANGE = 0x08, /** 배열의 범위를 벗어난 에러 */
};

class GameError
{
public:
	GameError() {}
	GameError(const ErrorCode& errorCode, const std::string message) : errorCode_(errorCode), message_(message) {}
	GameError(GameError&& instance) noexcept : errorCode_(instance.errorCode_), message_(instance.message_) {}
	GameError(const GameError& instance) noexcept : errorCode_(instance.errorCode_), message_(instance.message_) {}

	virtual ~GameError() {}

	GameError& operator=(GameError&& instance) noexcept
	{
		if (this == &instance) return *this;

		errorCode_ = instance.errorCode_;
		message_ = instance.message_;

		return *this;
	}

	GameError& operator=(const GameError& instance) noexcept
	{
		if (this == &instance) return *this;

		errorCode_ = instance.errorCode_;
		message_ = instance.message_;

		return *this;
	}

	const ErrorCode& GetCode() const { return errorCode_; }
	const std::string& GetMessage() const { return message_; }
	const char* GetMessagePtr() const { return message_.c_str(); }
	
protected:
	ErrorCode errorCode_ = ErrorCode::OK;
	std::string message_;
};

#if defined(SDL_h_) /** SDL이 정의 되었을 때. */
#include <SDL2/SDL.h>
class SDLError : public GameError
{
public:
	SDLError() : GameError(ErrorCode::FAILED_SDL, SDL_GetError()) {}
	virtual ~SDLError() {}
};
#endif

#if defined(_WIN32) || defined(_WIN64) /** Windows OS에서. */
#include <windows.h>
class WindowsError : public GameError
{
public:
	WindowsError() : GameError(ErrorCode::FAILED_WINDOWS, GetErrorMessage()) {}
	virtual ~WindowsError() {}

private:
	static std::string GetErrorMessage()
	{
		static const uint32_t MAX_BUFFER_SIZE = 1024;
		static char buffer[MAX_BUFFER_SIZE];

		uint32_t size = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, MAX_BUFFER_SIZE, nullptr);

		return std::string(buffer, size);
	}
};
#endif

#if defined(__glad_h_) /** OpenGL이 정의되었을 때. */
class GLError : public GameError
{
public:
	GLError(GLenum errorCode) : GameError(ErrorCode::FAILED_OPENGL, GetErrorMessage(errorCode)) {}
	virtual ~GLError() {}

	static std::string GetErrorMessage(const GLenum errorCode)
	{
		static std::map<GLenum, std::string> errorMessages =
		{
			{ GL_NO_ERROR,                      "No error has been recorded. (GL_NO_ERROR)"                                                                              },
			{ GL_INVALID_ENUM,                  "An unacceptable value is specified for an enumerated argument. (GL_INVALID_ENUM)"                                       },
			{ GL_INVALID_VALUE,                 "A numeric argument is out of range. (GL_INVALID_VALUE)"                                                                 },
			{ GL_INVALID_OPERATION,             "The specified operation is not allowed in the current state. (GL_INVALID_OPERATION)"                                    },
			{ GL_INVALID_FRAMEBUFFER_OPERATION, "The framebuffer object is not complete. (GL_INVALID_FRAMEBUFFER_OPERATION)"                                             },
			{ GL_OUT_OF_MEMORY,                 "There is not enough memory left to execute the command. (GL_OUT_OF_MEMORY)"                                             },
			{ GL_STACK_UNDERFLOW,               "An attempt has been made to perform an operation that would cause an internal stack to underflow. (GL_STACK_UNDERFLOW)" },
			{ GL_STACK_OVERFLOW,                "An attempt has been made to perform an operation that would cause an internal stack to overflow. (GL_STACK_OVERFLOW)"   },
			{ GL_CONTEXT_LOST,                  "This occurs when the OpenGL context becomes lost due to a graphics card reset. (GL_CONTEXT_LOST)"                       },
			{ GL_NONE,                          "Undefined error message."                                                                                               },
		};

		auto it = errorMessages.find(errorCode);
		return (it != errorMessages.end()) ? it->second : errorMessages.at(GL_NONE);
	}
};
#endif