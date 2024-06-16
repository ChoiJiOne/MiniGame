#pragma once

#include "CrossFadeController.h"
#include "Collision.h"
#include "IEntity.h"
#include "ITexture.h"
#include "Skeleton.h"
#include "SkinnedMesh.h"


/**
 * @brief 캐릭터 엔티티입니다.
 */
class Character : public IEntity
{
public:
	/**
	 * @brief 캐릭터의 상태입니다.
	 */
	enum class EStatus
	{
		IDLE = 0x00, // 대기중
		RUN = 0x01, // 달리는 중
	};


public:
	/**
	 * @brief 캐릭터 엔티티의 생성자입니다.
	 */
	Character();


	/**
	 * @brief 캐릭터 엔티티의 가상 소멸자입니다.
	 */
	virtual ~Character();


	/**
	 * @brief 캐릭터 엔티티의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Character);


	/**
	 * @brief 캐릭터 엔티티를 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 캐릭터 엔티티 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 캐릭터의 메시 리소스 목록을 얻습니다.
	 *
	 * @return 캐릭터의 메시 리소스 목록 참조자를 반환합니다.
	 */
	const std::vector<SkinnedMesh*>& GetMeshes() const { return meshes_; }


	/**
	 * @brief 캐릭터의 메터리얼(재질)을 얻습니다.
	 *
	 * @return 캐릭터의 메터리얼(재질) 참조자를 반환합니다.
	 */
	const ITexture* GetMaterial() const { return material_; }


	/**
	 * @brief 캐릭터의 경계 영역을 얻습니다.
	 *
	 * @return 캐릭터의 경계 영역 참조자를 반환합니다.
	 */
	const Sphere& GetSphere() const { return sphere_; }


	/**
	 * @brief 캐릭터의 트랜스폼을 얻습니다.
	 *
	 * @return 캐릭터의 트랜스폼 참조자를 반환합니다.
	 */
	const Transform& GetTransform() const { return transform_; }


	/**
	 * @brief 캐릭터의 크로스 페이드 컨트롤러를 얻습니다.
	 *
	 * @return 캐릭터의 크로스 페이드 컨트롤러 참조자를 반환합니다.
	 */
	CrossFadeController& GetCrossFadeController() { return crossFadeController_; }


	/**
	 * @brief 캐릭터의 바인드 포즈 행렬을 얻습니다.
	 * 
	 * @return 캐릭터의 바인드 포즈 행렬을 반환합니다.
	 */
	const std::vector<Mat4x4>& GetBindPose() const { return bindPose_; }


	/**
	 * @brief 캐릭터의 역 바인드 포즈 행렬을 얻습니다.
	 *
	 * @return 캐릭터의 역 바인드 포즈 행렬을 반환합니다.
	 */
	const std::vector<Mat4x4>& GetInvBindPose() { return crossFadeController_.GetSkeleton().GetInvBindPose(); }


private:
	/**
	 * @brief 캐릭터의 이동 방향을 얻습니다.
	 *
	 * @return 캐릭터의 이동 방향을 반환합니다.
	 */
	Vec3f GetMoveDirection();


private:
	/**
	 * @brief 스키닝 메시 리소스입니다.
	 */
	std::vector<SkinnedMesh*> meshes_;


	/**
	 * @brief 캐릭터의 메터리얼입니다.
	 */
	ITexture* material_ = nullptr;


	/**
	 * @brief 캐릭터의 경계 영역입니다.
	 */
	Sphere sphere_;


	/**
	 * @brief 캐릭터의 트랜스폼입니다.
	 */
	Transform transform_;


	/**
	 * @brief 대기 애니메이션 클립 인덱스입니다.
	 */
	uint32_t idleClip_ = 0;


	/**
	 * @brief 달리기 애니메이션 클립 인덱스입니다.
	 */
	uint32_t runClip_ = 0;


	/**
	 * @brief 애니메이션 클립입니다.
	 */
	std::vector<Clip> clips_;


	/**
	 * @brief 캐릭터 애니메이션의 뼈대입니다.
	 */
	Skeleton skeleton_;


	/**
	 * @brief 클립의 부드러운 전환을 위한 크로스 페이드 컨트롤러입니다.
	 */
	CrossFadeController crossFadeController_;


	/**
	 * @brief 바인드 포즈 행렬입니다.
	 */
	std::vector<Mat4x4> bindPose_;


	/**
	 * @brief 캐릭터의 현재 상태입니다.
	 */
	EStatus currentStatus_ = EStatus::IDLE;


	/**
	 * @brief 캐릭터의 이동 속도입니다.
	 */
	float moveSpeed_ = 0.0f;
};