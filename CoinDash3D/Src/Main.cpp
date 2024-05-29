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

#include "Camera.h"
#include "Character.h"
#include "Floor.h"
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
	MeshRenderer3D* meshRenderer = RenderModule::CreateResource<MeshRenderer3D>();

	PlatformModule::SetEndLoopCallback([&]() { RenderModule::Uninit(); });

	Character* character = GameModule::CreateEntity<Character>();
	Camera* camera = GameModule::CreateEntity<Camera>(character);
	Floor* floor = GameModule::CreateEntity<Floor>();
	Wall* wall = GameModule::CreateEntity<Wall>();

	PlatformModule::RunLoop(
		[&](float deltaSeconds) 
		{
			wall->Tick(deltaSeconds);
			character->Tick(deltaSeconds);
			camera->Tick(deltaSeconds);

			geometryRenderer->SetView(camera->GetView());
			geometryRenderer->SetProjection(camera->GetProjection());
			meshRenderer->SetView(camera->GetView());
			meshRenderer->SetProjection(camera->GetProjection());

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			geometryRenderer->DrawGrid3D(Vec3f(100.0f, 100.0f, 100.0f), 1.0f);
			
			std::vector<StaticMesh*>& meshes1 = wall->GetMeshes();
			std::vector<Transform>& transforms1 = wall->GetTransforms();
			ITexture2D* material1 = wall->GetMaterial();

			for (uint32_t index = 0; index < meshes1.size(); ++index)
			{
				meshRenderer->DrawStaticMesh3D(Transform::ToMat(transforms1[index]), meshes1[index], material1);
			}

			std::vector<StaticMesh*>& meshes2 = floor->GetMeshes();
			std::vector<Transform>& transforms2= floor->GetTransforms();
			ITexture2D* material2 = floor->GetMaterial();

			for (uint32_t index = 0; index < meshes2.size(); ++index)
			{
				meshRenderer->DrawStaticMesh3D(Transform::ToMat(transforms2[index]), meshes2[index], material2);
			}

			std::vector<SkinnedMesh*>& meshes = character->GetMeshes();
			for (const auto& mesh : meshes)
			{
				mesh->Skin(&character->GetCrossFadeController().GetSkeleton(), &character->GetCrossFadeController().GetCurrentPose());
				const std::vector<Mat4x4>& bindPose = mesh->GetPosePalette();
				const std::vector<Mat4x4>& invBindPose = character->GetCrossFadeController().GetSkeleton().GetInvBindPose();
				meshRenderer->DrawSkinnedMesh3D(Transform::ToMat(character->GetTransform()), mesh, bindPose, invBindPose, character->GetMaterial());
			}

			geometryRenderer->DrawSphere3D(Mat4x4::Translation(character->GetSphere().center), character->GetSphere().radius, Vec4f(0.0f, 1.0f, 0.0f, 1.0f));

			RenderModule::EndFrame();
		}
	);

	GameModule::Uninit();
	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}