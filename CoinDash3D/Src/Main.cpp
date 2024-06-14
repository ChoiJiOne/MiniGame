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
#include "GeometryRenderer2D.h"
#include "GeometryRenderer3D.h"
#include "SkinnedMesh.h"
#include "StaticMesh.h"
#include "TextRenderer.h"
#include "TTFont.h"
#include "Shader.h"
#include "GLTFLoader.h"
#include "CrossFadeController.h"
#include "BaseColorMap.h"
#include "TileColorMap.h"
#include "CascadeShadowMap.h"

#include <glad/glad.h>
#include <imgui.h>

Camera* camera = nullptr;
float cameraNearPlane = 0.01f;
float cameraFarPlane = 500.0f;
std::vector<float> shadowCascadeLevels{ cameraFarPlane / 32.0f, cameraFarPlane / 16.0f, cameraFarPlane / 4.0f, cameraFarPlane / 2.0f };

Vec3f lightDirection = Vec3f::Normalize(Vec3f(20.0f, 50.0f, 20.0f));
std::vector<Mat4x4> lightMatricesCache;

std::vector<Vec4f> GetFrustumCornersWorldSpace(const Mat4x4& proj, const Mat4x4& view)
{
	Mat4x4 inv = Mat4x4::Inverse(view * proj);

	std::vector<Vec4f> frustumCorners;
	for (uint32_t x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				Vec4f pt = Vec4f(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f) * inv;
				pt.x /= pt.w;
				pt.y /= pt.w;
				pt.z /= pt.w;
				pt.w /= pt.w;
				frustumCorners.push_back(pt);
			}
		}
	}

	return frustumCorners;
}

Mat4x4 GetLightSpaceMatrix(const float nearPlane, const float farPlane)
{
	int32_t screenWidth = 0;
	int32_t screenHeight = 0;
	RenderModule::GetScreenSize(screenWidth, screenHeight);
	float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	Mat4x4 proj = Mat4x4::Perspective(PiDiv4, aspectRatio, nearPlane, farPlane);
	Mat4x4 view = camera->GetView();
	const std::vector<Vec4f> corners = GetFrustumCornersWorldSpace(proj, view);

	Vec3f center;
	for (const auto& v : corners)
	{
		center += Vec3f(v.x, v.y, v.z);
	}
	center.x /= corners.size();
	center.y /= corners.size();
	center.z /= corners.size();

	const auto lightView = Mat4x4::LookAt(center + lightDirection, center, Vec3f(0.0f, 1.0f, 0.0f));

	float minX = FLT_MAX;
	float maxX = -FLT_MAX;
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	float minZ = FLT_MAX;
	float maxZ = -FLT_MAX;

	for (const auto& v : corners)
	{
		const auto trf = v * lightView;
		minX = MathModule::Min(minX, trf.x);
		maxX = MathModule::Max(maxX, trf.x);
		minY = MathModule::Min(minY, trf.y);
		maxY = MathModule::Max(maxY, trf.y);
		minZ = MathModule::Min(minZ, trf.z);
		maxZ = MathModule::Max(maxZ, trf.z);
	}

	constexpr float zMult = 5.0f;
	if (minZ < 0)
	{
		minZ *= zMult;
	}
	else
	{
		minZ /= zMult;
	}
	if (maxZ < 0)
	{
		maxZ /= zMult;
	}
	else
	{
		maxZ *= zMult;
	}

	Mat4x4 lightProjection = Mat4x4::Ortho(minX, maxX, minY, maxY, minZ, maxZ);
	return  lightProjection * lightView;
}

