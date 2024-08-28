#pragma once

#include <string>


enum class ErrorCode : int32_t
{
	OK              = 0x00,
	FAILED          = 0x01, /** 일반적인 에러 */
	FAILED_STARTUP  = 0x02,
	FAILED_SHUTDOWN = 0x03,
	FAILED_SDL      = 0x04, /** SDL2 라이브러리 에러 */
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
	
private:
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