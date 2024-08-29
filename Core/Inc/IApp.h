#pragma once

#include <functional>
#include <vector>

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

	virtual bool Startup() = 0;
	virtual void Shutdown() = 0;
	virtual void Run() = 0;

	static IApp* Get();

	int32_t GetNumVideoDisplay();
	void GetVideoDisplaySize(int32_t index, GameMath::Vec2i& outSize);

	template <typename T>
	void GetScreenSize(T& outWidth, T& outHeight);

protected:
	void RunLoop(const std::function<void(float)>& frameCallback);
	
protected:
	static IApp* instance_;

	void* window_ = nullptr;
	void* context_ = nullptr; /** OpenGL 컨텍스트 */

	int32_t numVideoDisplay_ = 0;
	std::vector<GameMath::Vec2i> displaySizes_;

	GameTimer timer_;

	bool bIsQuit_ = false;
};