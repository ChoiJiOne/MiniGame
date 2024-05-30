#include "Checkboard.h"
#include "GLTFLoader.h"
#include "RenderModule.h"
#include "StaticMesh.h"

#include "Coin.h"

Coin::Coin(const Vec3f& position)
{
	static StaticMesh* mesh = nullptr;
	static Transform transform;
	if (!mesh)
	{
		cgltf_data* data = GLTFLoader::Load("Resource/Model/Coin.gltf");

		GLTFLoader::MeshData meshData = GLTFLoader::LoadStaticMeshData(data).front();
		transform = GLTFLoader::LoadTransformData(data).front();

		GLTFLoader::Free(data);

		std::vector<VertexPositionNormalUv3D> vertices(meshData.positions.size());
		std::vector<uint32_t> indices = meshData.indices;

		for (uint32_t index = 0; index < vertices.size(); ++index)
		{
			vertices[index].position = meshData.positions[index];
			vertices[index].normal = meshData.normals[index];
			vertices[index].uv = meshData.texcoords[index];
		}

		mesh = RenderModule::CreateResource<StaticMesh>(vertices, indices);
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

	mesh_ = mesh;
	transform_ = transform;
	material_ = material;

	transform_.position += position;
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