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

	Camera* camera = GameModule::CreateEntity<Camera>();
	Wall* wall = GameModule::CreateEntity<Wall>();

	PlatformModule::RunLoop(
		[&](float deltaSeconds) 
		{
			camera->Tick(deltaSeconds);
			wall->Tick(deltaSeconds);

			geometryRenderer->SetView(camera->GetView());
			geometryRenderer->SetProjection(camera->GetProjection());
			meshRenderer->SetView(camera->GetView());
			meshRenderer->SetProjection(camera->GetProjection());

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			geometryRenderer->DrawGrid3D(Vec3f(100.0f, 100.0f, 100.0f), 1.0f);
			
			std::vector<StaticMesh*>& meshes = wall->GetMeshes();
			std::vector<Transform>& transforms = wall->GetTransforms();
			ITexture2D* material = wall->GetMaterial();

			for (uint32_t index = 0; index < meshes.size(); ++index)
			{
				meshRenderer->DrawStaticMesh3D(Transform::ToMat(transforms[index]), meshes[index], material);
			}

			RenderModule::EndFrame();
		}
	);

	GameModule::Uninit();
	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}