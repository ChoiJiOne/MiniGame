#pragma once

#include <vector>

#include "Collision.h"
#include "Mat.h"
#include "Transform.h"
#include "Vec.h"

#include "IEntity.h"

class Character;
class StaticMesh;
class ITexture;


/**
 * @brief 코인 엔티티입니다.
 */
class Coin : public IEntity
{
public:
	/**
	 * @brief 코인 엔티티의 생성자입니다.
	 *
	 * @param position 코인의 위치입니다.
	 * @param character 코인과 충돌 여부를 확인할 캐릭터 엔티티입니다.
	 */
	Coin(const Vec3f& position, Character* character);


	/**
	 * @brief 코인 엔티티의 가상 소멸자입니다.
	 */
	virtual ~Coin();


	/**
	 * @brief 코인 엔티티의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Coin);


	/**
	 * @brief 코인 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 코인 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 코인 엔티티의 정적 메시를 얻습니다.
	 *
	 * @return 코인 엔티티의 정적 메시를 반환합니다.
	 */
	const std::vector<StaticMesh*>& GetMeshes() const { return meshes_; }


	/**
	 * @brief 코인 엔티티의 경계 영역을 얻습니다.
	 * 
	 * @return 코인 엔티티의 경계 영역을 반환합니다.
	 */
	const AABB& GetAABB() const { return aabb_; }


	/**
	 * @brief 코인 엔티티의 트랜스폼을 얻습니다.
	 *
	 * @return 코인 엔티티의 트랜스폼 참조자를 반환합니다.
	 */
	const Transform& GetTransform() const { return transform_; }


	/**
	 * @brief 코인 엔티티의 메터리얼을 얻습니다.
	 *
	 * @return 코인 엔티티의 메터리얼을 반환합니다.
	 */
	const ITexture* GetMaterial() const { return material_; }


	/**
	 * @brief 플레이어가 이 코인을 소유하고 있는지 확인합니다.
	 * 
	 * @return 플레이어가 이 코인을 소유하고 있다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool HasCollectedCoin() const { return hasCollectedCoin_; }


private:
	/**
	 * @brief 코인 엔티티의 정적 메시 목록입니다.
	 */
	std::vector<StaticMesh*> meshes_;


	/**
	 * @brief 코인의 경계 영역입니다.
	 */
	AABB aabb_;


	/**
	 * @brief 코인 엔티티의 트랜스폼입니다.
	 */
	Transform transform_;


	/**
	 * @brief 코인 엔티티의 매터리얼입니다.
	 */
	ITexture* material_ = nullptr;


	/**
	 * @brief 코인과 충돌 여부를 확인할 캐릭터 엔티티입니다.
	 */
	Character* character_ = nullptr;


	/**
	 * @brief 플레이어가 코인을 가졌는지 확인합니다.
	 */
	bool hasCollectedCoin_ = false;
};