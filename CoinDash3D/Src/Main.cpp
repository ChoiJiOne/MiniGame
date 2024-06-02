#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "CrashModule.h"
#include "Checkboard.h"
#include "GameModule.h"
#include "GeometryRenderer3D.h"
#include "GLTFLoader.h"
#include "MeshRenderer3D.h"
#include "PlatformModule.h"
#include "RenderModule.h"
#include "StaticMesh.h"
#include "ShadowMap.h"
#include "DepthRenderer3D.h"

#include "Camera.h"
#include "Character.h"
#include "Coin.h"
#include "Floor.h"
#include "Light.h"
#include "Wall.h"

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

	GeometryRenderer3D* geometryRenderer = RenderModule::CreateResource<GeometryRenderer3D>();
	DepthRenderer3D* depthRenderer = RenderModule::CreateResource<DepthRenderer3D>();
	MeshRenderer3D* meshRenderer = RenderModule::CreateResource<MeshRenderer3D>();
	ShadowMap* shadowMap = RenderModule::CreateResource<ShadowMap>(ShadowMap::ESize::Size_1024x1024);

	PlatformModule::SetEndLoopCallback([&]() { RenderModule::Uninit(); });

	Floor* floor = GameModule::CreateEntity<Floor>();
	Wall* wall = GameModule::CreateEntity<Wall>();
	Character* character = GameModule::CreateEntity<Character>(wall);
	Camera* camera = GameModule::CreateEntity<Camera>(character);
	Coin* coin = GameModule::CreateEntity<Coin>(Vec3f(1.0f, 0.5f, 1.0f));
	Light* light = GameModule::CreateEntity<Light>(character);

	PlatformModule::RunLoop(
		[&](float deltaSeconds)
		{
			character->Tick(deltaSeconds);
			camera->Tick(deltaSeconds);
			light->Tick(deltaSeconds);

			depthRenderer->SetLightView(light->GetView());
			depthRenderer->SetLightProjection(light->GetProjection());
			geometryRenderer->SetView(camera->GetView());
			geometryRenderer->SetProjection(camera->GetProjection());
			meshRenderer->SetView(camera->GetView());
			meshRenderer->SetProjection(camera->GetProjection());
			meshRenderer->SetViewPosition(camera->GetEyePosition());
			meshRenderer->SetShadowMap(shadowMap);
			meshRenderer->SetLightView(light->GetView());
			meshRenderer->SetLightProjection(light->GetProjection());
			meshRenderer->SetLightDirection(light->GetDirection());
			meshRenderer->SetLightColor(light->GetColor());

			shadowMap->Bind();
			{
				shadowMap->Clear();
				RenderModule::SetViewport(0, 0, shadowMap->GetSize(), shadowMap->GetSize());

				std::vector<StaticMesh*>& staticMeshes = wall->GetMeshes();
				std::vector<Transform>& transforms = wall->GetTransforms();

				for (uint32_t index = 0; index < staticMeshes.size(); ++index)
				{
					depthRenderer->DrawStaticMesh3D(Transform::ToMat(transforms[index]), staticMeshes[index]);
				}

				depthRenderer->DrawStaticMesh3D(Transform::ToMat(floor->GetTransform()), floor->GetMesh());
				depthRenderer->DrawStaticMesh3D(Transform::ToMat(coin->GetTransform()), coin->GetMesh());

				std::vector<SkinnedMesh*>& skinnedMeshes = character->GetMeshes();
				for (const auto& skinnedMesh : skinnedMeshes)
				{
					const std::vector<Mat4x4>& bindPose = skinnedMesh->GetPosePalette();
					const std::vector<Mat4x4>& invBindPose = character->GetCrossFadeController().GetSkeleton().GetInvBindPose();
					depthRenderer->DrawSkinnedMesh3D(Transform::ToMat(character->GetTransform()), skinnedMesh, bindPose, invBindPose);
				}
			}
			shadowMap->Unbind();

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
			{
				std::vector<StaticMesh*>& staticMeshes = wall->GetMeshes();
				std::vector<Transform>& transforms = wall->GetTransforms();
				ITexture2D* material = wall->GetMaterial();

				for (uint32_t index = 0; index < staticMeshes.size(); ++index)
				{
					meshRenderer->DrawStaticMesh3D(Transform::ToMat(transforms[index]), staticMeshes[index], material);
				}

				meshRenderer->DrawStaticMesh3D(Transform::ToMat(floor->GetTransform()), floor->GetMesh(), floor->GetMaterial());
				meshRenderer->DrawStaticMesh3D(Transform::ToMat(coin->GetTransform()), coin->GetMesh(), coin->GetMaterial());

				std::vector<SkinnedMesh*>& skinnedMeshes = character->GetMeshes();
				for (const auto& skinnedMesh : skinnedMeshes)
				{
					const std::vector<Mat4x4>& bindPose = skinnedMesh->GetPosePalette();
					const std::vector<Mat4x4>& invBindPose = character->GetCrossFadeController().GetSkeleton().GetInvBindPose();
					meshRenderer->DrawSkinnedMesh3D(Transform::ToMat(character->GetTransform()), skinnedMesh, bindPose, invBindPose, character->GetMaterial());
				}
			}
			RenderModule::EndFrame();
		}
	);

	GameModule::Uninit();
	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}