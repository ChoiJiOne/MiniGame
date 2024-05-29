#include "Checkboard.h"
#include "GLTFLoader.h"
#include "RenderModule.h"
#include "StaticMesh.h"

#include "Floor.h"

Floor::Floor()
{
	static bool bIsCreateResource = false;
	if (!bIsCreateResource) // ���α׷� ���� �� �� ���� �ʱ�ȭ ����...
	{
		cgltf_data* data = GLTFLoader::Load("Resource/Model/Floor.gltf");

		std::vector<GLTFLoader::MeshData> meshData = GLTFLoader::LoadStaticMeshData(data);

		for (auto& mesh : meshData)
		{
			std::size_t size = mesh.positions.size();
			std::vector<VertexPositionNormalUv3D> vertices(size);
			std::vector<uint32_t> indices = mesh.indices;

			for (uint32_t index = 0; index < size; ++index)
			{
				vertices[index].position = mesh.positions[index];
				vertices[index].normal = mesh.normals[index];
				vertices[index].uv = mesh.texcoords[index];
			}

			meshes_.push_back(RenderModule::CreateResource<StaticMesh>(vertices, indices));
		}

		transforms_ = GLTFLoader::LoadTransformData(data);
		GLTFLoader::Free(data);

		bIsCreateResource = true;
	}

	static ITexture2D* material = nullptr;
	if (!material)
	{
		material = RenderModule::CreateResource<Checkboard>(
			Checkboard::ESize::Size_1024x1024,
			Checkboard::ESize::Size_32x32,
			Vec4f(1.0f, 1.0f, 1.0f, 1.0f),
			Vec4f(1.0f, 0.0f, 0.0f, 1.0f)
		);
	}

	material_ = material;
}

Floor::~Floor()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Floor::Tick(float deltaSeconds)
{
	// �ƹ� ���۵� �������� �ʽ��ϴ�.
}

void Floor::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}