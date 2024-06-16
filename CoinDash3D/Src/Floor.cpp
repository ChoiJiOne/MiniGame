#include "GLTFLoader.h"
#include "RenderModule.h"
#include "StaticMesh.h"
#include "TileColorMap.h"

#include "Floor.h"

Floor::Floor()
{
	static std::vector<StaticMesh*> meshes;
	if (meshes.empty())
	{
		cgltf_data* data = GLTFLoader::Load("Resource/Model/Floor.glb");
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
		TileColorMap::ESize boardSize = TileColorMap::ESize::Size_1024x1024;
		TileColorMap::ESize tileSize = TileColorMap::ESize::Size_32x32;
		Vec4f color0 = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		Vec4f color1 = Vec4f(1.0f, 0.0f, 0.0f, 1.0f);

		material = RenderModule::CreateResource<TileColorMap>(boardSize, tileSize, color0, color1);
	}

	meshes_ = meshes;
	transform_ = Transform();
	material_ = material;

	bIsInitialized_ = true;
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
	// 아무 동작도 수행하지 않습니다.
}

void Floor::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}