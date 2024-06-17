#include "AudioModule.h"
#include "Collision.h"
#include "MathModule.h"

#include "BaseColorMap.h"
#include "GLTFLoader.h"
#include "RenderModule.h"
#include "StaticMesh.h"

#include "Character.h"
#include "Coin.h"

Coin::Coin(const Vec3f& position, Character* character)
	: character_(character)
{
	static std::vector<StaticMesh*> meshes;
	static Vec3f minPosition(Infinity, Infinity, Infinity);
	static Vec3f maxPosition(NegInfinity, NegInfinity, NegInfinity);

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

				if (vertices[index].position.x < minPosition.x && vertices[index].position.y < minPosition.y && vertices[index].position.z < minPosition.z)
				{
					minPosition = vertices[index].position;
				}

				if (vertices[index].position.x > maxPosition.x && vertices[index].position.y > maxPosition.y && vertices[index].position.z > maxPosition.z)
				{
					maxPosition = vertices[index].position;
				}
			}

			meshes.push_back(RenderModule::CreateResource<StaticMesh>(vertices, indices));
		}
	}
	
	static ITexture* material = nullptr;
	if (!material)
	{
		material = RenderModule::CreateResource<BaseColorMap>(BaseColorMap::ESize::Size_256x256, Vec4f(1.0f, 0.843f, 0.0f, 1.0f));
	}

	static SoundID soundID = -1;
	if (soundID < 0)
	{
		soundID = AudioModule::CreateSound("Resource/Sound/Coin.wav");
	}

	meshes_ = meshes;
	transform_ = Transform();
	material_ = material;
	soundID_ = soundID;

	transform_.position += position;

	Vec3f extensions = maxPosition - minPosition;
	aabb_ = AABB(transform_.position, extensions);

	bIsInitialized_ = true;
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
	if (Collision::SphereToAABB(character_->GetSphere(), aabb_))
	{
		int32_t coinCount = character_->GetCoinCount();
		character_->SetCoinCount(coinCount + 1);
		hasCollectedCoin_ = true;

		AudioModule::Reset(soundID_);
		AudioModule::Play(soundID_);
	}
}

void Coin::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}