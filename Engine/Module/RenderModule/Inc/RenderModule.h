#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#error "not support platform"
#endif

#include "Vec.h"
#include "Mat.h"

#include "IResource.h"


/**
 * @brief 기본 도형의 그리기 모드입니다.
 */
enum class EDrawMode : int32_t
{
	Points      = 0x0000,
	Lines       = 0x0001,
	LineStrip   = 0x0003,
	Triangles   = 0x0004,
	TriangleFan = 0x0006,
	None        = 0xFFFF,
};


/**
 * @brief 렌더링 관련 처리를 수행하는 모듈입니다.
 *
 * @note 이 클래스의 모든 멤버 변수와 메서드는 모두 정적(static) 타입입니다.
 */
class RenderModule
{
public:
	/**
	 * @brief 렌더 모듈의 에러 처리 목록입니다.
	 */
	enum class Errors
	{
		OK            = 0x00, // 에러가 없습니다.
		FAILED        = 0x01, // 일반적인 에러입니다.
		ERR_WIN       = 0x02, // Windows 에러입니다.
		ERR_WGL       = 0x03, // Windows GL 에러입니다.
		ERR_GL        = 0x04, // OpenGL 에러입니다.
		ERR_PREV_INIT = 0x05, // 이전에 초기화를 수행했습니다.
		ERR_NOT_INIT  = 0x06, // 초기화가 수행되지 않았습니다.
		ERR_IMGUI     = 0x07, // IMGUI 관련 에러입니다.
	};


public:
	/**
	 * @brief 렌더 모듈을 초기화합니다.
	 *
	 * @param windowHandle 렌더링 대상이 되는 윈도우 핸들입니다.
	 *
	 * @return 초기화에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors Init(HWND windowHandle);


	/**
	 * @brief 렌더 모듈의 초기화를 해제합니다.
	 *
	 * @return 초기화 해제에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors Uninit();


	/**
	 * @brief 프레임 렌더링을 시작합니다.
	 *
	 * @param red 색상 버퍼의 R 값입니다.
	 * @param green 색상 버퍼의 G 값입니다.
	 * @param blue 색상 버퍼의 B 값입니다.
	 * @param alpha 색상 버퍼의 A 값입니다.
	 * @param depth 깊이 버퍼의 초기화 값입니다.
	 * @param stencil 스텐실 버퍼의 초기화 값입니다.
	 */
	static void BeginFrame(float red, float green, float blue, float alpha, float depth = 1.0f, uint8_t stencil = 0);


