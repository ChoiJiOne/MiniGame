#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "CrashModule.h"
#include "GameModule.h"
#include "GLTFLoader.h"
#include "PlatformModule.h"
#include "RenderModule.h"

#include "Camera.h"
#include "GLTFLoader.h"
#include "GeometryRenderer.h"
#include "SkinnedMesh.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "GLTFLoader.h"
#include "CrossFadeController.h"

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
	
	cgltf_data* data = GLTFLoader::Load("Resource/Model/Soldier.gltf");
	std::vector<GLTFLoader::MeshResource> meshResources = GLTFLoader::LoadMeshResources(data);
	Skeleton skeleton = GLTFLoader::LoadSkeleton(data);
	std::vector<Clip> clips = GLTFLoader::LoadAnimationClips(data);
	GLTFLoader::Free(data);
	
	std::vector<SkinnedMesh*> meshes;
	{
		for (const auto& meshResource : meshResources)
		{
			std::vector<SkinnedMesh::Vertex> vertices(meshResource.positions.size());
			std::vector<uint32_t> indices = meshResource.indices;

			for (uint32_t index = 0; index < vertices.size(); ++index)
			{
				vertices[index].position = meshResource.positions[index];
				vertices[index].normal = meshResource.normals[index];
				vertices[index].texcoord = meshResource.texcoords[index];
				vertices[index].weight = meshResource.weights[index];
				vertices[index].joints = meshResource.joints[index];
			}

			meshes.push_back(RenderModule::CreateResource<SkinnedMesh>(vertices, indices));
		}
	}

	uint32_t currentClip = 1;
	CrossFadeController crossFadeController;
	crossFadeController.SetSkeleton(skeleton);
	crossFadeController.Play(&clips[currentClip]);
	crossFadeController.Update(0.0f);

	std::vector<Mat4x4> bindPose;

	GeometryRenderer* renderer = RenderModule::CreateResource<GeometryRenderer>();
	Shader* shader = RenderModule::CreateResource<Shader>("Resource/Shader/SkinnedMesh.vert", "Resource/Shader/Mesh.frag");
	Camera* camera = GameModule::CreateEntity<Camera>();

	PlatformModule::RunLoop(
		[&](float deltaSeconds)
		{
			camera->Tick(deltaSeconds);
			crossFadeController.Update(deltaSeconds);

			renderer->SetView(camera->GetView());
			renderer->SetProjection(camera->GetProjection());
			renderer->SetOrtho(RenderModule::GetScreenOrtho());

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer->DrawGrid3D(Vec3f(100.0f, 100.0f, 100.0f), 1.0f);

			shader->Bind();
			{
				crossFadeController.GetCurrentPose().GetMatrixPalette(bindPose);
				const std::vector<Mat4x4>& invBindPose = crossFadeController.GetSkeleton().GetInvBindPose();

				shader->SetUniform("world", Mat4x4::Identity());
				shader->SetUniform("view", camera->GetView());
				shader->SetUniform("projection", camera->GetProjection());
				shader->SetUniform("bindPose", bindPose.data(), bindPose.size());
				shader->SetUniform("invBindPose", invBindPose.data(), invBindPose.size());

				for (const auto& mesh : meshes)
				{
					mesh->Bind();
					RenderModule::ExecuteDrawIndex(mesh->GetIndexCount(), EDrawMode::TRIANGLES);
					mesh->Unbind();
				}
			}
			shader->Unbind();

			RenderModule::EndFrame();
		}
	);

	GameModule::Uninit();
	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}