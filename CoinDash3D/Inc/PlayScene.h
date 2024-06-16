#pragma once

#include "IScene.h"

class Application; 
class DepthRenderer;
class GeometryRenderer2D;
class GeometryRenderer3D;
class MeshRenderer;
class TextRenderer;


/**
 * @brief 게임 플레이 씬입니다.
 */
class PlayScene : public IScene
{
public:
	/**
	 * @brief 게임 플레이 씬의 생성자입니다.
	 * 
	 * @param app 게임 플레이 씬이 참조할 애플리케이션 클래스입니다.
	 */
	PlayScene(Application* app);


	/**
	 * @brief 게임 플레이 씬의 가상 소멸자입니다.
	 */
	PlayScene() {}


	/**
	 * @brief 씬의 한 프레임을 수행합니다.
	 *
	 * @param deltaSeconds 초 단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 씬에 진입합니다.
	 */
	virtual void Enter() override;


	/**
	 * @brief 씬에서 나갑니다.
	 */
	virtual void Exit() override;


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
};