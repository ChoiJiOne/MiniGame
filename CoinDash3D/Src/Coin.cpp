#include "BaseColorMap.h"
#include "GLTFLoader.h"
#include "RenderModule.h"
#include "StaticMesh.h"

#include "Coin.h"

Coin::Coin(const Vec3f& position)
{
	static std::vector<StaticMesh*> meshes;
	if (meshes.empty())
	{
		cgltf_data* data = GLTFLoader::Load("Resource/Model/Coin.glb");		
		std::vector<GLTFLoader::MeshResource> meshResources = GLTFLoader::LoadMeshResources(data);
		GLTFLoader::Free(data);

		for (const auto& meshResource : meshResources)
		{
			std::vector<StaticMesh::Vertex> vertices(meshResource.positions.size());
			std::vector<uint32_t> indices = meshResource.indices;

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices[index].position = meshResource.positions[index];
				vertices[index].normal = meshResource.normals[index];
				vertices[index].tangent = meshResource.tangents[index];
				vertices[index].texcoord = meshResource.texcoords[index];
			}

			meshes.push_back(RenderModule::CreateResource<StaticMesh>(vertices, indices));
		}
	}
	
	static ITexture* material = nullptr;
	if (!material)
	{
		material = RenderModule::CreateResource<BaseColorMap>(BaseColorMap::ESize::Size_256x256, Vec4f(1.0f, 0.843f, 0.0f, 1.0f));
	}

	meshes_ = meshes;
	transform_ = Transform();
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