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
					selectPoint_.center = currPos_;
					current_ = &selectPoint_;
					break;
				case 1:
					selectLine_.start = currPos_ + GameMath::Vec2f(50.0f, 25.0f);
					selectLine_.end = currPos_ + GameMath::Vec2f(-50.0f, -25.0f);
					current_ = &selectLine_;
					break;
				case 2:
					selectCircle_.center = currPos_;
					selectCircle_.radius = 50.0f;
					current_ = &selectCircle_;
					break;
				case 3:
					selectAABB_.center = currPos_;
					selectAABB_.size = GameMath::Vec2f(50.0f, 25.0f);
					current_ = &selectAABB_;
					break;
				case 4:
					selectOBB_.center = currPos_;
					selectOBB_.size = GameMath::Vec2f(50.0f, 25.0f);
					selectOBB_.rotate = GameMath::ToRadian(30.0f);
					current_ = &selectOBB_;
					break;
				}
				
				BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

				RenderManager2D::Get().Begin();
				{
					DrawGrid();
					
					switch (select_)
					{
					case 0:
						RenderManager2D::Get().DrawPoint(selectPoint_.center, white_);
						break;
					case 1:
						RenderManager2D::Get().DrawLine(selectLine_.start, selectLine_.end, white_);
						break;
					case 2:
						RenderManager2D::Get().DrawCircleWireframe(selectCircle_.center, selectCircle_.radius, white_);
						break;
					case 3:
						RenderManager2D::Get().DrawRectWireframe(selectAABB_.center, selectAABB_.size.x, selectAABB_.size.y, white_);
						break;
					case 4:
						RenderManager2D::Get().DrawRectWireframe(selectOBB_.center, selectOBB_.size.x, selectOBB_.size.y, white_, selectOBB_.rotate);
						break;
					}

					RenderManager2D::Get().DrawLine(line_.start, line_.end, line_.Intersect(current_) ? red_ : blue_);
					RenderManager2D::Get().DrawCircleWireframe(circle_.center, circle_.radius, circle_.Intersect(current_) ? red_ : blue_);
					RenderManager2D::Get().DrawRectWireframe(aabb_.center, aabb_.size.x, aabb_.size.y, aabb_.Intersect(current_) ? red_ : blue_);
					RenderManager2D::Get().DrawRectWireframe(obb_.center, obb_.size.x, obb_.size.y, obb_.Intersect(current_) ? red_ : blue_, obb_.rotate);
				}
				RenderManager2D::Get().End();

				EndFrame();
			}
		);
	}

private:
	void DrawGrid()
	{
		for (float x = minX_; x <= maxX_; x += strideX_)
		{
			GameMath::Vec4f color = (x == 0.0f) ? red_ : gray_;
			RenderManager2D::Get().DrawLine(GameMath::Vec2f(x, minX_), GameMath::Vec2f(x, maxY_), color);
		}

		for (float y = minY_; y <= maxY_; y += strideY_)
		{
			GameMath::Vec4f color = (y == 0.0f) ? blue_ : gray_;
			RenderManager2D::Get().DrawLine(GameMath::Vec2f(minX_, y), GameMath::Vec2f(maxX_, y), color);
		}
	}
	
private:
	float minX_ = -400.0f;
	float maxX_ = +400.0f;
	float strideX_ = 10.0f;
	float minY_ = -300.0f;
	float maxY_ = +300.0f;
	float strideY_ = 10.0f;

	int32_t select_ = 0;
	GameMath::Vec2f screenSize_;
	GameMath::Vec2f currPos_;

	ICollision2D* current_ = nullptr;
	Point2D selectPoint_;
	Line2D selectLine_;
	Circle2D selectCircle_;
	Rect2D selectAABB_;
	OrientedRect2D selectOBB_;

	Line2D line_;
	Circle2D circle_;
	Rect2D aabb_;
	OrientedRect2D obb_;

	GameMath::Vec4f red_ = GameMath::Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
	GameMath::Vec4f blue_ = GameMath::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
	GameMath::Vec4f gray_ = GameMath::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
	GameMath::Vec4f white_ = GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
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