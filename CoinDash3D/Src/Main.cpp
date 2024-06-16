#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "CrashModule.h"
#include "DepthRenderer.h"
#include "GameModule.h"
#include "GeometryRenderer3D.h"
#include "GLTFLoader.h"
#include "PlatformModule.h"
#include "MeshRenderer.h"
#include "RenderModule.h"
#include "Shader.h"
#include "ShadowMap.h"
#include "StaticMesh.h"

#include "Camera.h"
#include "Character.h"
#include "Coin.h"
#include "Floor.h"


#include <glad/glad.h>

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	PlatformModule::WindowConstructParams windowParam{ L"CoinDash3D", 100, 100, 800, 600, false, false };

	CrashModule::Init();
	PlatformModule::Init(windowParam);
	RenderModule::Init(PlatformModule::GetWindowHandle());
	GameModule::Init();

	PlatformModule::SetEndLoopCallback([&]() { RenderModule::Uninit(); });

	GeometryRenderer3D* renderer3d = RenderModule::CreateResource<GeometryRenderer3D>();
	DepthRenderer* depthRenderer = RenderModule::CreateResource<DepthRenderer>();
	MeshRenderer* meshRenderer = RenderModule::CreateResource<MeshRenderer>();
	ShadowMap* shadowMap = RenderModule::CreateResource<ShadowMap>(ShadowMap::ESize::Size_2048);
	
	Coin* coin = GameModule::CreateEntity<Coin>(Vec3f(0.0f, 0.5f, 0.0f));
	Floor* floor = GameModule::CreateEntity<Floor>();
	Character* character = GameModule::CreateEntity<Character>();
	Camera* camera = GameModule::CreateEntity<Camera>(character);

	Vec3f lightPosition = Vec3f(5.0f, 5.0f, 5.0f);
	Mat4x4 lightProjection = Mat4x4::Ortho(-4.0f, +4.0f, -4.0f, +4.0f, 1.0f, 40.0f);
	Mat4x4 lightView = Mat4x4::LookAt(lightPosition, Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
	Mat4x4 lightSpaceMatrix = lightView * lightProjection;

		
	PlatformModule::RunLoop(
		[&](float deltaSeconds)
		{
			character->Tick(deltaSeconds);
			camera->Tick(deltaSeconds);
			coin->Tick(deltaSeconds);
			floor->Tick(deltaSeconds);

			renderer3d->SetView(camera->GetView());
			renderer3d->SetProjection(camera->GetProjection());

			shadowMap->Bind();
			{
				shadowMap->Clear();
				RenderModule::SetViewport(0, 0, shadowMap->GetSize(), shadowMap->GetSize());

				depthRenderer->SetLightSpaceMatrix(lightSpaceMatrix);

				const std::vector<StaticMesh*>& meshes0 = floor->GetMeshes();
				for (const auto& mesh : meshes0)
				{
					depthRenderer->DrawStaticMesh(Mat4x4::Identity(), mesh);
				}

				const std::vector<StaticMesh*>& meshes1 = coin->GetMeshes();
				for (const auto& mesh : meshes1)
				{
					depthRenderer->DrawStaticMesh(Mat4x4::Identity(), mesh);
				}

				const std::vector<SkinnedMesh*>& skinnedMeshes = character->GetMeshes();
				for (const auto& mesh : skinnedMeshes)
				{
					depthRenderer->DrawSkinnedMesh(Transform::ToMat(character->GetTransform()), character->GetBindPose(), character->GetInvBindPose(), mesh);
				}
			}
			shadowMap->Unbind();

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			meshRenderer->SetShadowMap(shadowMap);
			meshRenderer->SetView(camera->GetView());
			meshRenderer->SetProjection(camera->GetProjection());
			meshRenderer->SetLightSpaceMatrix(lightSpaceMatrix);
			meshRenderer->SetLightPosition(lightPosition);
			meshRenderer->SetCameraPosition(camera->GetEyePosition());

			const std::vector<StaticMesh*>& meshes0 = floor->GetMeshes();
			for (const auto& mesh : meshes0)
			{
				meshRenderer->DrawStaticMesh(Mat4x4::Identity(), mesh, floor->GetMaterial());
			}

			const std::vector<StaticMesh*>& meshes1 = coin->GetMeshes();
			for (const auto& mesh : meshes1)
			{
				meshRenderer->DrawStaticMesh(Mat4x4::Identity(), mesh, floor->GetMaterial());
			}

			const std::vector<SkinnedMesh*>& skinnedMeshes = character->GetMeshes();
			for (const auto& mesh : skinnedMeshes)
			{
				meshRenderer->DrawSkinnedMesh(Transform::ToMat(character->GetTransform()), character->GetBindPose(), character->GetInvBindPose(), mesh, character->GetMaterial());
			}
			
			RenderModule::EndFrame();
		}
	);

	GameModule::Uninit();
	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}