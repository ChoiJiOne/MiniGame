#include "Assertion.h"
#include "DepthRenderer.h"
#include "GeometryRenderer2D.h"
#include "GeometryRenderer3D.h"
#include "MeshRenderer.h"
#include "ShadowMap.h"
#include "TextRenderer.h"

#include "Application.h"
#include "Camera.h"
#include "Character.h"
#include "Coin.h"
#include "CoinSpawner.h"
#include "Floor.h"
#include "Light.h"
#include "MiniMap.h"
#include "PlayScene.h"
#include "StatusViewer.h"

PlayScene::PlayScene(Application* app)
{
	depthRenderer_ = app->depthRenderer_;
	geometryRenderer2D_ = app->geometryRenderer2D_;
	geometryRenderer3D_ = app->geometryRenderer3D_;
	meshRenderer_ = app->meshRenderer_;
	textRenderer_ = app->textRenderer_;
	shadowMap_ = app->shadowMap_;
	fonts_ = app->fonts_;
}

void PlayScene::Tick(float deltaSeconds)
{
	Update(deltaSeconds);
	PrepareForRendering();
	Render();
}

void PlayScene::Enter()
{
	floor_ = GameModule::CreateEntity<Floor>();
	character_ = GameModule::CreateEntity<Character>();
	camera_ = GameModule::CreateEntity<Camera>(character_);
	light_ = GameModule::CreateEntity<Light>(camera_);
	coins_ = std::list<Coin*>();
	coinSpawner_ = GameModule::CreateEntity<CoinSpawner>(coins_, character_);
	miniMap_ = GameModule::CreateEntity<MiniMap>(coins_, character_, geometryRenderer2D_);
	statusViewer_ = GameModule::CreateEntity<StatusViewer>(character_, geometryRenderer2D_, textRenderer_, fonts_[24]);

	bIsEnter_ = true;
}

void PlayScene::Exit()
{
	std::array<IEntity*, 7> entities =
	{
		statusViewer_,
		miniMap_,
		coinSpawner_,
		light_,
		camera_,
		character_,
		floor_,
	};

	for (auto& entity : entities)
	{
		if (entity)
		{
			GameModule::DestroyEntity(entity);
		}
	}

	for (auto& coin : coins_)
	{
		if (coin)
		{
			GameModule::DestroyEntity(coin);
		}
	}

	bIsEnter_ = false;
	bDetectSwitch_ = false;
}

void PlayScene::Update(float deltaSeconds)
{
	std::array<IEntity*, 3> entities =
	{ 
		character_,
		camera_,
		light_,
	};

	for (auto& entity : entities)
	{
		entity->Tick(deltaSeconds);
	}

	for (auto& coin : coins_)
	{
		coin->Tick(deltaSeconds);
	}

	coinSpawner_->Tick(deltaSeconds);
	miniMap_->Tick(deltaSeconds);
	statusViewer_->Tick(deltaSeconds);
}

void PlayScene::PrepareForRendering()
{
	Mat4x4 screenOrtho = RenderModule::GetScreenOrtho();

	geometryRenderer2D_->SetOrtho(screenOrtho);
	geometryRenderer3D_->SetView(camera_->GetView());
	geometryRenderer3D_->SetProjection(camera_->GetProjection());
	meshRenderer_->SetView(camera_->GetView());
	meshRenderer_->SetProjection(camera_->GetProjection());
	meshRenderer_->SetCameraPosition(camera_->GetEyePosition());
	textRenderer_->SetOrtho(screenOrtho);
}

void PlayScene::Render()
{
	DepthPass();
	RenderPass();
}

void PlayScene::DepthPass()
{
	shadowMap_->Bind();
	shadowMap_->Clear();
	RenderModule::SetViewport(0, 0, shadowMap_->GetSize(), shadowMap_->GetSize());

	depthRenderer_->SetLightSpaceMatrix(light_->GetLightSpaceMatrix());

	const std::vector<StaticMesh*>& staticMeshes = floor_->GetMeshes();
	for (const auto& mesh : staticMeshes)
	{
		depthRenderer_->DrawStaticMesh(Transform::ToMat(floor_->GetTransform()), mesh);
	}

	for (const auto& coin : coins_)
	{
		const std::vector<StaticMesh*>& meshes = coin->GetMeshes();
		for (const auto& mesh : meshes)
		{
			depthRenderer_->DrawStaticMesh(Transform::ToMat(coin->GetTransform()), mesh);
		}
	}

	const std::vector<SkinnedMesh*>& skinnedMeshes = character_->GetMeshes();
	for (const auto& mesh : skinnedMeshes)
	{
		depthRenderer_->DrawSkinnedMesh(Transform::ToMat(character_->GetTransform()), character_->GetBindPose(), character_->GetInvBindPose(), mesh);
	}

	shadowMap_->Unbind();
}

void PlayScene::RenderPass()
{
	RenderModule::BeginFrame(0.53f, 0.81f, 0.98f, 1.0f);

	meshRenderer_->SetShadowMap(shadowMap_);
	meshRenderer_->SetLightSpaceMatrix(light_->GetLightSpaceMatrix());
	meshRenderer_->SetLightDirection(light_->GetDirection());
	meshRenderer_->SetLightColor(light_->GetColor());

	const std::vector<StaticMesh*>& staticMeshes = floor_->GetMeshes();
	for (const auto& mesh : staticMeshes)
	{
		meshRenderer_->DrawStaticMesh(Transform::ToMat(floor_->GetTransform()), mesh, floor_->GetMaterial());
	}

	for (const auto& coin : coins_)
	{
		const std::vector<StaticMesh*>& meshes = coin->GetMeshes();
		for (const auto& mesh : meshes)
		{
			meshRenderer_->DrawStaticMesh(Transform::ToMat(coin->GetTransform()), mesh, coin->GetMaterial());
		}
	}

	const std::vector<SkinnedMesh*>& skinnedMeshes = character_->GetMeshes();
	for (const auto& mesh : skinnedMeshes)
	{
		meshRenderer_->DrawSkinnedMesh(Transform::ToMat(character_->GetTransform()), character_->GetBindPose(), character_->GetInvBindPose(), mesh, character_->GetMaterial());
	}

	miniMap_->Render();
	statusViewer_->Render();
		
	RenderModule::EndFrame();
}