#pragma once

#include <vector>

#include "Vec.h"

#include "IMesh.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"


/**
 * @brief 스키닝 메시 리소스입니다.
 */
class SkinnedMesh : public IMesh
{
public:
	/**
	 * @brief 스키닝 메시의 정점입니다.
	 */
	struct Vertex
	{
		Vec3f position;  // 정점의 위치입니다.
		Vec3f normal;    // 정점의 법선 벡터입니다.
		Vec3f tangent;   // 정점의 탄젠트 벡터입니다.
		Vec2f texcoord;  // 정점의 텍스처 좌표입니다.
		Vec4f weight;    // 정점의 가중치입니다.
		Vec4i joints;    // 정점의 뼈대 인덱스입니다.
	};

public:
	/**
	 * @brief 메시 리소스를 생성합니다.
	 *
	 * @param vertices 메시의 정점 목록입니다.
	 * @param indices 메시의 인덱스 목록입니다.
	 */
	explicit SkinnedMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);


	/**
	 * @brief 메시의 가상 소멸자입니다.
	 *
	 * @note 메시 내부에서 할당된 요소를 해제하기 위해서는 반드시 Release를 호출해야 합니다.
	 */
	virtual ~SkinnedMesh();


	/**
	 * @brief 메시의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(SkinnedMesh);


	/**
	 * @brief 메시의 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 메시 리소스를 파이프라인에 바인딩합니다.
	 */
	virtual void Bind() const override;


	/**
	 * @brief 바인딩된 메시 리소스를 바인딩 해제합니다.
	 */
	virtual void Unbind() const override;


	/**
	 * @brief 메시의 인덱스 수를 얻습니다.
	 *
	 * @return 메시의 인덱스 수를 반환합니다.
	 */
	virtual uint32_t GetIndexCount() const override { return static_cast<uint32_t>(indices_.size()); }


	/**
	 * @brief 메시의 정점 수를 얻습니다.
	 *
	 * @return 메시의 정점 수를 반환합니다.
	 */
	virtual uint32_t GetVertexCount() const override { return static_cast<uint32_t>(vertices_.size()); }


private:
	/**
	 * @brief 정점 목록입니다.
	 */
	std::vector<Vertex> vertices_;


	/**
	 * @brief GPU 상의 정점 버퍼 오브젝트입니다.
	 */
	VertexBuffer* vertexBuffer_ = nullptr;


	/**
	 * @brief 정점 목록에 대응하는 인덱스 목록입니다.
	 */
	std::vector<uint32_t> indices_;


	/**
	 * @brief GPU 상의 인덱스 버퍼 오브젝트입니다.
	 */
	IndexBuffer* indexBuffer_ = nullptr;


	/**
	 * @brief 정점 데이터를 제공하는 데 필요한 모든 상태입니다.
	 *
	 * @see
	 * - https://www.khronos.org/opengl/wiki/Vertex_Specification#:~:text=has%20the%20details.-,Vertex%20Array%20Object,-Vertex%20Array%20Object
	 * - https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml
	 */
	uint32_t vertexArrayObject_ = 0;
};