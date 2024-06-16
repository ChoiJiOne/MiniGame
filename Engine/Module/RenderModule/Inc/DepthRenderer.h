#pragma once

#include <vector>

#include "Mat.h"

#include "IResource.h"

class Shader;
class StaticMesh;
class SkinnedMesh;


/**
 * @brief 메시의 깊이 값을 렌더링하는 렌더러입니다.
 */
class DepthRenderer : public IResource
{
public:
	/**
	 * @brief 깊이 값을 렌더링하는 렌더러의 디폴트 생성자입니다.
	 */
	explicit DepthRenderer();


	/**
	 * @brief 깊이 값을 렌더링하는 렌더러의 가상 소멸자입니다.
	 *
	 * @note 렌더러 내부에서 할당된 요소를 해제하기 위해서는 반드시 Release를 호출해야 합니다.
	 */
	virtual ~DepthRenderer();


	/**
	 * @brief 깊이 값을 렌더링하는 렌더러의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(DepthRenderer);


	/**
	 * @brief 렌더러 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 라이트 시점의 행렬을 설정합니다.
	 *
	 * @param lightSpaceMatrix 설정할 라이트 시점의 행렬입니다.
	 */
	void SetLightSpaceMatrix(const Mat4x4& lightSpaceMatrix) { lightSpaceMatrix_ = lightSpaceMatrix; }


	/**
	 * @brief 정적 메시의 깊이 값을 렌더링합니다.
	 * 
	 * @param world 정적 메시의 월드 행렬입니다.
	 * @param mesh 정적 메시 리소스입니다.
	 */
	void DrawStaticMesh(const Mat4x4& world, const StaticMesh* mesh);


	/**
	 * @brief 스키닝 메시의 깊이 값을 렌더링합니다.
	 *
	 * @param world 정적 메시의 월드 행렬입니다.
	 * @param bindPose 바인드 포즈 행렬입니다.
	 * @param invBindPose 역 바인드 포즈 행렬입니다.
	 * @param mesh 스키닝 메시 리소스입니다.
	 */
	void DrawSkinnedMesh(const Mat4x4& world, const std::vector<Mat4x4>& bindPose, const std::vector<Mat4x4>& invBindPose, const SkinnedMesh* mesh);


private:
	/**
	 * @brief 라이트 시점의 행렬입니다.
	 */
	Mat4x4 lightSpaceMatrix_;


	/**
	 * @brief 정적 메시의 깊이 값을 렌더링할 셰이더입니다.
	 */
	Shader* staticMeshShader_ = nullptr;


	/**
	 * @brief 스키닝 메시의 깊이 값을 렌더링할 셰이더입니다.
	 */
	Shader* skinnedMeshShader_ = nullptr;
};