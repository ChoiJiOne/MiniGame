#pragma once

#include <array>
#include <functional>
#include <memory>
#include <vector>

#include "Camera2D.h"
#include "GameMath.h"
#include "GameTimer.h"
#include "GameUtils.h"
#include "Macro.h"

class IApp
{
public:
	IApp(const char* title, int32_t x, int32_t y, int32_t w, int32_t h, bool bIsResizble, bool bIsFullscreen);
	virtual ~IApp();

	DISALLOW_COPY_AND_ASSIGN(IApp); /** 명시적으로 복사 생성자 및 대입 연산자 삭제 */

	virtual void Startup() = 0;
	virtual void Shutdown() = 0;
	virtual void Run() = 0;

	static IApp* Get();
	
	bool IsQuit() const { return bIsQuit_; }
	bool IsResize() const { return bIsResize_; }
	bool IsMinimize() const { return bIsMinimize_; }
	bool IsMaximize() const { return bIsMaximize_; }
	bool IsEnterMouse() const { return bIsEnterMouse_; }
	bool IsGainFocus() const { return bIsGainFocus_; }

	/** 입력 관련 설정 */
	Press GetKeyPress(const Key& key);
	Press GetMousePress(const Mouse& mouse);
	const Vec2i& GetPrevMousePos() { return prevMouseState_.position; }
	const Vec2i& GetCurrMousePos() { return currMouseState_.position; }
	WindowEventID AddWindowEventAction(const WindowEvent& windowEvent, const std::function<void()>& eventAction, bool bIsActive = true);
	void DeleteWindowEventAction(const WindowEventID& windowEventID);
	void SetActiveWindowEventAction(const WindowEventID& windowEventID, bool bIsActive);

protected:
	struct WindowEventAction
	{
		bool				  bIsActive = false;               // 윈도우 이벤트의 활성화 여부입니다.
		WindowEvent		      windowEvent = WindowEvent::NONE; // 윈도우 이벤트입니다.
		std::function<void()> windowEventAction = nullptr;     // 윈도우 이벤트에 대응하는 액션입니다.
	};

	struct KeyboardState
	{
		static const int32_t BUFFER_SIZE = 512;        // 키보드의 키 값 상태를 저장하는 버퍼의 크기입니다.
		std::array<uint8_t, BUFFER_SIZE> keybordState; // 키보드의 키 값 상태를 저장하는 버퍼입니다.
	};

	struct MouseState
	{
		uint32_t state; // 마우스 버튼의 상태입니다.
		Vec2i position; // 마우스 버튼의 위치입니다.
	};

	void RunLoop(const std::function<void(float)>& frameCallback);

	void RegisterAppWindowEvent();
	bool IsPressKey(const KeyboardState& keyboardState, const Key& key);
	bool IsPressMouse(const MouseState& mouseState, const Mouse& mouse);

protected:
	static IApp* instance_;

	void* window_ = nullptr;

	GameTimer timer_;

	bool bIsQuit_ = false;
	bool bIsResize_ = false;
	bool bIsMaximize_ = false;
	bool bIsMinimize_ = false;
	bool bIsEnterMouse_ = false; /** 마우스가 윈도우 내부에 있는지 확인 */
	bool bIsGainFocus_ = true;

	KeyboardState prevKeyboardState_;
	KeyboardState currKeyboardState_;
	MouseState prevMouseState_;
	MouseState currMouseState_;

	static const uint32_t MAX_EVENT_ACTION_SIZE = 200;
	uint32_t windowEventActionSize_ = 0;
	std::array<WindowEventAction, MAX_EVENT_ACTION_SIZE> windowEventActions_;
};