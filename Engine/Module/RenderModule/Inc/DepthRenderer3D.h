#pragma once

#include <vector>

#include "Mat.h"

#include "IResource.h"

class Shader;
class IMesh;


/**
 * @brief 3D 메시의 깊이 값을 렌더링하는 렌더러입니다.
 */
class DepthRenderer3D : public IResource
{
public:
	/**
	 * @brief 3D 메시의 깊이 값을 렌더링하는 렌더러의 생성자입니다.
	 */
	DepthRenderer3D();


	/**
	 * @brief 3D 메시의 깊이 값을 렌더링하는 렌더러의 가상 소멸자입니다.
	 */
	virtual ~DepthRenderer3D();


	/**
	 * @brief 3D 메시의 깊이 값을 렌더링하는 렌더러의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(DepthRenderer3D);


	/**
	 * @brief 3D 메시의 깊이 값을 렌더링하는 렌더러의 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 렌더러의 라이트 뷰 행렬을 설정합니다.
	 *
	 * @param lightView 설정할 뷰 행렬입니다.
	 */
	void SetLightView(const Mat4x4& lightView) { lightView_ = lightView; }


	/**
	 * @brief 렌더러의 라이트 투영 행렬을 설정합니다.
	 *
	 * @param lightProjection 설정할 투영 행렬입니다.
	 */
	void SetLightProjection(const Mat4x4& lightProjection) { lightProjection_ = lightProjection; }


	/**
	 * @brief 정적 메시를 그립니다.
	 *
	 * @param world 월드 행렬입니다.
	 * @param mesh 정적 메시 리소스입니다.
	 */
	void DrawStaticMesh3D(const Mat4x4& world, IMesh* mesh);


	/**
	 * @brief 스키닝 메시를 그립니다.
	 *
	 * @param world 월드 행렬입니다.
	 * @param mesh 정적 메시 리소스입니다.
	 * @param bindPose 바인드 포즈 행렬 목록입니다.
	 * @param invBindPose 역 바인드 포즈 행렬 목록입니다.
	 */
	void DrawSkinnedMesh3D(const Mat4x4& world, IMesh* mesh, const std::vector<Mat4x4>& bindPose, const std::vector<Mat4x4>& invBindPose);


private:
	/**
	 * @brief 라이트 시점의 시야 행렬입니다.
	 */
	Mat4x4 lightView_;


	/**
	 * @brief 라이트 시점의 투영 행렬입니다.
	 */
	Mat4x4 lightProjection_;


	/**
	 * @brief 정적 메시를 렌더링하는 셰이더입니다.
	 */
	Shader* staticMeshDepth_ = nullptr;


	/**
	 * @brief 스키닝 메시를 렌더링하는 셰이더입니다.
	 */
	Shader* skinnedMeshDepth_ = nullptr;
};