#pragma once

#include <list>

#include "IEntity.h"

class Character;
class Coin;


/**
 * @brief 코인을 생성하는 엔티티입니다.
 */
class CoinSpawner : public IEntity
{
public:
	/**
	 * @brief 코인을 생성하는 엔티티의 생성자입니다.
	 *
	 * @param coins 코인 목록의 참조자입니다.
	 * @param character 코인이 참조할 캐릭터 엔티티입니다.
	 */
	CoinSpawner(std::list<Coin*>& coins, Character* character);


	/**
	 * @brief 코인을 생성하는 엔티티의 가상 소멸자입니다.
	 */
	virtual ~CoinSpawner();


	/**
	 * @brief 코인을 생성하는 엔티티의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(CoinSpawner);


	/**
	 * @brief 코인을 생성하는 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds);


	/**
	 * @brief 코인을 생성하는 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release();


private:
	/**
	 * @brief 코인을 생성합니다.
	 */
	void GenerateCoins();


private:
	/**
	 * @brief 코인 목록의 참조자입니다.
	 */
	std::list<Coin*>& coins_;


	/**
	 * @brief 코인이 참조할 캐릭터 엔티티입니다.
	 */
	Character* character_ = nullptr;


	/**
	 * @brief 최대 코인 생성 수입니다.
	 */
	uint32_t maxCoin_ = 10;
};