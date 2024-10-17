#include <SDL2/SDL.h>

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Assertion.h"
#include "Config.h"
#include "RenderStateManager.h"

RenderStateManager RenderStateManager::instance_;

RenderStateManager& RenderStateManager::GetRef()
{
	return instance_;
}

RenderStateManager* RenderStateManager::GetPtr()
{
	return &instance_;
}

template <>
void RenderStateManager::GetScreenSize(float& outWidth, float& outHeight)
{
	int32_t w = 0;
	int32_t h = 0;
	SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(window_), &w, &h);

	outWidth = static_cast<float>(w);
	outHeight = static_cast<float>(h);
}

template <>
void RenderStateManager::GetScreenSize(int32_t& outWidth, int32_t& outHeight)
{
	SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(window_), &outWidth, &outHeight);
}

int32_t RenderStateManager::GetNumVideoDisplay()
{
	return numVideoDisplay_;
}

void RenderStateManager::GetVideoDisplaySize(int32_t index, Vec2i& outSize)
{
	CHECK(index >= 0 && index < displaySizes_.size());
	outSize = displaySizes_[index];
}

void RenderStateManager::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	glViewport(x, y, width, height);
}

void RenderStateManager::SetWindowViewport()
{
	int32_t w = 0;
	int32_t h = 0;
	GetScreenSize<int32_t>(w, h);
	SetViewport(0, 0, w, h);
}

void RenderStateManager::SetVsyncMode(bool bIsEnable)
{
	ASSERT(SDL_GL_SetSwapInterval(static_cast<int32_t>(bIsEnable)) == 0, "%s", SDL_GetError());
}

void RenderStateManager::SetDepthMode(bool bIsEnable)
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

void RenderStateManager::SetStencilMode(bool bIsEnable)
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

void RenderStateManager::SetAlphaBlendMode(bool bIsEnable)
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

void RenderStateManager::SetMultisampleMode(bool bIsEnable)
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

void RenderStateManager::SetCullFaceMode(bool bIsEnable)
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

bool RenderStateManager::HasGLExtension(const std::string& extension)
{
	auto it = std::find(extensions_.begin(), extensions_.end(), extension);
	return it != extensions_.end();
}

void RenderStateManager::BeginFrame(float red, float green, float blue, float alpha, float depth, uint8_t stencil)
{
	SetWindowViewport();

	glClearColor(red, green, blue, alpha);
	glClearDepth(depth);
	glClearStencil(stencil);

	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void RenderStateManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(reinterpret_cast<SDL_Window*>(window_));
}

void RenderStateManager::PreStartup()
{
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
}

void RenderStateManager::PostStartup()
{
	SDL_Window* window = reinterpret_cast<SDL_Window*>(window_);

	context_ = SDL_GL_CreateContext(window);
	ASSERT(context_ != nullptr, "%s", SDL_GetError());

	ASSERT(SDL_GL_MakeCurrent(window, context_) == 0, "%s", SDL_GetError());
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

	ASSERT(ImGui_ImplSDL2_InitForOpenGL(window, context_), "Failed to initialize ImGui for SDL2.");
	ASSERT(ImGui_ImplOpenGL3_Init(), "Failed to initialzie ImGui for OpenGL.");
}

void RenderStateManager::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context_);
	context_ = nullptr;
}