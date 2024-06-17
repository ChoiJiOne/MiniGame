#pragma once

#include <array>

#include "IResource.h"
#include "RenderModule.h"
#include "VertexBuffer.h"

class Shader;
class Skybox;


/**
 * @brief 스카이 박스를 렌더링하는 렌더러입니다.
 */
class SkyboxRenderer : public IResource
{
public:
	/**
	 * @brief 스카이 박스를 그리는 렌더러의 생성자입니다.
	 */
	explicit SkyboxRenderer();


	/**
	 * @brief 스카이 박스를 그리는 렌더러의 가상 소멸자입니다.
	 *
	 * @note 렌더러 내부에서 할당된 요소를 해제하기 위해서는 반드시 Release를 호출해야 합니다.
	 */
	virtual ~SkyboxRenderer();


	/**
	 * @brief 스카이 박스를 그리는 렌더러의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(SkyboxRenderer);


	/**
	 * @brief 렌더러 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 스카이 박스 그리기를 수행합니다.
	 *
	 * @param view 바인딩된 프레임 버퍼에 그릴 시 참조할 시야 행렬입니다.
	 * @param projection 바인딩된 프레임 버퍼에 그릴 시 참조할 투영 행렬입니다.
	 * @param skybox 바인딩된 프레임 버퍼에 그릴 스카이 박스입니다.
	 */
	void Draw(const Mat4x4& view, const Mat4x4& projection, const Skybox* skybox);


private:
	/**
	 * @brief 스카이 박스의 정점 목록의 오브젝트입니다.
	 */
	uint32_t vertexArrayObject_ = 0;


	/**
	 * @brief 스카이 박스 정점의 수입니다.
	 */
	uint32_t vertexCount_ = 0;


	/**
	 * @brief 스카이 박스의 정점 버퍼입니다.
	 */
	VertexBuffer* vertexBuffer_ = nullptr;


	/**
	 * @brief 스카이 박스를 렌더링하는 셰이더입니다.
	 */
	Shader* shader_ = nullptr;


	/**
	 * @brief 스카이 박스 텍스처의 바인드 슬롯입니다.
	 */
	static const uint32_t SKYBOX_BIND_SLOT = 0;
};