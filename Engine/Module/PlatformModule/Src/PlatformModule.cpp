#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>

#include "GameTimer.h"
#include "InputController.h"
#include "PlatformModule.h"

#define IDI_EXE_ICON 101

bool PlatformModule::bIsInit = false;
bool PlatformModule::bIsFullscreenMode = false;
bool PlatformModule::bIsMinimize = false;
bool PlatformModule::bIsMaximize = false;
HINSTANCE PlatformModule::instance = nullptr;
std::wstring PlatformModule::className;
HWND PlatformModule::windowHandle = nullptr;
wchar_t PlatformModule::lastErrorMessage[MAX_BUFFER_SIZE];
bool PlatformModule::bIsQuitLoop = false;
std::function<void()> PlatformModule::endLoopCallback;
std::function<void(float)> PlatformModule::frameCallback;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam);
	if (result)
	{
		return result;
	}

	EWindowEvent windowEvent = EWindowEvent::NONE;

	switch (message)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			windowEvent = EWindowEvent::FOCUS_LOST;
		}
		else // Active...
		{
			windowEvent = EWindowEvent::FOCUS_GAIN;
		}
		break;

	case WM_CLOSE:
		windowEvent = EWindowEvent::CLOSE;
		break;

	case WM_MOVE:
		windowEvent = EWindowEvent::MOVE;
		break;

	case WM_SIZE:
		switch (wParam)
		{
		case SIZE_RESTORED:
			if (PlatformModule::bIsMinimize)
			{
				windowEvent = EWindowEvent::EXIT_MIN;
			}
			else if (PlatformModule::bIsMaximize)
			{
				windowEvent = EWindowEvent::EXIT_MAX;
			}
			else
			{
				windowEvent = EWindowEvent::RESIZE;
			}

			PlatformModule::bIsMinimize = false;
			PlatformModule::bIsMaximize = false;
			break;

		case SIZE_MINIMIZED:
			PlatformModule::bIsMinimize = true;
			PlatformModule::bIsMaximize = false;
			windowEvent = EWindowEvent::ENTER_MIN;
			break;

		case SIZE_MAXIMIZED:
			PlatformModule::bIsMinimize = false;
			PlatformModule::bIsMaximize = true;
			windowEvent = EWindowEvent::ENTER_MAX;
			break;

		default:
			windowEvent = EWindowEvent::NONE;
			break;
		}
		break;

	case WM_ENTERSIZEMOVE:
		windowEvent = EWindowEvent::ENTER_RESIZE;
		break;

	case WM_EXITSIZEMOVE:
		windowEvent = EWindowEvent::EXIT_RESIZE;
		break;
		
	case WM_DESTROY:
		if (hwnd == PlatformModule::windowHandle)
		{
			if (PlatformModule::endLoopCallback)
			{
				PlatformModule::endLoopCallback();
			}

			PostQuitMessage(0);
		}
		break;
	}

	if (windowEvent != EWindowEvent::NONE)
	{
		InputController::ExecuteWindowEventAction(windowEvent);
	}

	return DefWindowProcW(hwnd, message, wParam, lParam);
}

PlatformModule::Errors PlatformModule::Init(const WindowConstructParams& windowParam)
{
	if (bIsInit)
	{
		SetLastErrorMessage(L"Already initialize platform module.");
		return Errors::ERR_PREV_INIT;
	}

	instance = GetModuleHandleW(nullptr);
	className = windowParam.title;

	Errors result = Errors::OK;

	result = RegisterWindowClass();
	if (result != Errors::OK)
	{
		return result;
	}

	result = CreatePlatformWindow(windowParam);
	if (result != Errors::OK)
	{
		return result;
	}

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	if (!ImGui_ImplWin32_InitForOpenGL(windowHandle))
	{
		SetLastErrorMessage(L"Failed to initialize IMGUI for Win32.");
		return Errors::ERR_IMGUI;
	}

	bIsInit = true;
	return Errors::OK;
}

PlatformModule::Errors PlatformModule::Uninit()
{
	if (!bIsInit)
	{
		SetLastErrorMessage(L"Haven't initialized or have already been uninitialized.");
		return Errors::ERR_NOT_INIT;
	}

	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (bIsFullscreenMode)
	{
		if (ChangeDisplaySettingsW(nullptr, 0) != DISP_CHANGE_SUCCESSFUL)
		{
			SetLastErrorMessage(L"Failed to change display setting.");
			return Errors::ERR_API;
		}
	}

	if (IsWindow(windowHandle))
	{
		BOOL result = DestroyWindow(windowHandle);
		if (!result)
		{
			SetLastErrorMessage();
			return Errors::ERR_API;
		}
	}

	if (!UnregisterClassW(className.c_str(), instance))
	{
		SetLastErrorMessage();
		return Errors::ERR_API;

	}

	bIsInit = false;
	return Errors::OK;
}