	/**
	 * @brief 백버퍼와 프론트 버퍼를 스왑합니다.
	 *
	 * @return 백버퍼와 프론트 버퍼의 스왑에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static void EndFrame();


	/**
	 * @brief 뷰 포트를 설정합니다.
	 *
	 * @param x 뷰포트 사각형의 왼쪽 아래 모서리(픽셀)입니다. 기본값은 (0,0)입니다.
	 * @param y 뷰포트 사각형의 왼쪽 아래 모서리(픽셀)입니다. 기본값은 (0,0)입니다.
	 * @param width 뷰포트의 너비입니다. OpenGL 컨텍스트가 창에 처음 연결되면 너비 와 높이 가 해당 창의 크기로 설정됩니다.
	 * @param height 뷰포트의 높이입니다. OpenGL 컨텍스트가 창에 처음 연결되면 너비 와 높이 가 해당 창의 크기로 설정됩니다.
	 */
	static void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);


	/**
	 * @brief 윈도우 뷰 포트를 설정합니다.
	 */
	static void SetWindowViewport();


	/**
	 * @brief 수직 동기화 옵션을 설정합니다.
	 *
	 * @param bIsEnable 수직 동기화 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	static void SetVsyncMode(bool bIsEnable);


	/**
	 * @brief 깊이 테스트 옵션을 설정합니다.
	 *
	 * @param bIsEnable 깊이 테스트 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	static void SetDepthMode(bool bIsEnable);


	/**
	 * @brief 스텐실 테스트 옵션을 설정합니다.
	 *
	 * @param bIsEnable 스텐실 테스트 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	static void SetStencilMode(bool bIsEnable);


	/**
	 * @brief 알파 블랜딩 옵션을 설정합니다.
	 *
	 * @param bIsEnable 알파 블랜딩 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	static void SetAlphaBlendMode(bool bIsEnable);


	/**
	 * @brief 멀티 샘플링 옵션을 설정합니다.
	 *
	 * @param bIsEnable 멀티 샘플링 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	static void SetMultisampleMode(bool bIsEnable);


	/**
	 * @brief 셰이더의 포인트 크기 옵션을 설정합니다.
	 *
	 * @param bIsEnabel 포인트 크기 옵션을 활성화하려면 true, 그렇지 않으면 false입니다.
	 */
	static void SetPointSizeMode(bool bIsEnable);


	/**
	 * @brief 리소스를 생성합니다.
	 *
	 * @param args 리소스의 생성자에 전달한 인자들입니다.
	 *
	 * @return 생성된 리소스의 포인터를 반환합니다. 리소스 생성에 실패하면 nullptr를 반환합니다.
	 */
	template <typename TResource, typename... Args>
	static TResource* CreateResource(Args&&... args)
	{
		if (!(0 <= cacheSize && cacheSize < MAX_RESOURCE_SIZE))
		{
			return nullptr;
		}

		int32_t resourceID = -1;
		for (uint32_t index = 0; index < cacheSize; ++index)
		{
			if (!cache[index] && !usage[index])
			{
				resourceID = static_cast<int32_t>(index);
				break;
			}
		}

		if (resourceID == -1)
		{
			resourceID = cacheSize++;
		}

		usage[resourceID] = true;
		cache[resourceID] = std::make_unique<TResource>(args...);
		
		return reinterpret_cast<TResource*>(cache[resourceID].get());
	}


	/**
	 * @brief 리소스 매니저가 관리하는 리소스를 삭제합니다.
	 *
	 * @param resource 삭제할 리소스 포인터입니다.
	 */
	static void DestroyResource(const IResource* resource)
	{
		int32_t resourceID = -1;
		for (uint32_t index = 0; index < cacheSize; ++index)
		{
			IResource* cacheResource = cache[index].get();
			if (resource == cacheResource)
			{
				resourceID = static_cast<int32_t>(index);
				break;
			}
		}

		if (resourceID != -1 && cache[resourceID])
		{
			if (cache[resourceID]->IsInitialized())
			{
				cache[resourceID]->Release();
			}

			usage[resourceID] = false;
			cache[resourceID].reset();
		}
	}


	/**
	 * @brief 스크린 크기를 얻습니다.
	 *
	 * @param outWidth 스크린의 가로 크기입니다.
	 * @param outHeight 스크린의 세로 크기입니다.
	 *
	 * @return 스크린 크기를 얻는 데 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors GetScreenSize(int32_t& outWidth, int32_t& outHeight);


	/**
	 * @brief 스크린 크기에 대응하는 투영 행렬을 얻습니다.
	 * 
	 * @param nearZ 투영 행렬에서 가까운 Z값입니다.
	 * @param farZ 투영 행렬에서 먼 Z값입니다.
	 * 
	 * @return 스크린 크기에 대응하는 투영 행렬을 반환합니다.
	 */
	static Mat4x4 GetScreenOrtho(float nearZ = -1.0f, float farZ = 1.0f);


	/**
	 * @brief 렌더 모듈의 가장 최근 에러 메시지를 얻습니다.
	 *
	 * @return 렌더 모듈의 가장 최근에 발생한 에러 메시지의 포인터 값을 반환합니다.
	 */
	static const wchar_t* GetLastErrorMessage();


	/**
	 * @brief 파이프라인에 바인딩된 리소스를 기반으로 그리기를 수행합니다.
	 * 
	 * @param vertexCount 그리기 수행 시 참조할 버텍스 버퍼의 수입니다.
	 * @param drawMode 그리기 모드입니다.
	 */
	static void ExecuteDrawVertex(uint32_t vertexCount, const EDrawMode& drawMode);


	/**
	 * @brief 파이프라인에 바인딩된 리소스를 기반으로 그리기를 수행합니다.
	 *
	 * @param vertexCount 그리기 수행 시 참조할 버텍스 버퍼의 수입니다.
	 * @param drawMode 그리기 모드입니다.
	 * @param instanceCount 인스턴스 카운트입니다.
	 */
	static void ExecuteDrawVertexInstanced(uint32_t vertexCount, const EDrawMode& drawMode, uint32_t instanceCounts);


	/**
	 * @brief 파이프라인에 바인딩된 리소스를 기반으로 그리기를 수행합니다.
	 * 
	 * @param indexCount 그리기 수행 시 참조할 인덱스 버퍼의 수입니다.
	 * @param drawMode 그리기 모드입니다.
	 */
	static void ExecuteDrawIndex(uint32_t indexCount, const EDrawMode& drawMode);


	/**
	 * @brief 파이프라인에 바인딩된 리소스를 기반으로 그리기를 수행합니다.
	 *
	 * @param indexCount 그리기 수행 시 참조할 인덱스 버퍼의 수입니다.
	 * @param drawMode 그리기 모드입니다.
	 * @param instanceCount 인스턴스 카운트입니다.
	 */
	static void ExecuteDrawIndexInstanced(uint32_t indexCount, const EDrawMode& drawMode, uint32_t instanceCounts);


