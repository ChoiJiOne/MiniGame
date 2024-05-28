#pragma once

#include <vector>

#include "IMesh.h"
#include "IndexBuffer.h"
#include "Vertex3D.h"
#include "VertexBuffer.h"

class Skeleton;
class Pose;


/**
 * @brief 스키닝 메시 리소스입니다.
 */
class SkinnedMesh : public IMesh
{
public:
	/**
	 * @brief 메시 리소스를 생성합니다.
	 * 
	 * @param vertices 메시의 정점 목록입니다.
	 * @param indices 메시의 인덱스 목록입니다.
	 * @param 스키닝된 정점 목록을 GPU로 전송할 지 여부입니다.
	 */
	explicit SkinnedMesh(const std::vector<VertexPositionNormalUvSkin3D>& vertices, const std::vector<uint32_t>& indices, bool bIsUploadGPU);


	/**
	 * @brief 메시의 가상 소멸자입니다.
	 */
	virtual ~SkinnedMesh();


	/**
	 * @brief 메시의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(SkinnedMesh);


	/**
	 * @brief 메시의 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 메시 리소스를 파이프라인에 바인딩합니다.
	 */
	virtual void Bind() override;


	/**
	 * @brief 바인딩된 메시 리소스를 바인딩 해제합니다.
	 */
	virtual void Unbind() override;


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


	/**
	 * @brief 메시의 정점 목록을 얻습니다.
	 *
	 * @return 메시의 정점 목록 상수 참조자를 반환합니다.
	 */
	const std::vector<VertexPositionNormalUvSkin3D>& GetVertices() const { return vertices_; }


	/**
	 * @brief 메시의 스키닝된 정점 목록을 얻습니다.
	 *
	 * @return 메시의 정점 목록 상수 참조자를 반환합니다.
	 */
	const std::vector<VertexPositionNormalUvSkin3D>& GetSkinnedVertices() const { return skinnedVertices_; }


	/**
	 * @brief 포즈 행렬 목록을 얻습니다.
	 * 
	 * @return 포즈 행렬 목록의 상수 참조자를 반환합니다.
	 */
	const std::vector<Mat4x4>& GetPosePalette() const { return posePalette_; }


	/**
	 * @brief 메시 스키닝을 수행합니다.
	 * 
	 * @param skeleton 스키닝 수행 시 참조할 뼈대입니다. 
	 * @param pose 스키닝 수행 시 참조할 포즈입니다.
	 */
	void Skin(Skeleton* skeleton, Pose* pose);


private:
	/**
	 * @brief 정점 목록입니다.
	 */
	std::vector<VertexPositionNormalUvSkin3D> vertices_;


	/**
	 * @brief 스키닝된 정점 목록입니다.
	 */
	std::vector<VertexPositionNormalUvSkin3D> skinnedVertices_;


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


	/**
	 * @brief 포즈 행렬 목록입니다.
	 */
	std::vector<Mat4x4> posePalette_;


	/**
	 * @brief 스키닝된 정점 목록을 GPU로 전송할 지 여부입니다.
	 */
	bool bIsUploadGPU_ = false;
};