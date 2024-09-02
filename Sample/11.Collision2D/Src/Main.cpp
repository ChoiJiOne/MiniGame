#include <cstdint>
#include <Windows.h>

#include <imgui.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "Collision2D.h"
#include "IApp.h"
#include "RenderManager2D.h"
#include "ResourceManager.h"
#include "TTFont.h"

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("11.Collision2D", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		line_ = Line2D(GameMath::Vec2f(-100.0f, -200.0f), GameMath::Vec2f(100.0f, -10.0f));
		circle_ = Circle2D(GameMath::Vec2f(270.0f, 150.0f), 50.0f);
		aabb_ = Rect2D(GameMath::Vec2f(-270.0f, -150.0f), GameMath::Vec2f(200.0f, 100.0f));
		obb_ = OrientedRect2D(GameMath::Vec2f(270.0f, -150.0f), GameMath::Vec2f(200.0f, 100.0f), GameMath::ToRadian(45.0f));
	}

	virtual void Shutdown() override
	{
	}

	virtual void Run() override
	{
		float minX = -400.0f;
		float maxX = +400.0f;
		float strideX = 10.0f;
		float minY = -300.0f;
		float maxY = +300.0f;
		float strideY = 10.0f;

		RunLoop(
			[&](float deltaSeconds)
			{
				GameMath::Vec2i currPos = GetCurrMousePos();

				GetScreenSize<float>(screenSize_.x, screenSize_.y);
				currPos_.x = -screenSize_.x * 0.5f + static_cast<float>(currPos.x);
				currPos_.y = +screenSize_.y * 0.5f - static_cast<float>(currPos.y);

				ImGui::Begin("Collision", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				{
					ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
					ImGui::SetWindowSize(ImVec2(400.0f, 150.0f));

					ImGui::RadioButton("1.POINT", &select_, 0);
					ImGui::RadioButton("2.LINE ", &select_, 1);
					ImGui::RadioButton("3.CIRCLE", &select_, 2);
					ImGui::RadioButton("4.AABB", &select_, 3);
					ImGui::RadioButton("5.OBB", &select_, 4);
				}
				ImGui::End();

				switch (select_)
				{
				case 0:
					targetPoint_.center = currPos_;
					targetCollision_ = &targetPoint_;
					break;
				case 1:
					targetLine_.start = currPos_ + GameMath::Vec2f(50.0f, 25.0f);
					targetLine_.end = currPos_ + GameMath::Vec2f(-50.0f, -25.0f);
					targetCollision_ = &targetLine_;
					break;
				case 2:
					targetCircle_.center = currPos_;
					targetCircle_.radius = 50.0f;
					targetCollision_ = &targetCircle_;
					break;
				case 3:
					targetAABB_.center = currPos_;
					targetAABB_.size = GameMath::Vec2f(50.0f, 25.0f);
					targetCollision_ = &targetAABB_;
					break;
				case 4:
					targetOBB_.center = currPos_;
					targetOBB_.size = GameMath::Vec2f(50.0f, 25.0f);
					targetOBB_.rotate = GameMath::ToRadian(30.0f);
					targetCollision_ = &targetOBB_;
					break;
				}
				
				BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

				RenderManager2D::Get().Begin();
				{

					for (float x = minX; x <= maxX; x += strideX)
					{
						GameMath::Vec4f color = (x == 0.0f) ? GameMath::Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : GameMath::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
						RenderManager2D::Get().DrawLine(GameMath::Vec2f(x, minX), GameMath::Vec2f(x, maxY), color);
					}

					for (float y = minY; y <= maxY; y += strideY)
					{
						GameMath::Vec4f color = (y == 0.0f) ? GameMath::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMath::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
						RenderManager2D::Get().DrawLine(GameMath::Vec2f(minX, y), GameMath::Vec2f(maxX, y), color);
					}

					switch (select_)
					{
					case 0:
						RenderManager2D::Get().DrawPoint(targetPoint_.center, GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
						break;
					case 1:
						RenderManager2D::Get().DrawLine(targetLine_.start, targetLine_.end, GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
						break;
					case 2:
						RenderManager2D::Get().DrawCircleWireframe(targetCircle_.center, targetCircle_.radius, GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
						break;
					case 3:
						RenderManager2D::Get().DrawRectWireframe(targetAABB_.center, targetAABB_.size.x, targetAABB_.size.y, GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
						break;
					case 4:
						RenderManager2D::Get().DrawRectWireframe(targetOBB_.center, targetOBB_.size.x, targetOBB_.size.y, GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f), targetOBB_.rotate);
						break;
					}

					RenderManager2D::Get().DrawLine(line_.start, line_.end, line_.Intersect(targetCollision_) ? GameMath::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMath::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
					RenderManager2D::Get().DrawCircleWireframe(circle_.center, circle_.radius, circle_.Intersect(targetCollision_) ? GameMath::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMath::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
					RenderManager2D::Get().DrawRectWireframe(aabb_.center, aabb_.size.x, aabb_.size.y, aabb_.Intersect(targetCollision_) ? GameMath::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMath::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
					RenderManager2D::Get().DrawRectWireframe(obb_.center, obb_.size.x, obb_.size.y, obb_.Intersect(targetCollision_) ? GameMath::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMath::Vec4f(0.0f, 0.0f, 1.0f, 1.0f), obb_.rotate);
				}
				RenderManager2D::Get().End();

				EndFrame();
			}
		);
	}

private:
	int32_t select_ = 0;
	GameMath::Vec2f screenSize_;
	GameMath::Vec2f currPos_;

	ICollision2D* targetCollision_ = nullptr;
	Point2D targetPoint_;
	Line2D targetLine_;
	Circle2D targetCircle_;
	Rect2D targetAABB_;
	OrientedRect2D targetOBB_;

	Line2D line_;
	Circle2D circle_;
	Rect2D aabb_;
	OrientedRect2D obb_;
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