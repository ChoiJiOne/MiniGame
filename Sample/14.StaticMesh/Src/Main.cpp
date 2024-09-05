#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "IApp.h"
#include "DebugDrawManager3D.h"
#include "EntityManager.h"
#include "FreeEulerCamera3D.h"

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("14.StaticMesh", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		SetDepthMode(true);

		camera_ = EntityManager::Get().Create<FreeEulerCamera3D>(
			GameMath::Vec3f(3.0f, 3.0f, 3.0f), 
			-GameMath::ToRadian(135.0f), 
			-GameMath::PI / 6.0f, 
			GameMath::PI_DIV_4, 
			0.01f, 
			100.0f
		);
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
				
				EndFrame();
			}
		);
	}

private:
	void DrawGrid()
	{
		GameMath::Vec4f color;
		for (float x = minX_; x <= maxX_; x += stride_)
		{
			color = (x == 0.0f) ? GameMath::Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : GameMath::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
			DebugDrawManager3D::Get().DrawLine(GameMath::Vec3f(x, 0.0f, minZ_), GameMath::Vec3f(x, 0.0f, maxZ_), color);
		}

		for (float z = minZ_; z <= maxZ_; z += stride_)
		{
			color = (z == 0.0f) ? GameMath::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMath::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
			DebugDrawManager3D::Get().DrawLine(GameMath::Vec3f(minX_, 0.0f, z), GameMath::Vec3f(maxX_, 0.0f, z), color);
		}

		DebugDrawManager3D::Get().DrawLine(GameMath::Vec3f(0.0f, minY_, 0.0f), GameMath::Vec3f(0.0f, maxY_, 0.0f), GameMath::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
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