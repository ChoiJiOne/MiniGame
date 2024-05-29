#include "Checkboard.h"
#include "GLTFLoader.h"
#include "RenderModule.h"
#include "StaticMesh.h"

#include "Coin.h"

Coin::Coin()
{
	static bool bIsCreateResource = false;
	if (!bIsCreateResource) // 프로그램 실행 후 한 번만 초기화 수행...
	{
		cgltf_data* data = GLTFLoader::Load("Resource/Model/Coin.gltf");

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
			Checkboard::ESize::Size_512x512,
			Vec4f(1.0f, 0.843f, 0.0f, 1.0f),
			Vec4f(1.0f, 0.843f, 0.0f, 1.0f)
		);
	}

	material_ = material;
}

Coin::~Coin()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Coin::Tick(float deltaSeconds)
{
	// 아무 동작도 수행하지 않습니다.
}

void Coin::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}