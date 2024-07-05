#pragma once

#include "IManager.h"


namespace GameMaker
{
class Renderer2D;
class Renderer3D;


/**
 * @brief 기본 도형의 그리기 모드입니다.
 */
enum class EDrawMode : int32_t
{
	POINTS       = 0x0000,
	LINES        = 0x0001,
	LINE_STRIP   = 0x0003,
	TRIANGLES    = 0x0004,
	TRIANGLE_FAN = 0x0006,
	NONE         = 0xFFFF,
};


/**
 * @brief 렌더링 관련 처리를 담당하는 매니저입니다.
 * 
 * @note 이 클래스는 싱글턴입니다.
 */
class RenderManager : public IManager
{
public:
	/**
	 * @brief 렌더 매니저의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(RenderManager);


	/**
	 * @brief 렌더 매니저의 참조자를 얻습니다.
	 *
	 * @return 렌더 매니저의 참조자를 반환합니다.
	 */
	static RenderManager& Get();


	/**
	 * @brief 렌더 매니저의 사용을 시작합니다.
	 *
	 * @note 이 메서드는 한 번만 호출되어야 합니다.
	 */
	virtual void Startup() override;


	/**
	 * @brief 렌더 매니저의 사용을 종료합니다.
	 *
	 * @note
	 * - 애플리케이션 종료 전에 이 메서드를 반드시 호출하여 내부 리소스를 정리해야 합니다.
	 * - 이 메서드는 반드시 한 번만 호출되어야 합니다.
	 */
	virtual void Shutdown() override;


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
	void BeginFrame(float red, float green, float blue, float alpha, float depth = 1.0f, uint8_t stencil = 0);


	/**
	 * @brief 백버퍼와 프론트 버퍼를 스왑합니다.
	 *
	 * @return 백버퍼와 프론트 버퍼의 스왑에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	void EndFrame();


	/**
	 * @brief 뷰 포트를 설정합니다.
	 *
	 * @param x 뷰포트 사각형의 왼쪽 아래 모서리(픽셀)입니다. 기본값은 (0,0)입니다.
	 * @param y 뷰포트 사각형의 왼쪽 아래 모서리(픽셀)입니다. 기본값은 (0,0)입니다.
	 * @param width 뷰포트의 너비입니다. OpenGL 컨텍스트가 창에 처음 연결되면 너비 와 높이 가 해당 창의 크기로 설정됩니다.
	 * @param height 뷰포트의 높이입니다. OpenGL 컨텍스트가 창에 처음 연결되면 너비 와 높이 가 해당 창의 크기로 설정됩니다.
	 */
	void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);


	/**
	 * @brief 윈도우 뷰 포트를 설정합니다.
	 */
	void SetWindowViewport();


	/**
	 * @brief 수직 동기화 옵션을 설정합니다.
	 *
	 * @param bIsEnable 수직 동기화 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	void SetVsyncMode(bool bIsEnable);


	/**
	 * @brief 깊이 테스트 옵션을 설정합니다.
	 *
	 * @param bIsEnable 깊이 테스트 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	void SetDepthMode(bool bIsEnable);


	/**
	 * @brief 스텐실 테스트 옵션을 설정합니다.
	 *
	 * @param bIsEnable 스텐실 테스트 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	void SetStencilMode(bool bIsEnable);


	/**
	 * @brief 알파 블랜딩 옵션을 설정합니다.
	 *
	 * @param bIsEnable 알파 블랜딩 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	void SetAlphaBlendMode(bool bIsEnable);


	/**
	 * @brief 멀티 샘플링 옵션을 설정합니다.
	 *
	 * @param bIsEnable 멀티 샘플링 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	void SetMultisampleMode(bool bIsEnable);


	/**
	 * @brief 셰이더의 포인트 크기 옵션을 설정합니다.
	 *
	 * @param bIsEnable 포인트 크기 옵션을 활성화하려면 true, 그렇지 않으면 false입니다.
	 */
	void SetPointSizeMode(bool bIsEnable);


	/**
	 * @brief 페이스 컬링 모드 옵션을 설정합니다.
	 *
	 * @param bIsEnable 페이스 컬링 모드를 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	void SetCullFaceMode(bool bIsEnable);


	/**
	 * @brief 파이프라인에 바인딩된 리소스를 기반으로 그리기를 수행합니다.
	 *
	 * @param vertexCount 그리기 수행 시 참조할 버텍스 버퍼의 수입니다.
	 * @param drawMode 그리기 모드입니다.
	 */
	void ExecuteDrawVertex(uint32_t vertexCount, const EDrawMode& drawMode);


	/**
	 * @brief 파이프라인에 바인딩된 리소스를 기반으로 그리기를 수행합니다.
	 *
	 * @param vertexCount 그리기 수행 시 참조할 버텍스 버퍼의 수입니다.
	 * @param drawMode 그리기 모드입니다.
	 * @param instanceCount 인스턴스 카운트입니다.
	 */
	void ExecuteDrawVertexInstanced(uint32_t vertexCount, const EDrawMode& drawMode, uint32_t instanceCounts);


	/**
	 * @brief 파이프라인에 바인딩된 리소스를 기반으로 그리기를 수행합니다.
	 *
	 * @param indexCount 그리기 수행 시 참조할 인덱스 버퍼의 수입니다.
	 * @param drawMode 그리기 모드입니다.
	 */
	void ExecuteDrawIndex(uint32_t indexCount, const EDrawMode& drawMode);


	/**
	 * @brief 파이프라인에 바인딩된 리소스를 기반으로 그리기를 수행합니다.
	 *
	 * @param indexCount 그리기 수행 시 참조할 인덱스 버퍼의 수입니다.
	 * @param drawMode 그리기 모드입니다.
	 * @param instanceCount 인스턴스 카운트입니다.
	 */
	void ExecuteDrawIndexInstanced(uint32_t indexCount, const EDrawMode& drawMode, uint32_t instanceCounts);


	/**
	 * @brief 스크린의 크기를 얻습니다.
	 * 
	 * @param outWidth 스크린의 가로 크기입니다.
	 * @param outHeight 스크린의 세로 크기입니다.
	 */
	template <typename T>
	void GetScreenSize(T& outWidth, T& outHeight);


private:
	/**
	 * @brief 렌더 매니저에 디폴트 생성자와 빈 가상 소멸자를 삽입합니다.
	 */
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(RenderManager);


private:
	/**
	 * @brief 렌더링 대상이 되는 윈도우 포인터입니다.
	 */
	void* window_ = nullptr;
	

	/**
	 * @brief OpenGL 컨텍스트입니다.
	 */
	void* context_ = nullptr;
};

}