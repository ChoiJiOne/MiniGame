#pragma once

#include <vector>

#include "Mat.h"

#include "IResource.h"

class ITexture;
class Shader;
class StaticMesh;
class SkinnedMesh;
class ShadowMap;


/**
 * @brief 메시를 렌더링하는 렌더러입니다.
 */
class MeshRenderer : public IResource
{
public:
	/**
	 * @brief 메쉬를 그리는 렌더러의 디폴트 생성자입니다.
	 */
	explicit MeshRenderer();


	/**
	 * @brief 메쉬를 그리는 렌더러의 가상 소멸자입니다.
	 *
	 * @note 렌더러 내부에서 할당된 요소를 해제하기 위해서는 반드시 Release를 호출해야 합니다.
	 */
	virtual ~MeshRenderer();


	/**
	 * @brief 메쉬를 그리는 렌더러의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(MeshRenderer);


	/**
	 * @brief 렌더러 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 렌더러의 파라미터를 설정합니다.
	 *
	 * @param view 설정할 뷰 행렬입니다.
	 */
	void SetView(const Mat4x4& view) { view_ = view; }


	/**
	 * @brief 렌더러의 원근 투영 행렬을 설정합니다.
	 *
	 * @param projection 설정할 투영 행렬입니다.
	 */
	void SetProjection(const Mat4x4& projection) { projection_ = projection; }


	/**
	 * @brief 렌더러의 셰도우 맵을 설정합니다.
	 * 
	 * @param shadowMap 설정할 셰도우 맵입니다.
	 */
	void SetShadowMap(ShadowMap* shadowMap) { shadowMap_ = shadowMap; }


	/**
	 * @brief 렌더러의 라이트 위치 값을 설정합니다.
	 * 
	 * @param lightPosition 설정할 라이트 위치 값입니다.
	 */
	void SetLightPosition(const Vec3f& lightPosition) { lightPosition_ = lightPosition; }


	/**
	 * @brief 렌더러의 라이트 시점 행렬을 설정합니다.
	 * 
	 * @param lightSpaceMatrix 설정할 라이트 시점 행렬 값입니다.
	 */
	void SetLightSpaceMatrix(Mat4x4 lightSpaceMatrix) { lightSpaceMatrix_ = lightSpaceMatrix; }


	/**
	 * @brief 렌더러의 카메라 위치 값을 설정합니다.
	 * 
	 * @param cameraPosition 설정할 카메라 위치 값입니다.
	 */
	void SetCameraPosition(const Vec3f& cameraPosition) { cameraPosition_ = cameraPosition; }


	/**
	 * @brief 정적 메시를 렌더링합니다.
	 *
	 * @param world 정적 메시의 월드 행렬입니다.
	 * @param mesh 정적 메시 리소스입니다.
	 * @param material 정적 메시의 재질입니다.
	 */
	void DrawStaticMesh(const Mat4x4& world, const StaticMesh* mesh, const ITexture* material);


	/**
	 * @brief 스키닝 메시를 렌더링합니다.
	 *
	 * @param world 정적 메시의 월드 행렬입니다.
	 * @param bindPose 바인드 포즈 행렬입니다.
	 * @param invBindPose 역 바인드 포즈 행렬입니다.
	 * @param mesh 스키닝 메시 리소스입니다.
	 * @param material 스키닝 메시의 재질입니다.
	 */
	void DrawSkinnedMesh(const Mat4x4& world, const std::vector<Mat4x4>& bindPose, const std::vector<Mat4x4>& invBindPose, const SkinnedMesh* mesh, const ITexture* material);


private:
	/**
	 * @brief 렌더러의 뷰 행렬입니다.
	 */
	Mat4x4 view_;


	/**
	 * @brief 렌더러의 원근 투영 행렬입니다.
	 */
	Mat4x4 projection_;


	/**
	 * @brief 라이트의 위치입니다.
	 */
	Vec3f lightPosition_;


	/**
	 * @brief 라이트의 시점의 행렬입니다.
	 */
	Mat4x4 lightSpaceMatrix_;


	/**
	 * @brief 카메라의 현재 위치입니다.
	 */
	Vec3f cameraPosition_;


	/**
	 * @brief 셰도우 매핑 시 참조할 셰도우 맵입니다. 
	 */
	ShadowMap* shadowMap_ = nullptr;


	/**
	 * @brief 정적 메시의 깊이 값을 렌더링할 셰이더입니다.
	 */
	Shader* staticMeshShader_ = nullptr;


	/**
	 * @brief 스키닝 메시의 깊이 값을 렌더링할 셰이더입니다.
	 */
	Shader* skinnedMeshShader_ = nullptr;


	/**
	 * @brief 셰도우 맵의 바인드 슬롯입니다.
	 */
	static const uint32_t SHADOW_MAP_BIND_SLOT = 0;


	/**
	 * @brief 메시 재질 맵의 바인드 슬롯입니다.
	 */
	static const uint32_t MATERIAL_MAP_BIND_SLOT = 1;
};
