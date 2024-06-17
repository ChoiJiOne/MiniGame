#pragma once

#include <list>

#include "IScene.h"

class Application; 
class DepthRenderer;
class GeometryRenderer2D;
class GeometryRenderer3D;
class MeshRenderer;
class PostEffectComposer;
class ShadowMap;
class TextRenderer;

class Button;
class Camera;
class Character;
class Coin;
class CoinSpawner;
class Floor;
class Light;
class MiniMap;
class StatusViewer;


/**
 * @brief 게임 플레이 씬입니다.
 */
class PlayScene : public IScene
{
public:
	/**
	 * @brief 게임 플레이 씬의 상태입니다.
	 */
	enum class EStatus
	{
		READY = 0x00,
		PLAY  = 0x01,
		PAUSE = 0x02,
		DONE  = 0x03,
	};


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


	/**
	 * @brief 시작 씬의 포인터를 설정합니다.
	 * 
	 * @param startScene 설정할 시작 씬의 포인터입니다.
	 */
	void SetStartScene(IScene* startScene) { startScene_ = startScene; }


private:
	/**
	 * @brief 전체 로직을 업데이트합니다.
	 * 
	 * @param deltaSeconds 초 단위 델타 시간값입니다.
	 */
	void Update(float deltaSeconds);


	/**
	 * @brief 렌더링 준비를 합니다.
	 */
	void PrepareForRendering();


	/**
	 * @brief 씬 렌더링을 수행합니다.
	 */
	void Render();


	/**
	 * @brief 깊이 씬 렌더링을 수행합니다.
	 */
	void DepthPass();


	/**
	 * @brief 씬 렌더링을 수행합니다.
	 */
	void RenderPass();


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
	 * @brief 후처리 효과(Grayscale, Blur, Fade 등등)를 적용할 때 사용합니다.
	 */
	PostEffectComposer* postEffectComposer_ = nullptr;


	/**
	 * @brief 2D 텍스트를 렌더링하는 렌더러입니다.
	 */
	TextRenderer* textRenderer_ = nullptr;


	/**
	 * @brief 깊이 값을 렌더링할 셰도우 맵입니다.
	 */
	ShadowMap* shadowMap_ = nullptr;


	/**
	 * @brief 프레임 버퍼입니다.
	 */
	FrameBuffer* framebuffer_ = nullptr;


	/**
	 * @brief 크기에 따른 폰트 리소스입니다.
	 */
	std::map<int32_t, TTFont*> fonts_;


	/**
	 * @brief 카메라 엔티티입니다.
	 */
	Camera* camera_ = nullptr;


	/**
	 * @brief 플레이어가 조종하는 캐릭터입니다.
	 */
	Character* character_ = nullptr;


	/**
	 * @brief 바닥 엔티티입니다.
	 */
	Floor* floor_ = nullptr;


	/**
	 * @brief 라이트 엔티티입니다.
	 */
	Light* light_ = nullptr;


	/**
	 * @brief 코인 엔티티입니다.
	 */
	std::list<Coin*> coins_;


	/**
	 * @brief 코인을 생성하는 엔티티입니다.
	 */
	CoinSpawner* coinSpawner_ = nullptr;


	/**
	 * @brief 미니맵 엔티티입니다.
	 */
	MiniMap* miniMap_ = nullptr;


	/**
	 * @brief 현재 상태를 표시하는 엔티티입니다.
	 */
	StatusViewer* statusViewer_ = nullptr;


	/**
	 * @brief 게임 리셋 버튼입니다.
	 */
	Button* resetButton_ = nullptr;


	/**
	 * @brief 게임 종료 버튼입니다.
	 */
	Button* quitButton_ = nullptr;


	/**
	 * @brief 시작 씬입니다.
	 */
	IScene* startScene_ = nullptr;


	/**
	 * @brief 현재 게임 플레이 씬의 상태입니다.
	 */
	EStatus status_ = EStatus::READY;
};