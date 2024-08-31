#include <algorithm>
#include <map>
#include <windows.h>

/** 서드 파티 라이브러리 사용을 위한 헤더 선언 시작 */
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <SDL2/SDL.h>

#include <glad/glad.h>

#include <mimalloc-new-delete.h>
/** 서드 파티 라이브러리 사용을 위한 헤더 선언 종료*/

#include "Assertion.h"
#include "AudioManager.h"
#include "CrashManager.h"
#include "Config.h"
#include "EntityManager.h"
#include "IApp.h"
#include "ResourceManager.h"

static LPTOP_LEVEL_EXCEPTION_FILTER topLevelExceptionFilter_;
extern LONG WINAPI DetectApplicationCrash(EXCEPTION_POINTERS* ep);

IApp* IApp::instance_ = nullptr;

IApp::IApp(const char* title, int32_t x, int32_t y, int32_t w, int32_t h, bool bIsResizble, bool bIsFullscreen)
{
	topLevelExceptionFilter_ = ::SetUnhandledExceptionFilter(DetectApplicationCrash);

	instance_ = this;

	ASSERT(SDL_Init(SDL_INIT_EVERYTHING) == 0, "%s", SDL_GetError());

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
		ASSERT((SDL_GL_SetAttribute(attribute.first, attribute.second) == 0), "%s", SDL_GetError());
	}

	numVideoDisplay_ = SDL_GetNumVideoDisplays();
	ASSERT(numVideoDisplay_ >= 1, "%s", SDL_GetError());

	displaySizes_.resize(numVideoDisplay_);
	for (uint32_t index = 0; index < displaySizes_.size(); ++index)
	{
		SDL_DisplayMode displayMode;
		ASSERT((SDL_GetDesktopDisplayMode(index, &displayMode) == 0), "%s", SDL_GetError());

		displaySizes_[index].x = displayMode.w;
		displaySizes_[index].y = displayMode.h;
	}

	uint32_t baseFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	baseFlags |= (bIsResizble ? SDL_WINDOW_RESIZABLE : 0);
	baseFlags |= (bIsFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);


	SDL_Window* window = SDL_CreateWindow(title, x, y, w, h, baseFlags);
	ASSERT(window != nullptr, "%s", SDL_GetError());

	SDL_GLContext context = SDL_GL_CreateContext(window);
	ASSERT(context != nullptr, "%s", SDL_GetError());

	ASSERT(SDL_GL_MakeCurrent(window, context) == 0, "%s", SDL_GetError());

	window_ = window;
	context_ = context;

	ASSERT(gladLoadGLLoader((GLADloadproc)(SDL_GL_GetProcAddress)), "Failed to load OpenGL function.");

	int32_t extensions = 0;
	GL_CHECK(glGetIntegerv(GL_NUM_EXTENSIONS, &extensions));

	extensions_ = std::vector<std::string>(extensions);
	for (uint32_t index = 0; index < extensions; ++index)
	{
		const char* ext = (const char*)(glGetStringi(GL_EXTENSIONS, index));
		extensions_[index] = ext;
	}
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	ASSERT(ImGui_ImplSDL2_InitForOpenGL(window, context), "Failed to initialize ImGui for SDL2.");
	ASSERT(ImGui_ImplOpenGL3_Init(), "Failed to initialzie ImGui for OpenGL.");

	AudioManager::Get().Startup();
}

IApp::~IApp()
{
	EntityManager::Get().Cleanup();
	ResourceManager::Get().Cleanup();
	AudioManager::Get().Shutdown();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context_);
	context_ = nullptr;

	SDL_DestroyWindow(reinterpret_cast<SDL_Window*>(window_));
	window_ = nullptr;

	SDL_Quit();

	::SetUnhandledExceptionFilter(topLevelExceptionFilter_);
}

IApp* IApp::Get()
{
	return instance_;
}

int32_t IApp::GetNumVideoDisplay()
{
	return numVideoDisplay_;
}

void IApp::GetVideoDisplaySize(int32_t index, GameMath::Vec2i& outSize)
{
	CHECK(index >= 0 && index < displaySizes_.size());
	outSize = displaySizes_[index];
}

template <>
void IApp::GetScreenSize(float& outWidth, float& outHeight)
{
	int32_t w = 0;
	int32_t h = 0;
	SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(window_), &w, &h);

	outWidth = static_cast<float>(w);
	outHeight = static_cast<float>(h);
}

template <>
void IApp::GetScreenSize(int32_t& outWidth, int32_t& outHeight)
{
	SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(window_), &outWidth, &outHeight);
}

void IApp::RunLoop(const std::function<void(float)>& frameCallback)
{
	timer_.Reset();

	SDL_Event e;
	while (!bIsQuit_)
	{
		if (SDL_PollEvent(&e))
		{
			ImGui_ImplSDL2_ProcessEvent(&e);

			switch (e.type)
			{
			case SDL_QUIT:
				bIsQuit_ = true;
				break;
			case SDL_WINDOWEVENT:
				break;
			}
		}
		else
		{
			timer_.Tick();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			if (frameCallback)
			{
				frameCallback(timer_.GetDeltaSeconds());
			}
		}
	}
}

void IApp::BeginFrame(float red, float green, float blue, float alpha, float depth, uint8_t stencil)
{
	SetWindowViewport();

	glClearColor(red, green, blue, alpha);
	glClearDepth(depth);
	glClearStencil(stencil);

	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void IApp::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(reinterpret_cast<SDL_Window*>(window_));
}

void IApp::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	glViewport(x, y, width, height);
}

void IApp::SetWindowViewport()
{
	int32_t w = 0;
	int32_t h = 0;
	GetScreenSize<int32_t>(w, h);
	SetViewport(0, 0, w, h);
}

void IApp::SetVsyncMode(bool bIsEnable)
{
	ASSERT(SDL_GL_SetSwapInterval(static_cast<int32_t>(bIsEnable)) == 0, "%s", SDL_GetError());
}

void IApp::SetDepthMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_CHECK(glEnable(GL_DEPTH_TEST));
	}
	else
	{
		GL_CHECK(glDisable(GL_DEPTH_TEST));
	}
}

void IApp::SetStencilMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_CHECK(glEnable(GL_STENCIL_TEST));
	}
	else
	{
		GL_CHECK(glDisable(GL_STENCIL_TEST));
	}
}

void IApp::SetAlphaBlendMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_CHECK(glEnable(GL_BLEND));
		GL_CHECK(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO));
	}
	else
	{
		GL_CHECK(glDisable(GL_BLEND));
	}
}

void IApp::SetMultisampleMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_CHECK(glEnable(GL_MULTISAMPLE));
	}
	else
	{
		GL_CHECK(glDisable(GL_MULTISAMPLE));
	}
}

void IApp::SetCullFaceMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_CHECK(glEnable(GL_CULL_FACE));
	}
	else
	{
		GL_CHECK(glDisable(GL_CULL_FACE));
	}
}

bool IApp::HasGLExtension(const std::string& extension)
{
	auto it = std::find(extensions_.begin(), extensions_.end(), extension);
	return it != extensions_.end();
}