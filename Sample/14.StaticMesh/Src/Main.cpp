#include <cstdint>
#include <Windows.h>
#include <tuple>

#include <glad/glad.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "IApp.h"
#include "DebugDrawManager3D.h"
#include "EntityManager.h"
#include "FreeEulerCamera3D.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "Texture2D.h"

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("14.StaticMesh", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		SetDepthMode(true);

		Vec3f cameraPos(3.0f, 3.0f, 3.0f);
		float yaw = -GameMath::ToRadian(135.0f);
		float pitch = -PI / 6.0f;
		float fov = PI_DIV_4;
		float nearZ = 0.01f;
		float farZ = 100.0f;

		camera_ = EntityManager::Get().Create<FreeEulerCamera3D>(cameraPos, yaw, pitch, fov, nearZ, farZ);
		shader_ = ResourceManager::Get().Create<Shader>("GameMaker/Sample/14.StaticMesh/Res/Shader.vert", "GameMaker/Sample/14.StaticMesh/Res/Shader.frag");

		StaticMesh* cube = StaticMesh::CreateBox(Vec3f(1.0f, 1.0f, 1.0f));
		Texture2D* box = ResourceManager::Get().Create<Texture2D>("GameMaker/Sample/14.StaticMesh/Res/Box.png", Texture2D::Filter::LINEAR);
		std::tuple<StaticMesh*, Texture2D*, Mat4x4> boxObject(cube, box, Mat4x4::Translation(-1.0f, 0.0f, 0.0f));
		objects_.push_back(boxObject);

		StaticMesh* sphere = StaticMesh::CreateSphere(1.0f, 30);
		Texture2D* earth = ResourceManager::Get().Create<Texture2D>("GameMaker/Sample/14.StaticMesh/Res/Earth.png", Texture2D::Filter::LINEAR);
		std::tuple<StaticMesh*, Texture2D*, Mat4x4> sphereObject(sphere, earth, Mat4x4::Translation(+1.0f, 0.0f, 0.0f));
		objects_.push_back(sphereObject);
	}

	virtual void Shutdown() override
	{
		EntityManager::Get().Destroy(camera_);
		camera_ = nullptr;
	}

	virtual void Run() override
	{
		RunLoop(
			[&](float deltaSeconds)
			{
				camera_->Tick(deltaSeconds);

				BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

				DebugDrawManager3D::Get().Begin(camera_);
				{
					DrawGrid();
				}
				DebugDrawManager3D::Get().End();

				shader_->Bind();
				{
					shader_->SetUniform("view", camera_->GetView());
					shader_->SetUniform("projection", camera_->GetProjection());

					for (auto& object : objects_)
					{
						std::get<1>(object)->Active(0);
						shader_->SetUniform("world", std::get<2>(object));

						std::get<0>(object)->Bind();
						GL_CHECK(glDrawElements(static_cast<GLenum>(DrawMode::TRIANGLES), std::get<0>(object)->GetIndexCount(), GL_UNSIGNED_INT, nullptr));
						std::get<0>(object)->Unbind();
					}
				}
				shader_->Unbind();
				
				EndFrame();
			}
		);
	}

private:
	void DrawGrid()
	{
		Vec4f color;
		for (float x = minX_; x <= maxX_; x += stride_)
		{
			color = (x == 0.0f) ? Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
			DebugDrawManager3D::Get().DrawLine(Vec3f(x, 0.0f, minZ_), Vec3f(x, 0.0f, maxZ_), color);
		}

		for (float z = minZ_; z <= maxZ_; z += stride_)
		{
			color = (z == 0.0f) ? Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
			DebugDrawManager3D::Get().DrawLine(Vec3f(minX_, 0.0f, z), Vec3f(maxX_, 0.0f, z), color);
		}

		DebugDrawManager3D::Get().DrawLine(Vec3f(0.0f, minY_, 0.0f), Vec3f(0.0f, maxY_, 0.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
	}

private:
	float minX_ = -50.0f;
	float maxX_ = +50.0f;
	float minY_ = -50.0f;
	float maxY_ = +50.0f;
	float minZ_ = -50.0f;
	float maxZ_ = +50.0f;
	float stride_ = 1.0f;

	FreeEulerCamera3D* camera_ = nullptr;
	Shader* shader_ = nullptr;

	std::vector<std::tuple<StaticMesh*, Texture2D*, Mat4x4>> objects_;
};

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::unique_ptr<IApp> app = std::make_unique<DemoApp>();
	app->Startup();
	app->Run();
	app->Shutdown();
	return 0;
}