std::vector<Mat4x4> GetLightSpaceMatrices()
{
	std::vector<Mat4x4> result;

	for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
	{
		if (i == 0)
		{
			result.push_back(GetLightSpaceMatrix(cameraNearPlane, shadowCascadeLevels[i]));
		}
		else if (i < shadowCascadeLevels.size())
		{
			result.push_back(GetLightSpaceMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
		}
		else
		{
			result.push_back(GetLightSpaceMatrix(shadowCascadeLevels[i - 1], cameraFarPlane));
		}
	}
	return result;
}

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
	
	GeometryRenderer2D* renderer2d = RenderModule::CreateResource<GeometryRenderer2D>();
	GeometryRenderer3D* renderer3d = RenderModule::CreateResource<GeometryRenderer3D>();
	TextRenderer* textRenderer = RenderModule::CreateResource<TextRenderer>();
	TTFont* font = RenderModule::CreateResource<TTFont>("Resource/Font/SeoulNamsanEB.ttf", 0, 0x127, 32.0f);
	TileColorMap* tileColorMap = RenderModule::CreateResource<TileColorMap>(
		TileColorMap::ESize::Size_1024x1024, 
		TileColorMap::ESize::Size_16x16,
		Vec4f(1.0f, 1.0f, 1.0f, 1.0f),
		Vec4f(1.0f, 0.0f, 0.0f, 1.0f)
	);
	camera = GameModule::CreateEntity<Camera>();

	Mat4x4 screenOrtho = RenderModule::GetScreenOrtho();
	renderer2d->SetOrtho(screenOrtho);
	textRenderer->SetOrtho(screenOrtho);

	cgltf_data* data = GLTFLoader::Load("Resource/Model/test03.glb");
	std::vector<GLTFLoader::MeshResource> meshResources = GLTFLoader::LoadMeshResources(data);
	GLTFLoader::Free(data);

	std::vector<StaticMesh*> meshes;
	for (const auto& meshResource : meshResources)
	{
		std::vector<StaticMesh::Vertex> vertices(meshResource.positions.size());
		std::vector<uint32_t> indices = meshResource.indices;

		for (uint32_t index = 0; index < vertices.size(); ++index)
		{
			vertices[index].position = meshResource.positions[index];
			vertices[index].normal = meshResource.normals[index];
			vertices[index].texcoord = meshResource.texcoords[index];
		}

		meshes.push_back(RenderModule::CreateResource<StaticMesh>(vertices, indices));
	}

	Shader* shader = RenderModule::CreateResource<Shader>("Resource/Shader/StaticMesh.vert", "Resource/Shader/Mesh.frag");
	Shader* depth = RenderModule::CreateResource<Shader>("Resource/Shader/Depth.vert", "Resource/Shader/Depth.geom", "Resource/Shader/Depth.frag");
	CascadeShadowMap* cascadeShadowMap = RenderModule::CreateResource<CascadeShadowMap>(CascadeShadowMap::ESize::Size_4096, shadowCascadeLevels.size());
	
	PlatformModule::RunLoop(
		[&](float deltaSeconds)
		{
			camera->Tick(deltaSeconds);

			renderer3d->SetView(camera->GetView());
			renderer3d->SetProjection(camera->GetProjection());

			std::vector<Mat4x4> lightMatrices = GetLightSpaceMatrices();
			cascadeShadowMap->Bind();
			{
				RenderModule::SetViewport(0, 0, cascadeShadowMap->GetSize(), cascadeShadowMap->GetSize());
				cascadeShadowMap->Clear();
				glCullFace(GL_FRONT);
				depth->Bind();
				{
					depth->SetUniform("lightSpaceMatrices", lightMatrices.data(), lightMatrices.size());
					depth->SetUniform("world", Mat4x4::Identity());

					for (const auto& mesh : meshes)
					{
						mesh->Bind();
						RenderModule::ExecuteDrawIndex(mesh->GetIndexCount(), EDrawMode::TRIANGLES);
						mesh->Unbind();
					}
				}
				depth->Unbind();
				glCullFace(GL_BACK);
			}
			cascadeShadowMap->Unbind();
			
			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
			renderer3d->DrawGrid3D(Vec3f(100.0f, 100.0f, 100.0f), 1.0f);

			shader->Bind();
			{
				cascadeShadowMap->Active(0);
				tileColorMap->Active(1);

				shader->SetUniform("world", Mat4x4::Identity());
				shader->SetUniform("view", camera->GetView());
				shader->SetUniform("projection", camera->GetProjection());
				shader->SetUniform("lightDirection", lightDirection);
				shader->SetUniform("cameraPosition", camera->GetEyePosition());
				shader->SetUniform("farPlane", cameraFarPlane);
				shader->SetUniform("lightSpaceMatrices", lightMatrices.data(), lightMatrices.size());
				shader->SetUniform("cascadePlaneDistances", shadowCascadeLevels.data(), shadowCascadeLevels.size());
				shader->SetUniform("cascadeCount", static_cast<int32_t>(shadowCascadeLevels.size()) + 1);

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