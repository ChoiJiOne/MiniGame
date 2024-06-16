#pragma once

#include <vector>

#include "Collision.h"
#include "Mat.h"
#include "Transform.h"
#include "Vec.h"

#include "IEntity.h"

class StaticMesh;
class ITexture;


/**
 * @brief 바닥 엔티티입니다.
 */
class Floor : public IEntity
{
public:
	/**
	 * @brief 바닥 엔티티의 생성자입니다.
	 */
	Floor();


	/**
	 * @brief 바닥 엔티티의 가상 소멸자입니다.
	 */
	virtual ~Floor();


	/**
	 * @brief 바닥 엔티티의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Floor);


	/**
	 * @brief 바닥 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds);


	/**
	 * @brief 바닥 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release();


	/**
	 * @brief 바닥 엔티티의 정적 메시를 얻습니다.
	 *
	 * @return 바닥 엔티티의 정적 메시를 반환합니다.
	 */
	const std::vector<StaticMesh*>& GetMeshes() const { return meshes_; }


	/**
	 * @brief 바닥 엔티티의 트랜스폼을 얻습니다.
	 *
	 * @return 바닥 엔티티의 트랜스폼 참조자를 반환합니다.
	 */
	const Transform& GetTransform() const { return transform_; }


	/**
	 * @brief 바닥 엔티티의 메터리얼을 얻습니다.
	 *
	 * @return 바닥 엔티티의 메터리얼을 반환합니다.
	 */
	const ITexture* GetMaterial() const { return material_; }


private:
	/**
	 * @brief 바닥 엔티티의 정적 메시 목록입니다.
	 */
	std::vector<StaticMesh*> meshes_;


	/**
	 * @brief 바닥 엔티티의 트랜스폼입니다.
	 */
	Transform transform_;


	/**
	 * @brief 바닥 엔티티의 매터리얼입니다.
	 */
	ITexture* material_ = nullptr;
};