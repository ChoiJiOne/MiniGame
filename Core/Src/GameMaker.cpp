/** 서드 파티 라이브러리 사용을 위한 헤더 선언 시작 */
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <SDL2/SDL.h>

#include <mimalloc-new-delete.h>
/** 서드 파티 라이브러리 사용을 위한 헤더 선언 종료*/

#include <map>

#include "Config.h"
#include "GameMaker.h"

static bool bIsStartup_ = false;
static bool bIsInitSDL_ = false;
static SDL_Window* window_ = nullptr;
static int32_t numVideoDisplay_ = 0;
static std::vector<GameMath::Vec2i> displaySizes_;

GameError GameMaker::Startup(const char* title, int32_t x, int32_t y, int32_t w, int32_t h, bool bIsResizble, bool bIsFullscreen)
{
	if (bIsStartup_)
	{
		return GameError(ErrorCode::FAILED_STARTUP, "Already startup GameMaker.");
	}

	bIsInitSDL_ = (SDL_Init(SDL_INIT_EVERYTHING) == 0);  /** 실패 시 음수의 에러 코드 반환 */
	if (!bIsInitSDL_)
	{
		return SDLError();
	}

	std::map<SDL_GLattr, int32_t> attributes =
	{
		{ SDL_GL_CONTEXT_FLAGS,         SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG },
		{ SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE            },
		{ SDL_GL_CONTEXT_MAJOR_VERSION, GL_MAJOR                               },
		{ SDL_GL_CONTEXT_MINOR_VERSION, GL_MINOR                               },
		{ SDL_GL_RED_SIZE,              GL_RED_SIZE                            },
		{ SDL_GL_GREEN_SIZE,            GL_GREEN_SIZE                          },
		{ SDL_GL_BLUE_SIZE,             GL_BLUE_SIZE                           },
		{ SDL_GL_ALPHA_SIZE,            GL_ALPHA_SIZE                          },
		{ SDL_GL_DEPTH_SIZE,            GL_DEPTH_SIZE                          },
		{ SDL_GL_STENCIL_SIZE,          GL_STENCIL_SIZE                        },
		{ SDL_GL_DOUBLEBUFFER,          GL_DOUBLE_BUFFER                       },
		{ SDL_GL_MULTISAMPLEBUFFERS,    GL_MULTISAMPLE_BUFFERS                 },
		{ SDL_GL_MULTISAMPLESAMPLES,    GL_MULTISAMPLE_SAMPLES                 },
	};

	for (const auto& attribute : attributes)
	{
		if (SDL_GL_SetAttribute(attribute.first, attribute.second) < 0)
		{
			return SDLError();
		}
	}

	numVideoDisplay_ = SDL_GetNumVideoDisplays();
	if (numVideoDisplay_ < 1)
	{
		return SDLError();
	}

	displaySizes_.resize(numVideoDisplay_);
	for (uint32_t index = 0; index < displaySizes_.size(); ++index)
	{
		SDL_DisplayMode displayMode;
		if (SDL_GetDesktopDisplayMode(index, &displayMode) < 0)
		{
			return SDLError();
		}

		displaySizes_[index].x = displayMode.w;
		displaySizes_[index].y = displayMode.h;
	}

	uint32_t baseFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	baseFlags |= (bIsResizble ? SDL_WINDOW_RESIZABLE : 0);
	baseFlags |= (bIsFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

	window_ = SDL_CreateWindow(title, x, y, w, h, baseFlags);
	if (!window_)
	{
		return SDLError();
	}
	
	bIsStartup_ = true;
	return GameError(ErrorCode::OK, "Succeed startup GameMaker.");
}

GameError GameMaker::Shutdown()
{
	if (!bIsStartup_)
	{
		if (window_)
		{
			SDL_DestroyWindow(window_);
			window_ = nullptr;
		}

		if (bIsInitSDL_)
		{
			bIsInitSDL_ = false;
			SDL_Quit();
		}

		return GameError(ErrorCode::FAILED_SHUTDOWN, "Startup has not called, or Shutdown has already been invoked.");
	}

	if (window_)
	{
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}

	if (bIsInitSDL_)
	{
		bIsInitSDL_ = false;
		SDL_Quit();
	}

	bIsStartup_ = false;
	return GameError(ErrorCode::OK, "Succeed shutdown GameMaker.");
}

void GameMaker::RunLoop(const std::function<void(float)>& frameCallback)
{
}

int32_t GameMaker::GetNumVideoDisplay()
{
	return numVideoDisplay_;
}

GameError GameMaker::GetVideoDisplaySize(int32_t index, GameMath::Vec2i& outSize)
{
	if (index < 0 || index >= displaySizes_.size())
	{
		return GameError(ErrorCode::BUFFER_OUT_OF_RANGE, "Video Display index is out of range.");
	}

	outSize = displaySizes_[index];
	return GameError(ErrorCode::OK, "Succeed get video display size.");
}