private:
	/**
	 * @brief 에러 메시지를 설정합니다.
	 *
	 * @param message 설정할 메시지입니다.
	 */
	static void SetLastErrorMessage(const std::wstring& message);


	/**
	 * @brief Windows에러 메시지를 설정합니다.
	 *
	 * @note 에러가 존재하지 않으면 아무 동작도 수행하지 않습니다.
	 */
	static void SetLastWindowsErrorMessage();


	/**
	 * @brief Windows확장 OpenGL을 초기화합니다.
	 *
	 * @return Windows확장 OpenGL 초기화에 성공하면 true, 그렇지 않으면 false를 반환합니다.
	 */
	static Errors SetupWGLExtensions();


	/**
	 * @brief Windows GL 컨텍스트를 생성합니다.
	 *
	 * @return Windows GL 컨텍스트 생성에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors CreateWGLContext();


private:
	/**
	 * @brief 렌더 모듈이 초기화된 적이 있는지 확인합니다.
	 */
	static bool bIsInit;


	/**
	 * @brief 렌더링 대상이 되는 윈도우 핸들입니다.
	 */
	static HWND renderTargetHandle;


	/**
	 * @brief 렌더링이 수행될 디바이스 컨텍스트의 핸들입니다.
	 */
	static HDC deviceContext;


	/**
	 * @brief 렌더링이 수행될 OpenGL 컨텍스트의 핸들입니다.
	 */
	static HGLRC glRenderContext;


	/**
	 * @brief 스크린에 대응하는 직교 투영 행렬입니다.
	 */
	static Mat4x4 ortho;


	/**
	 * @brief 문자열 버퍼의 최대 크기입니다.
	 */
	static const uint32_t MAX_BUFFER_SIZE = 1024;


	/**
	 * @brief 최근 에러 메시지입니다.
	 */
	static wchar_t lastErrorMessage[MAX_BUFFER_SIZE];

	
	/**
	 * @brief 생성할 수 있는 최대 리소스 수입니다.
	 */
	static const uint32_t MAX_RESOURCE_SIZE = 200;


	/**
	 * @brief 캐시 크기입니다.
	 */
	static uint32_t cacheSize;


	/**
	 * @brief 리소스의 캐시입니다.
	 */
	static std::array<std::unique_ptr<IResource>, MAX_RESOURCE_SIZE> cache;


	/**
	 * @brief 리소스의 사용 여부입니다.
	 */
	static std::array<bool, MAX_RESOURCE_SIZE> usage;
};