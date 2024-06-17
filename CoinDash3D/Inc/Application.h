#pragma once

#include <array>
#include <map>
#include <memory>

#include "AudioModule.h"
#include "CrashModule.h"
#include "FileModule.h"
#include "GameModule.h"
#include "MathModule.h"
#include "PlatformModule.h"
#include "RenderModule.h"

#include "IScene.h"

class DepthRenderer;
class GeometryRenderer2D;
class GeometryRenderer3D;
class MeshRenderer;
class ShadowMap;
class TextRenderer;
class TTFont;


/**
 * @brief 어플리케이션을 초기화/실행/해제하는 클래스입니다.
 */
class Application
{
public:
	/**
	 * @brief 어플리케이션의 생성자입니다.
	 */
	Application();


	/**
	 * @brief 어플리케이션의 가상 소멸자입니다.
	 */
	virtual ~Application();


	/**
	 * @brief 어플레케이션의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Application);


	/**
	 * @brief 어플리케이션을 초기화합니다.
	 */
	void Init();


	/**
	 * @brief 어플리케이션을 실행합니다.
	 */
	void Run();


private:
	/**
	 * @brief 씬 내부에서 Application의 내부에 접근할 수 있도록 선언합니다.
	 */
	friend class IScene;
	friend class PlayScene;
	friend class StartScene;


private:
	/**
	 * @brief 3D 씬의 깊이 값을 렌더링하는 렌더러입니다.
	 */
	DepthRenderer* depthRenderer_ = nullptr;


	/**
	 * @brief 2D 기하 도형을 렌더링하는 렌더러입니다.
	 */
	GeometryRenderer2D* geometryRenderer2D_ = nullptr;


	/**
	 * @brief 3D 기하 도형을 렌더링하는 렌더러입니다.
	 */
	GeometryRenderer3D* geometryRenderer3D_ = nullptr;


	/**
	 * @brief 3D 메쉬를 렌더링하는 렌더러입니다.
	 */
	MeshRenderer* meshRenderer_ = nullptr;


	/**
	 * @brief 2D 텍스트를 렌더링하는 렌더러입니다.
	 */
	TextRenderer* textRenderer_ = nullptr;


	/**
	 * @brief 셰도우 맵입니다.
	 */
	ShadowMap* shadowMap_ = nullptr;


	/**
	 * @brief 크기에 따른 폰트 리소스입니다.
	 */
	std::map<int32_t, TTFont*> fonts_;


	/**
	 * @brief 시작 씬입니다.
	 */
	std::unique_ptr<IScene> startScene_ = nullptr;

		
	/**
	 * @brief 플레이 씬입니다.
	 */
	std::unique_ptr<IScene> playScene_ = nullptr;


	/**
	 * @brief 현재 씬입니다.
	 */
	IScene* currentScene_ = nullptr;
};