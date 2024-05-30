#include <array>

#include <cgltf.h>

#include "GLTFLoader.h"
#include "InputController.h"
#include "RenderModule.h"
#include "Texture2D.h"

#include "Camera.h"
#include "Character.h"
#include "Wall.h"

Character::Character(Wall* wall)
	: wall_(wall)
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
	sphere_ = Sphere(transform_.position + Vec3f(0.0f, 0.7f, 0.0f), 0.3f);
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

	static std::array<EKey, 4> keys =
	{
		EKey::KEY_A,
		EKey::KEY_D,
		EKey::KEY_W,
		EKey::KEY_S,
	};

	// 하나라도, Press면 달리기 수행
	for (const auto& key : keys)
	{
		EPressState state = InputController::GetKeyPressState(key);

		if (state == EPressState::PRESSED && currentStatus_ == EStatus::IDLE)
		{
			currentStatus_ = EStatus::RUN;
			crossFadeController_.FadeTo(&clips_[runClip_], 0.1f);
			break;
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

	if (bChangeClip && currentStatus_ == EStatus::RUN)
	{
		currentStatus_ = EStatus::IDLE;
		crossFadeController_.FadeTo(&clips_[idleClip_], 0.1f);
	}

	Vec3f direction = GetMoveDirection();
	float rotateRadian = Vec3f::Radian(direction, Vec3f(0.0f, 0.0f, 1.0f));
	rotateRadian = (direction.x > 0.0f) ? rotateRadian : TwoPi - rotateRadian;

	transform_.rotate = Quat::AxisRadian(Vec3f(0.0f, 1.0f, 0.0f), rotateRadian);

	if (currentStatus_ == EStatus::RUN)
	{
		Vec3f position = transform_.position;

		float sin = MathModule::Sin(rotateRadian);
		float cos = MathModule::Cos(rotateRadian);
		
		position += Vec3f(deltaSeconds * moveSpeed_ * sin, 0.0f, deltaSeconds * moveSpeed_ * cos);
		sphere_.center = position + Vec3f(0.0f, 0.7f, 0.0f);

		const std::vector<AABB>& aabbs = wall_->GetBoundingBoxes();
		bool bIsCollision = false;
		for (const auto& aabb : aabbs)
		{
			if (Collision::SphereToAABB(sphere_, aabb))
			{
				bIsCollision = true;
				break;
			}
		}

		if (bIsCollision)
		{
			position = transform_.position;
			sphere_.center = position + Vec3f(0.0f, 0.7f, 0.0f);
		}

		transform_.position = position;
	}

	for (const auto& mesh : meshes_)
	{
		mesh->Skin(&skeleton_, &crossFadeController_.GetCurrentPose());
	}
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

	if (InputController::GetKeyPressState(EKey::KEY_A) == EPressState::HELD)
	{
		bIsUpdate = true;
		direction += Vec3f(-1.0f, 0.0f, 0.0f);
	}

	if (InputController::GetKeyPressState(EKey::KEY_D) == EPressState::HELD)
	{
		bIsUpdate = true;
		direction += Vec3f(+1.0f, 0.0f, 0.0f);
	}

	if (InputController::GetKeyPressState(EKey::KEY_W) == EPressState::HELD)
	{
		bIsUpdate = true;
		direction += Vec3f(0.0f, 0.0f, -1.0f);
	}

	if (InputController::GetKeyPressState(EKey::KEY_S) == EPressState::HELD)
	{
		bIsUpdate = true;
		direction += Vec3f(0.0f, 0.0f, +1.0f);
	}

	if (bIsUpdate && direction != Vec3f(0.0f, 0.0f, 0.0f))
	{
		currentStatus_ = EStatus::RUN;
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