const wchar_t* PlatformModule::GetLastErrorMessage()
{
	return lastErrorMessage;
}

HWND PlatformModule::GetWindowHandle()
{
	return windowHandle;
}

void PlatformModule::RunLoop(const std::function<void(float)>& callback)
{
	frameCallback = callback;

	GameTimer gameTimer;
	gameTimer.Reset();

	MSG msg = { 0 };
	while (msg.message != WM_QUIT && !bIsQuitLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			InputController::Tick();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			gameTimer.Tick();
			float deltaSeconds = gameTimer.GetDeltaSeconds();

			if (frameCallback)
			{
				frameCallback(deltaSeconds);
			}
		}
	}

	if (bIsQuitLoop && PlatformModule::endLoopCallback)
	{
		PlatformModule::endLoopCallback();
	}
}

PlatformModule::Errors PlatformModule::GetScreenSize(int32_t& outWidth, int32_t& outHeight)
{
	RECT windowRect;
	if (!GetClientRect(windowHandle, &windowRect))
	{
		SetLastErrorMessage();
		return Errors::ERR_API;
	}

	outWidth = static_cast<int32_t>(windowRect.right - windowRect.left);
	outHeight = static_cast<int32_t>(windowRect.bottom - windowRect.top);

	return Errors::OK;
}

void PlatformModule::SetLastErrorMessage()
{
	DWORD errorCode = static_cast<DWORD>(GetLastError());
	FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM, 
		nullptr, 
		errorCode, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		lastErrorMessage, 
		MAX_BUFFER_SIZE, 
		nullptr
	);
}

void PlatformModule::SetLastErrorMessage(const std::wstring& message)
{
	std::copy(message.begin(), message.end(), lastErrorMessage);
}

PlatformModule::Errors PlatformModule::RegisterWindowClass()
{
	WNDCLASSEXW wcexw;
	ZeroMemory(&wcexw, sizeof(WNDCLASSEXW));

	wcexw.cbSize = sizeof(WNDCLASSEXW);
	wcexw.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcexw.lpfnWndProc = WindowProc;
	wcexw.cbClsExtra = 0;
	wcexw.cbWndExtra = 0;
	wcexw.hInstance = instance;
	wcexw.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_EXE_ICON));
	wcexw.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(IDI_EXE_ICON));
	wcexw.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcexw.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcexw.lpszMenuName = nullptr;
	wcexw.lpszClassName = className.c_str();

	if (!RegisterClassExW(&wcexw))
	{
		SetLastErrorMessage();
		return Errors::ERR_API;
	}

	return Errors::OK;
}

PlatformModule::Errors PlatformModule::CreatePlatformWindow(const WindowConstructParams& windowParam)
{
	bIsFullscreenMode = windowParam.bIsFullscreenMode;

	int32_t x = 0;
	int32_t y = 0;
	int32_t w = 0;
	int32_t h = 0;
	DWORD windowStyle = WS_VISIBLE;

	if (bIsFullscreenMode)
	{
		w = static_cast<int32_t>(GetSystemMetrics(SM_CXSCREEN));
		h = static_cast<int32_t>(GetSystemMetrics(SM_CYSCREEN));
		windowStyle |= WS_POPUP;

		DEVMODEW devMode = {};
		devMode.dmSize = sizeof(DEVMODEW);
		devMode.dmPelsWidth = static_cast<uint32_t>(w);
		devMode.dmPelsHeight = static_cast<uint32_t>(h);
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettingsW(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			SetLastErrorMessage(L"Failed to change display setting.");
			return Errors::ERR_API;
		}
	}
	else
	{
		RECT windowRect = { 0, 0, windowParam.w, windowParam.h };
		if (!AdjustWindowRect(&windowRect, windowStyle, FALSE))
		{
			SetLastErrorMessage();
			return Errors::ERR_API;
		}

		x = windowParam.x;
		y = windowParam.y;
		w = static_cast<int32_t>(windowRect.right - windowRect.left);
		h = static_cast<int32_t>(windowRect.bottom - windowRect.top);
		windowStyle |= windowParam.bIsResizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	}

	windowHandle = CreateWindowExW(
		WS_EX_APPWINDOW,
		windowParam.title.c_str(), 
		windowParam.title.c_str(),
		windowStyle, x, y, w, h,
		nullptr,
		nullptr,
		instance,
		nullptr
	);

	if (!windowHandle)
	{
		SetLastErrorMessage();
		return Errors::ERR_API;
	}

	return Errors::OK;
}