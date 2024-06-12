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
	TTFont* font = RenderModule::CreateResource<TTFont>("Resource/Font/SeoulNamsanEB.ttf", 0, 0x127, 24.0f);

	Camera* camera = GameModule::CreateEntity<Camera>();

	std::vector<Vec2f> positions;
	for (float x = 0.0f; x < 1000; x += 10.0f)
	{
		positions.push_back(Vec2f(x, 0.0f));
		positions.push_back(Vec2f(x, 1000.0f));
	}

	for (float y = 0.0f; y < 1000; y += 10.0f)
	{
		positions.push_back(Vec2f(0.0f, y));
		positions.push_back(Vec2f(1000.0f, y));
	}

	std::vector<Vec2f> p;
	p.push_back(Vec2f(100.0f, 100.0f));

	RenderModule::SetPointSizeMode(true);

	PlatformModule::RunLoop(
		[&](float deltaSeconds)
		{
			camera->Tick(deltaSeconds);

			renderer3d->SetView(camera->GetView());
			renderer3d->SetProjection(camera->GetProjection());
			renderer2d->SetOrtho(RenderModule::GetScreenOrtho());
			textRenderer->SetOrtho(RenderModule::GetScreenOrtho());

			RenderModule::BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);


			//renderer3d->DrawGrid3D(Vec3f(100.0f, 100.0f, 100.0f), 1.0f);

			renderer2d->DrawLines2D(positions, Vec4f(1.0f, 1.0f, 1.0f, 1.0f));


			float width = 0.0f;
			float height = 0.0f;
			Vec2f center = Vec2f(100.0f, 100.0f);
			font->MeasureText(L"Hello, World", width, height);
			renderer2d->DrawPoints2D(p, Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
			//renderer2d->DrawRectangle2D(center, 100.0f, 100.0f, 0.0f, Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
			renderer2d->DrawWireframeRectangle2D(center, 100.0f, 19.0f, 0.0f, Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
			//renderer2d->DrawWireframeRectangle2D(center, width, height, 0.0f, Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
			//textRenderer->DrawText2D(font, L"Hello, World!", Vec2f(300.0f, 300.0f), Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

			RenderModule::EndFrame();
		}
	);

	GameModule::Uninit();
	PlatformModule::Uninit();
	CrashModule::Uninit();
	return 0;
}