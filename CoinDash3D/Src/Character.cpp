#include <cgltf.h>

#include "GLTFLoader.h"
#include "InputController.h"
#include "RenderModule.h"
#include "Texture2D.h"

#include "Camera.h"
#include "Character.h"

Character::Character()
{
	static bool bIsInitMesh = false;
	if (!bIsInitMesh)
	{
		cgltf_data* data = GLTFLoader::Load("Resource/Model/Michelle.gltf");

		std::vector<GLTFLoader::MeshData> meshData = GLTFLoader::LoadSkinnedMeshData(data);
		skeleton_ = GLTFLoader::LoadSkeleton(data);
		clips_ = GLTFLoader::LoadAnimationClips(data);

		GLTFLoader::Free(data);

		for (const auto& mesh : meshData)
		{
			uint32_t size = mesh.positions.size();

			std::vector<VertexPositionNormalUvSkin3D> vertices(size);
			std::vector<uint32_t> indices = mesh.indices;

			for (uint32_t index = 0; index < size; ++index)
			{
				vertices[index].position = mesh.positions[index];
				vertices[index].normal = mesh.normals[index];
				vertices[index].uv = mesh.texcoords[index];
				vertices[index].weight = mesh.weights[index];
				vertices[index].joints = mesh.joints[index];
			}

			meshes_.push_back(RenderModule::CreateResource<SkinnedMesh>(vertices, indices, false));
		}

		bIsInitMesh = true;
	}

	static Texture2D* material = nullptr;
	if (!material)
	{
		material = RenderModule::CreateResource<Texture2D>("Resource/Texture/Michelle.png", false);
	}

	material_ = material;
	sphere_ = Sphere(transform_.position + Vec3f(0.0f, 0.7f, 0.0f), 0.5f);
	transform_ = Transform();
	
	for (uint32_t index = 0; index < clips_.size(); ++index)
	{
		if (clips_[index].GetName() == "IDLE")
		{
			currentClip_ = index;
			break;
		}
	}

	crossFadeController_.SetSkeleton(skeleton_);
	crossFadeController_.Play(&clips_[currentClip_]);
}

Character::~Character()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Character::Tick(float deltaSeconds)
{
	crossFadeController_.Update(deltaSeconds);

	if (InputController::GetKeyPressState(EKey::KEY_LEFT) == EPressState::PRESSED)
	{
		transform_.rotate = Quat::AxisRadian(Vec3f(0.0f, 1.0f, 0.0f), MathModule::ToRadian(-90.0f));
	}

	if (InputController::GetKeyPressState(EKey::KEY_RIGHT) == EPressState::PRESSED)
	{
		transform_.rotate = Quat::AxisRadian(Vec3f(0.0f, 1.0f, 0.0f), MathModule::ToRadian(+90.0f));
	}

	if (InputController::GetKeyPressState(EKey::KEY_UP) == EPressState::PRESSED)
	{
		transform_.rotate = Quat::AxisRadian(Vec3f(0.0f, 1.0f, 0.0f), MathModule::ToRadian(-180.0f));
	}

	if (InputController::GetKeyPressState(EKey::KEY_DOWN) == EPressState::PRESSED)
	{
		transform_.rotate = Quat::AxisRadian(Vec3f(0.0f, 1.0f, 0.0f), MathModule::ToRadian(+0.0f));
	}
}

void Character::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}