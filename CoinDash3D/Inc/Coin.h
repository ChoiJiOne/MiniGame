#pragma once

#include <vector>

#include "Collision.h"
#include "Mat.h"
#include "Transform.h"
#include "Vec.h"

#include "IEntity.h"

class StaticMesh;
class ITexture2D;


/**
 * @brief 코인 엔티티입니다.
 */
class Coin : public IEntity
{
public:
	/**
	 * @brief 코인 엔티티의 생성자입니다.
	 */
	Coin();


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
	virtual void Tick(float deltaSeconds);


	/**
	 * @brief 코인 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release();


	/**
	 * @brief 코인 엔티티의 정적 메시를 얻습니다.
	 *
	 * @return 코인 엔티티의 정적 메시를 반환합니다.
	 */
	StaticMesh* GetMesh() { return mesh_; }


	/**
	 * @brief 코인 엔티티의 트랜스폼을 얻습니다.
	 *
	 * @return 코인 엔티티의 트랜스폼 참조자를 반환합니다.
	 */
	Transform& GetTransform() { return transform_; }


	/**
	 * @brief 코인 엔티티의 메터리얼을 얻습니다.
	 *
	 * @return 코인 엔티티의 메터리얼을 반환합니다.
	 */
	ITexture2D* GetMaterial() { return material_; }


private:
	/**
	 * @brief 코인 엔티티의 정적 메시 목록입니다.
	 */
	StaticMesh* mesh_;


	/**
	 * @brief 코인 엔티티의 트랜스폼입니다.
	 */
	Transform transform_;


	/**
	 * @brief 코인 엔티티의 매터리얼입니다.
	 */
	ITexture2D* material_ = nullptr;
};