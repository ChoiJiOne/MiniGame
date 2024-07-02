#pragma once

#include <vector>

#include "GameMath.h"

#include "IMesh.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"


namespace GameMaker
{
/**
 * @brief ���� �޽� ���ҽ��Դϴ�.
 */
class StaticMesh : public IMesh
{
public:
	/**
	 * @brief ���� �޽��� �����Դϴ�.
	 */
	struct Vertex
	{
		Vec3f position;  // ������ ��ġ�Դϴ�.
		Vec3f normal;    // ������ ���� �����Դϴ�.
		Vec3f tangent;   // ������ ź��Ʈ �����Դϴ�.
		Vec2f texcoord;  // ������ �ؽ�ó ��ǥ�Դϴ�.
	};


public:
	/**
	 * @brief �޽� ���ҽ��� �����մϴ�.
	 *
	 * @param vertices �޽��� ���� ����Դϴ�.
	 * @param indices �޽��� �ε��� ����Դϴ�.
	 */
	explicit StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);


	/**
	 * @brief �޽��� ���� �Ҹ����Դϴ�.
	 *
	 * @note �޽� ���ο��� �Ҵ�� ��Ҹ� �����ϱ� ���ؼ��� �ݵ�� Release�� ȣ���ؾ� �մϴ�.
	 */
	virtual ~StaticMesh();


	/**
	 * @brief �޽��� ���� �����ڿ� ���� �����ڸ� ���������� �����մϴ�.
	 */
	DISALLOW_COPY_AND_ASSIGN(StaticMesh);


	/**
	 * @brief �޽��� ���� ���ҽ��� �Ҵ� �����մϴ�.
	 */
	virtual void Release() override;


	/**
	 * @brief �޽� ���ҽ��� ���������ο� ���ε��մϴ�.
	 */
	virtual void Bind() const override;


	/**
	 * @brief ���ε��� �޽� ���ҽ��� ���ε� �����մϴ�.
	 */
	virtual void Unbind() const override;


	/**
	 * @brief �޽��� �ε��� ���� ����ϴ�.
	 *
	 * @return �޽��� �ε��� ���� ��ȯ�մϴ�.
	 */
	virtual uint32_t GetIndexCount() const override { return static_cast<uint32_t>(indices_.size()); }


	/**
	 * @brief �޽��� ���� ���� ����ϴ�.
	 *
	 * @return �޽��� ���� ���� ��ȯ�մϴ�.
	 */
	virtual uint32_t GetVertexCount() const override { return static_cast<uint32_t>(vertices_.size()); }


private:
	/**
	 * @brief ���� ����Դϴ�.
	 */
	std::vector<Vertex> vertices_;


	/**
	 * @brief GPU ���� ���� ���� ������Ʈ�Դϴ�.
	 */
	VertexBuffer* vertexBuffer_ = nullptr;


	/**
	 * @brief ���� ��Ͽ� �����ϴ� �ε��� ����Դϴ�.
	 */
	std::vector<uint32_t> indices_;


	/**
	 * @brief GPU ���� �ε��� ���� ������Ʈ�Դϴ�.
	 */
	IndexBuffer* indexBuffer_ = nullptr;


	/**
	 * @brief ���� �����͸� �����ϴ� �� �ʿ��� ��� �����Դϴ�.
	 *
	 * @see
	 * - https://www.khronos.org/opengl/wiki/Vertex_Specification#:~:text=has%20the%20details.-,Vertex%20Array%20Object,-Vertex%20Array%20Object
	 * - https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml
	 */
	uint32_t vertexArrayObject_ = 0;
};

}