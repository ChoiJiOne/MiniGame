#include "Assertion.h"
#include "DepthRenderer.h"
#include "GeometryRenderer2D.h"
#include "GeometryRenderer3D.h"
#include "MeshRenderer.h"
#include "PostEffectComposer.h"
#include "ShadowMap.h"
#include "TextRenderer.h"
#include "TTFont.h"

#include "Application.h"
#include "PlayScene.h"
#include "StartScene.h"

Application::Application()
{
	PlatformModule::WindowConstructParams windowParam{ L"CoinDash3D", 200, 200, 800, 600, false, false };

	ASSERT(CrashModule::Init() == CrashModule::Errors::OK, "Failed to initialize CrashModule.");
	ASSERT(PlatformModule::Init(windowParam) == PlatformModule::Errors::OK, "Failed to initialize PlatformModule.");
	ASSERT(AudioModule::Init() == AudioModule::Errors::OK, "Failed to initialize AudioModule.");
	ASSERT(RenderModule::Init(PlatformModule::GetWindowHandle()) == RenderModule::Errors::OK, "Failed to initialize RenderModule.");
	ASSERT(GameModule::Init() == GameModule::Errors::OK, "Failed to initialize GameModule.");

	PlatformModule::SetEndLoopCallback([&]() { RenderModule::Uninit(); });

	depthRenderer_ = RenderModule::CreateResource<DepthRenderer>();
	geometryRenderer2D_ = RenderModule::CreateResource<GeometryRenderer2D>();
	geometryRenderer3D_ = RenderModule::CreateResource<GeometryRenderer3D>();
	meshRenderer_ = RenderModule::CreateResource<MeshRenderer>();
	postEffectComposer_ = RenderModule::CreateResource<PostEffectComposer>();
	textRenderer_ = RenderModule::CreateResource<TextRenderer>();
	shadowMap_ = RenderModule::CreateResource<ShadowMap>(ShadowMap::ESize::Size_4096);

	std::string path = "Resource/Font/SeoulNamsanEB.ttf";
	std::array<int32_t, 6> fontSizes = { 16, 24, 32, 64, 72, 128, };
	for (const auto& fontSize : fontSizes)
	{
		TTFont* font = RenderModule::CreateResource<TTFont>(path, 0x0, 0x127, static_cast<float>(fontSize));
		fonts_.insert({ fontSize, font });
	}
}

Application::~Application()
{
	GameModule::Uninit();
	AudioModule::Uninit();
	PlatformModule::Uninit();
	CrashModule::Uninit();
}

void Application::Init()
{
	startScene_ = std::make_unique<StartScene>(this);
	playScene_ = std::make_unique<PlayScene>(this);

	StartScene* startScene = reinterpret_cast<StartScene*>(startScene_.get());
	PlayScene* playScene = reinterpret_cast<PlayScene*>(playScene_.get());

	startScene->SetPlayScene(playScene);
	playScene->SetStartScene(startScene);
}

void Application::Run()
{
	currentScene_ = startScene_.get();
	currentScene_->Enter();

	PlatformModule::RunLoop(
		[&](float deltaSeconds)
		{
			currentScene_->Tick(deltaSeconds);

			if (currentScene_->IsDetectSwitch())
			{
				currentScene_->Exit();
				currentScene_ = currentScene_->GetLink();
				currentScene_->Enter();
			}
		}
	);
}
