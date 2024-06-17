#pragma once

#include "IScene.h"

class Application;
class GeometryRenderer2D;
class TextRenderer;

class Button;
class Panel;


/**
 * @brief 게임 시작 씬입니다.
 */
class StartScene : public IScene
{
public:
	/**
	 * @brief 게임 시작 씬의 생성자입니다.
	 *
	 * @param app 게임 플레이 씬이 참조할 애플리케이션 클래스입니다.
	 */
	StartScene(Application* app);


	/**
	 * @brief 게임 시작 씬의 가상 소멸자입니다.
	 */
	StartScene() {}


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
	 * @brief 씬 내의 엔티티를 업데이트합니다.
	 * 
	 * @param deltaSeconds 초 단위 델타 시간값입니다.
	 */
	void Update(float deltaSeconds);


	/**
	 * @brief 렌더링 준비를 합니다.
	 */
	void PrepareForRendering();


	/**
	 * @brief 씬 내의 엔티티를 렌더링합니다.
	 */
	void Render();


private:
	/**
	 * @brief 2D 기하 도형을 렌더링하는 렌더러입니다.
	 */
	GeometryRenderer2D* geometryRenderer2D_ = nullptr;


	/**
	 * @brief 2D 텍스트를 렌더링하는 렌더러입니다.
	 */
	TextRenderer* textRenderer_ = nullptr;


	/**
	 * @brief 크기에 따른 폰트 리소스입니다.
	 */
	std::map<int32_t, TTFont*> fonts_;


	/**
	 * @brief 타이틀 패널입니다.
	 */
	Panel* titlePanel_ = nullptr;


	/**
	 * @brief 게임 시작 버튼입니다.
	 */
	Button* startButton_ = nullptr;


	/**
	 * @brief 게임 종료 버튼입니다.
	 */
	Button* quitButton_ = nullptr;
};