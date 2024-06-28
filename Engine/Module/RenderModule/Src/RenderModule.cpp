#include <glad/glad.h>
#include <glad/glad_wgl.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "GLAssertion.h"
#include "Shader.h"
#include "RenderModule.h"

bool RenderModule::bIsInit = false;
HWND RenderModule::renderTargetHandle = nullptr;
HDC RenderModule::deviceContext = nullptr;
HGLRC RenderModule::glRenderContext = nullptr;
Mat4x4 RenderModule::ortho;
wchar_t RenderModule::lastErrorMessage[MAX_BUFFER_SIZE];
uint32_t RenderModule::cacheSize = 0;
std::array<std::unique_ptr<IResource>, RenderModule::MAX_RESOURCE_SIZE> RenderModule::cache;
std::array<bool, RenderModule::MAX_RESOURCE_SIZE> RenderModule::usage;

/**
 * @brief OpenGL의 버전입니다.
 *
 * @note 이 값은 4.6으로 고정되어 있습니다.
 */
#define FIX_OPENGL_MAJOR_VERSION 4 // OpenGL 주(4) 버전입니다.
#define FIX_OPENGL_MINOR_VERSION 6 // OpenGL 부(6) 버전입니다.

RenderModule::Errors RenderModule::Init(HWND windowHandle)
{
	if (bIsInit)
	{
		SetLastErrorMessage(L"Already initialize platform module.");
		return Errors::ERR_PREV_INIT;
	}

	renderTargetHandle = windowHandle;

	Errors result = CreateContext();
	if (result != Errors::OK)
	{
		return result;
	}

	if (!gladLoadGL())
	{
		SetLastErrorMessage(L"Failed to load OpenGL function.");
		return Errors::ERR_GL;
	}

	if (!ImGui_ImplOpenGL3_Init())
	{
		SetLastErrorMessage(L"Failed to Initialize IMGUI for OpenGL.");
		return Errors::ERR_IMGUI;
	}

	SetDepthMode(true);     // 기본적으로 깊이 테스트 모드는 활성화.
	SetAlphaBlendMode(true);// 기본적으로 알파 블랜딩 모드는 활성화.
	SetPointSizeMode(true); // 기본적으로 포인트 크기 모드는 활성화.
	SetCullFaceMode(true);  // 기본적으로 페이스 컬링 모드는 활성화.

	bIsInit = true;
	return Errors::OK;
}

RenderModule::Errors RenderModule::Uninit()
{
	if (!bIsInit)
	{
		SetLastErrorMessage(L"Haven't initialized or have already been uninitialized.");
		return Errors::ERR_NOT_INIT;
	}

	ImGui_ImplOpenGL3_Shutdown();

	for (std::size_t index = 0; index < cacheSize; ++index)
	{
		if (cache[index])
		{
			if (cache[index]->IsInitialized())
			{
				cache[index]->Release();
			}

			cache[index].reset();
		}
	}

	if (!::wglMakeCurrent(nullptr, nullptr))
	{
		SetLastWindowsErrorMessage();
		return Errors::ERR_WGL;
	}

	if (!::wglDeleteContext(glRenderContext))
	{
		SetLastWindowsErrorMessage();
		return Errors::ERR_WGL;
	}

	gladUnloadWGL();

	if (deviceContext)
	{
		if (!::ReleaseDC(renderTargetHandle, deviceContext))
		{
			SetLastWindowsErrorMessage();
			return Errors::ERR_WIN;
		}

		deviceContext = nullptr;
	}

	renderTargetHandle = nullptr;
	bIsInit = false;

	return Errors::OK;
}

void RenderModule::BeginFrame(float red, float green, float blue, float alpha, float depth, uint8_t stencil)
{
	ImGui::Render();

	int32_t width = 0;
	int32_t height = 0;
	GetScreenSize(width, height);

	SetViewport(0, 0, width, height);
	ortho = Mat4x4::Ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

	glClearColor(red, green, blue, alpha);
	glClearDepth(depth);
	glClearStencil(stencil);

	GL_FAILED(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void RenderModule::EndFrame()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	CHECK(SwapBuffers(deviceContext));
}

void RenderModule::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	glViewport(x, y, width, height);
}

void RenderModule::SetWindowViewport()
{
	int32_t width;
	int32_t height;
	GetScreenSize(width, height);
	SetViewport(0, 0, width, height);
}

void RenderModule::SetVsyncMode(bool bIsEnable)
{
	CHECK(wglSwapIntervalEXT(static_cast<int32_t>(bIsEnable)));
}

void RenderModule::SetDepthMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_DEPTH_TEST));
	}
	else
	{
		GL_FAILED(glDisable(GL_DEPTH_TEST));
	}
}

void RenderModule::SetStencilMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_STENCIL_TEST));
	}
	else
	{
		GL_FAILED(glDisable(GL_STENCIL_TEST));
	}
}

void RenderModule::SetAlphaBlendMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_BLEND));
		GL_FAILED(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO));
	}
	else
	{
		GL_FAILED(glDisable(GL_BLEND));
	}
}

