#pragma once

#include "Entity2D.h"

class Character;
class GeometryRenderer2D;
class TextRenderer;
class TTFont;


/**
 * @brief 현재 게임 상태를 표시하는 엔티티입니다.
 */
class StatusViewer : public Entity2D
{
public:
	/**
	 * @brief 현재 게임 상태를 표시하는 엔티티의 생성자입니다.
	 * 
	 * @param character 게임 상태 표시할 때 참조할 캐릭터입니다.
	 * @param geometryRenderer 게임 상태를 렌더링할 때 사용할 2D 렌더러입니다.
	 * @param textRenderer 게임 상태를 렌더링할 때 사용할 텍스트 렌더러입니다.
	 * @param font 텍스트 렌더링 시 참조할 폰트입니다.
	 */
	StatusViewer(Character* character, GeometryRenderer2D* geometryRenderer, TextRenderer* textRenderer, TTFont* font);


	/**
	 * @brief 현재 게임 상태를 표시하는 엔티티의 가상 소멸자입니다.
	 */
	virtual ~StatusViewer();


	/**
	 * @brief 현재 게임 상태를 표시하는 엔티티의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(StatusViewer);


	/**
	 * @brief 현재 게임 상태를 표시하는 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 현재 게임 상태를 표시하는 엔티티를 화면에 그립니다.
	 */
	virtual void Render() override;


	/**
	 * @brief 현재 게임 상태를 표시하는 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


private:
	/**
	 * @brief 게임 상태 표시할 때 참조할 캐릭터입니다.
	 */
	Character* character_ = nullptr;


	/**
	 * @brief 게임 상태를 렌더링할 때 사용할 2D 렌더러입니다.
	 */
	GeometryRenderer2D* geometryRenderer_ = nullptr;


	/**
	 * @brief 게임 상태를 렌더링할 때 사용할 텍스트 렌더러입니다.
	 */
	TextRenderer* textRenderer_ = nullptr;


	/**
	 * @brief 텍스트 렌더링 시 참조할 폰트입니다.
	 */
	TTFont* font_ = nullptr;


	/**
	 * @brief 백그라운드의 색상입니다.
	 */
	Vec4f backgroundColor_;


	/**
	 * @brief 백그라운드의 중심입니다.
	 */
	Vec2f center_;


	/**
	 * @brief 백그라운드의 가로 크기입니다.
	 */
	float width_ = 0.0f;


	/**
	 * @brief 백그라운드의 세로 크기입니다.
	 */
	float height_ = 0.0f;


	/**
	 * @brief 백그라운드의 둥근 부분의 크기입니다.
	 */
	float side_ = 10.0f;


	/**
	 * @brief 일반적인 텍스트 컬러입니다.
	 */
	Vec4f textColor_;


	/**
	 * @brief 현재 코인의 수입니다.
	 */
	int32_t currentCoinCount_ = 0;


	/**
	 * @brief 현재 코인의 수 텍스트입니다.
	 */
	std::wstring currentCoinCountText_;


	/**
	 * @brief 현재 코인의 수 텍스트 위치입니다.
	 */
	Vec2f currentCoinCountTextPosition_;


	/**
	 * @brief 현재 남은 시간입니다.
	 */
	int32_t currentRemainTime_ = 0.0f;


	/**
	 * @brief 현재 남은 시간 텍스트입니다.
	 */
	std::wstring currentRemainTimeText_;


	/**
	 * @brief 현재 남은 시간 텍스트의 위치입니다.
	 */
	Vec2f currentRemainTimeTextPosition_;


	/**
	 * @brief 카운드 다운 위치입니다.
	 */
	Vec2f countDownPosition_;


	/**
	 * @brief 카운트 다운의 가로 크기입니다.
	 */
	float countDownWidth_ = 0.0f;


	/**
	 * @brief 카운트 다운의 세로 크기입니다.
	 */
	float countDownHeight_ = 0.0f;


	/**
	 * @brief 카운트 다운의 백그라운드 색상입니다.
	 */
	Vec4f countDownBackgroundColor_;


	/**
	 * @brief 카운트 다운의 색상입니다.
	 */
	Vec4f countDownColor_;
};
