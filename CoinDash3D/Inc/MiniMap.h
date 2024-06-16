#pragma once

#include <list>

#include "Entity2D.h"

class Character;
class Coin;
class GeometryRenderer2D;


/**
 * @brief 월드 내의 위치를 표시하는 미니맵 엔티티입니다.
 */
class MiniMap : public Entity2D
{
public:
	/**
	 * @brief 미니맵의 생성자입니다.
	 * 
	 * @param coins 미니맵의 코인 목록 참조자입니다.
	 * @param character 미니맵의 캐릭터입니다.
	 * @param renderer 미니맵 렌더링 시 참조할 2D 렌더러입니다.
	 */
	MiniMap(std::list<Coin*>& coins, Character* character, GeometryRenderer2D* renderer);


	/**
	 * @brief 미니맵의 가상 소멸자입니다.
	 */
	virtual ~MiniMap();


	/**
	 * @brief 미니맵의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(MiniMap);


	/**
	 * @brief 미니맵 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 미니맵을 화면에 그립니다.
	 */
	virtual void Render() override;


	/**
	 * @brief 코인을 생성하는 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


private:
	/**
	 * @brief 3D위치를 2D 미니맵 상의 위치로 변환합니다.
	 * 
	 * @param position3D 변환할 3D 위치입니다.
	 * 
	 * @return 변환된 2D 미니맵 상의 위치를 반환합니다.
	 */
	Vec2f Convert3DTo2D(const Vec3f& position3D);


private:
	/**
	 * @brief 미니맵의 코인 목록 참조자입니다.
	 */
	std::list<Coin*>& coins_;


	/**
	 * @brief 미니맵의 캐릭터입니다.
	 */
	Character* character_ = nullptr;


	/**
	 * @brief 미니맵 렌더링 시 참조할 2D 렌더러입니다.
	 */
	GeometryRenderer2D* renderer_ = nullptr;


	/**
	 * @brief 미니맵의 위치입니다.
	 */
	Vec2f center_;


	/**
	 * @brief 미니맵의 크기입니다.
	 * 
	 * @note 미니맵은 정사각형입니다.
	 */
	float size_;


	/**
	 * @brief 미니맵의 백그라운드 색상입니다.
	 */
	Vec4f background_;


	/**
	 * @brief 코인의 색상입니다.
	 */
	Vec4f coinColor_;


	/**
	 * @brief 플레이어의 색상입니다.
	 */
	Vec4f characterColor_;


	/**
	 * @brief 월드의 최대 좌표 값입니다.
	 */
	float maxPosition_ = 0.0f;
};