void RenderModule::SetMultisampleMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_MULTISAMPLE));
	}
	else
	{
		GL_FAILED(glDisable(GL_MULTISAMPLE));
	}
}

void RenderModule::SetPointSizeMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_PROGRAM_POINT_SIZE));
	}
	else
	{
		GL_FAILED(glDisable(GL_PROGRAM_POINT_SIZE));
	}
}

void RenderModule::SetCullFaceMode(bool bIsEnable)
{
	if (bIsEnable)
	{
		GL_FAILED(glEnable(GL_CULL_FACE));
	}
	else
	{
		GL_FAILED(glDisable(GL_CULL_FACE));
	}
}

RenderModule::Errors RenderModule::GetScreenSize(int32_t& outWidth, int32_t& outHeight)
{
	RECT windowRect;
	if (!GetClientRect(renderTargetHandle, &windowRect))
	{
		SetLastWindowsErrorMessage();
		return Errors::ERR_WIN;
	}

	outWidth = static_cast<int32_t>(windowRect.right - windowRect.left);
	outHeight = static_cast<int32_t>(windowRect.bottom - windowRect.top);

	return Errors::OK;
}

const wchar_t* RenderModule::GetLastErrorMessage()
{
	return lastErrorMessage;
}

void RenderModule::ExecuteDrawVertex(uint32_t vertexCount, const EDrawMode& drawMode)
{
	GL_FAILED(glDrawArrays(static_cast<GLenum>(drawMode), 0, vertexCount));
}

void RenderModule::ExecuteDrawVertexInstanced(uint32_t vertexCount, const EDrawMode& drawMode, uint32_t instanceCounts)
{
	GL_FAILED(glDrawArraysInstanced(static_cast<GLenum>(drawMode), 0, vertexCount, instanceCounts));
}

void RenderModule::ExecuteDrawIndex(uint32_t indexCount, const EDrawMode& drawMode)
{
	GL_FAILED(glDrawElements(static_cast<GLenum>(drawMode), indexCount, GL_UNSIGNED_INT, nullptr));
}

void RenderModule::ExecuteDrawIndexInstanced(uint32_t indexCount, const EDrawMode& drawMode, uint32_t instanceCounts)
{
	GL_FAILED(glDrawElementsInstanced(static_cast<GLenum>(drawMode), indexCount, GL_UNSIGNED_INT, nullptr, instanceCounts));
}

void RenderModule::SetLastErrorMessage(const std::wstring& message)
{
	std::copy(message.begin(), message.end(), lastErrorMessage);
}

void RenderModule::SetLastWindowsErrorMessage()
{
	DWORD errorCode = static_cast<DWORD>(GetLastError());
	::FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		lastErrorMessage,
		MAX_BUFFER_SIZE,
		nullptr
	);
}

RenderModule::Errors RenderModule::CreateContext()
{
	deviceContext = ::GetDC(renderTargetHandle);
	if (deviceContext == nullptr)
	{
		SetLastErrorMessage(L"Failed to get device context.");
		return Errors::ERR_WIN;
	}

	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int32_t pixelFormat = ::ChoosePixelFormat(deviceContext, &pfd);
	if (!pixelFormat)
	{
		SetLastWindowsErrorMessage();
		return Errors::ERR_WGL;
	}

	if (!::SetPixelFormat(deviceContext, pixelFormat, &pfd))
	{
		SetLastWindowsErrorMessage();
		return Errors::ERR_WGL;
	}

	HGLRC dummyContext = ::wglCreateContext(deviceContext);
	if (!dummyContext)
	{
		SetLastWindowsErrorMessage();
		return Errors::ERR_WGL;
	}

	if (!::wglMakeCurrent(deviceContext, dummyContext))
	{
		SetLastWindowsErrorMessage();
		return Errors::ERR_WGL;
	}

	if (!gladLoadWGLLoader((GLADloadproc)(wglGetProcAddress), deviceContext))
	{
		SetLastErrorMessage(L"Failed to windows GL.");
		return Errors::ERR_WGL;
	}

	const std::array<int32_t, 9> attributes = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, FIX_OPENGL_MAJOR_VERSION,
		WGL_CONTEXT_MINOR_VERSION_ARB, FIX_OPENGL_MINOR_VERSION,
		WGL_CONTEXT_FLAGS_ARB, 0,
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};

	glRenderContext = wglCreateContextAttribsARB(deviceContext, nullptr, attributes.data());
	if (!glRenderContext)
	{
		SetLastErrorMessage(L"Failed to create ARB context.");
		return Errors::ERR_WGL;
	}

	if (!::wglMakeCurrent(nullptr, nullptr))
	{
		SetLastWindowsErrorMessage();
		return Errors::ERR_WGL;
	}

	if (!::wglDeleteContext(dummyContext))
	{
		SetLastWindowsErrorMessage();
		return Errors::ERR_WGL;
	}

	if (!::wglMakeCurrent(deviceContext, glRenderContext))
	{
		SetLastWindowsErrorMessage();
		return Errors::ERR_WGL;
	}

	return Errors::OK;
}