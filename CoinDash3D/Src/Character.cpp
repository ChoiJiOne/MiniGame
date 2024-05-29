#include <array>

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
			idleClip_ = index;
		}
		else if (clips_[index].GetName() == "RUN")
		{
			runClip_ = index;
		}
	}

	crossFadeController_.SetSkeleton(skeleton_);
	crossFadeController_.Play(&clips_[idleClip_]);

	moveSpeed_ = 5.0f;
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

	Vec3f direction = GetMoveDirection();
	float rotateRadian = Vec3f::Radian(direction, Vec3f(0.0f, 0.0f, 1.0f));
	rotateRadian = (direction.x > 0.0f) ? rotateRadian : TwoPi - rotateRadian;

	transform_.rotate = Quat::AxisRadian(Vec3f(0.0f, 1.0f, 0.0f), rotateRadian);

	if (currentStatus_ == EStatus::RUN)
	{
		float sin = MathModule::Sin(rotateRadian);
		float cos = MathModule::Cos(rotateRadian);
		transform_.position += Vec3f(deltaSeconds * moveSpeed_ * sin, 0.0f, deltaSeconds * moveSpeed_ * cos);
	}

	static std::array<EKey, 4> keys =
	{
		EKey::KEY_LEFT,
		EKey::KEY_RIGHT,
		EKey::KEY_UP,
		EKey::KEY_DOWN,
	};

	// 하나라도, Press면 달리기 수행
	for (const auto& key : keys)
	{
		EPressState state = InputController::GetKeyPressState(key);

		if (state == EPressState::PRESSED)
		{
			currentStatus_ = EStatus::RUN;
			crossFadeController_.FadeTo(&clips_[runClip_], 0.5f);
		}
	}

	// 모두 Released면 대기 수행
	bool bChangeClip = true;
	for (const auto& key : keys)
	{
		EPressState state = InputController::GetKeyPressState(key);

		if (state != EPressState::RELEASED && state != EPressState::NONE) // 모두 Released면 대기 수행
		{
			bChangeClip = false;
			break;
		}
	}

	if (bChangeClip)
	{
		currentStatus_ = EStatus::IDLE;
		crossFadeController_.FadeTo(&clips_[idleClip_], 0.5f);
	}

	sphere_.center = transform_.position + Vec3f(0.0f, 0.7f, 0.0f);
}

void Character::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}

Vec3f Character::GetMoveDirection()
{
	Vec3f direction;
	bool bIsUpdate = false;

	if (InputController::GetKeyPressState(EKey::KEY_LEFT) == EPressState::HELD)
	{
		bIsUpdate = true;
		direction += Vec3f(-1.0f, 0.0f, 0.0f);
	}

	if (InputController::GetKeyPressState(EKey::KEY_RIGHT) == EPressState::HELD)
	{
		bIsUpdate = true;
		direction += Vec3f(+1.0f, 0.0f, 0.0f);
	}

	if (InputController::GetKeyPressState(EKey::KEY_UP) == EPressState::HELD)
	{
		bIsUpdate = true;
		direction += Vec3f(0.0f, 0.0f, -1.0f);
	}

	if (InputController::GetKeyPressState(EKey::KEY_DOWN) == EPressState::HELD)
	{
		bIsUpdate = true;
		direction += Vec3f(0.0f, 0.0f, +1.0f);
	}

	if (bIsUpdate && direction != Vec3f(0.0f, 0.0f, 0.0f))
	{
		direction = Vec3f::Normalize(direction);
	}
	else
	{
		float rotate = Quat::Radian(transform_.rotate);

		direction.x = MathModule::Sin(rotate);
		direction.y = 0.0f;
		direction.z = MathModule::Cos(rotate);
	}

	return direction;
}