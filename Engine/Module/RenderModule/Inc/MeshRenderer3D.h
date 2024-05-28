#pragma once

#include <vector>

#include "Mat.h"

#include "IResource.h"
#include "ITexture2D.h"

class Shader;
class IMesh;


/**
 * @brief 3D 메시를 렌더링하는 렌더러입니다.
 */
class MeshRenderer3D : public IResource
{
public:
	/**
	 * @brief 3D 메시를 렌더링하는 렌더러의 생성자입니다.
	 */
	MeshRenderer3D();


	/**
	 * @brief 3D 메시를 렌더링하는 렌더러의 가상 소멸자입니다.
	 */
	virtual ~MeshRenderer3D();


	/**
	 * @brief 3D 메시를 렌더링하는 렌더러의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(MeshRenderer3D);


	/**
	 * @brief 3D 메시를 렌더링하는 렌더러의 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 렌더러의 뷰 행렬을 설정합니다.
	 *
	 * @param view 설정할 뷰 행렬입니다.
	 */
	void SetView(const Mat4x4& view) { view_ = view; }


	/**
	 * @brief 렌더러의 투영 행렬을 설정합니다.
	 *
	 * @param projection 설정할 투영 행렬입니다.
	 */
	void SetProjection(const Mat4x4& projection) { projection_ = projection; }

	
	/**
	 * @brief 정적 메시를 그립니다.
	 * 
	 * @param world 월드 행렬입니다.
	 * @param mesh 정적 메시 리소스입니다.
	 * @param material 정적 메시의 재질입니다.
	 */
	void DrawStaticMesh3D(const Mat4x4& world, IMesh* mesh, ITexture2D* material);


	/**
	 * @brief 스키닝 메시를 그립니다.
	 * 
	 * @param world 월드 행렬입니다.
	 * @param mesh 정적 메시 리소스입니다.
	 * @param bindPose 바인드 포즈 행렬 목록입니다.
	 * @param invBindPose 역 바인드 포즈 행렬 목록입니다.
	 * @param material 정적 메시의 재질입니다.
	 */
	void DrawSkinnedMesh3D(const Mat4x4& world, IMesh* mesh, const std::vector<Mat4x4>& bindPose, const std::vector<Mat4x4>& invBindPose, ITexture2D* material);


private:
	/**
	 * @brief 렌더러의 뷰 행렬입니다.
	 */
	Mat4x4 view_;


	/**
	 * @brief 렌더러의 투영 행렬입니다.
	 */
	Mat4x4 projection_;


	/**
	 * @brief 정적 메시를 렌더링하는 셰이더입니다.
	 */
	Shader* staticMeshShader_ = nullptr;


	/**
	 * @brief 스키닝 메시를 렌더링하는 셰이더입니다.
	 */
	Shader* skinnedMeshShader_ = nullptr;
};