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
	enum class Type
	{
		LINE   = 0x00,
		CIRCLE = 0x01,
		AABB   = 0x02,
		OBB    = 0x03,
	};

public:
	DemoApp() : IApp("11.Collision2D", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		line0_ = Line2D(Vec2f(-100.0f, -200.0f), Vec2f(100.0f, -10.0f));
		circle0_ = Circle2D(Vec2f(270.0f, 150.0f), 50.0f);
		aabb0_ = Rect2D(Vec2f(-270.0f, -150.0f), Vec2f(200.0f, 100.0f));
		obb0_ = OrientedRect2D(Vec2f(270.0f, -150.0f), Vec2f(200.0f, 100.0f), GameMath::ToRadian(45.0f));

		line1_ = Line2D(Vec2f(-200.0f, -50.0f), Vec2f(200.0f, 50.0f));
		circle1_ = Circle2D(Vec2f(0.0f, 0.0f), 150.0f);
		aabb1_ = Rect2D(Vec2f(0.0f, 0.0f), Vec2f(220.0f, 170.0f));
		obb1_ = OrientedRect2D(Vec2f(0.0f, 0.0f), Vec2f(220.0f, 170.0f), GameMath::ToRadian(130.0f));

		collisions0_ = { &line0_, &circle0_, &aabb0_, &obb0_, };
		collisions1_ = { &line1_, &circle1_, &aabb1_, &obb1_, };
	}

	virtual void Shutdown() override
	{
	}

	virtual void Run() override
	{
		RunLoop(
			[&](float deltaSeconds)
			{
				Vec2i currPos = GetCurrMousePos();
				GetScreenSize<float>(screenSize_.x, screenSize_.y);
				currPos_.x = -screenSize_.x * 0.5f + static_cast<float>(currPos.x);
				currPos_.y = +screenSize_.y * 0.5f - static_cast<float>(currPos.y);

				ImGui::Begin("Collision", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				{
					ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
					ImGui::SetWindowSize(ImVec2(350.0f, 120.0f));
					SelectObjectGUI("OBJECT0", &select0_, red_);
					SelectObjectGUI("OBJECT1", &select1_, blue_);
					RotateObjectGUI("OBJECT0", &select0_, collisions0_[select0_]);
					RotateObjectGUI("OBJECT1", &select1_, collisions1_[select1_]);
				}
				ImGui::End();

				UpdateObject(collisions0_[select0_]);

				BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

				RenderManager2D::Get().Begin();
				{
					DrawGrid();

					if (collisions0_[select0_]->Intersect(collisions1_[select1_]))
					{
						DrawCollision(collisions0_[select0_], green_);
						DrawCollision(collisions1_[select1_], green_);
					}
					else
					{
						DrawCollision(collisions0_[select0_], red_);
						DrawCollision(collisions1_[select1_], blue_);
					}
				}
				RenderManager2D::Get().End();

				EndFrame();
			}
		);
	}

private:
	void SelectObjectGUI(const char* name, int32_t* selectPtr, const Vec4f& color)
	{
		ImGui::TextColored(ImVec4(color.x, color.y, color.z, color.w), name);
		ImGui::RadioButton(GameUtils::PrintF("LINE##%s", name).c_str(), selectPtr, static_cast<int32_t>(Type::LINE));
		ImGui::SameLine();
		ImGui::RadioButton(GameUtils::PrintF("CIRCLE##%s", name).c_str(), selectPtr, static_cast<int32_t>(Type::CIRCLE));
		ImGui::SameLine();
		ImGui::RadioButton(GameUtils::PrintF("AABB##%s", name).c_str(), selectPtr, static_cast<int32_t>(Type::AABB));
		ImGui::SameLine();
		ImGui::RadioButton(GameUtils::PrintF("OBB##%s", name).c_str(), selectPtr, static_cast<int32_t>(Type::OBB));
	}

	void RotateObjectGUI(const char* name, int32_t* selectPtr, ICollision2D* collision)
	{
		if (*selectPtr != 3)
		{
			return;
		}

		OrientedRect2D* orientedRect = reinterpret_cast<OrientedRect2D*>(collision);
		ImGui::SliderFloat(GameUtils::PrintF("%s##%s", name, name).c_str(), &orientedRect->rotate, 0.0f, GameMath::ToRadian(360.0f));
	}

	void UpdateObject(ICollision2D* collision)
	{
		ICollision2D::Type type = collision->GetType();
		switch (type)
		{
		case ICollision2D::Type::POINT:
		{
			Point2D* point = reinterpret_cast<Point2D*>(collision);
			point->center = currPos_;
		}
		break;

		case ICollision2D::Type::LINE:
		{
			Line2D* line = reinterpret_cast<Line2D*>(collision);
			line->start = currPos_ + Vec2f(50.0f, 25.0f);
			line->end = currPos_ + Vec2f(-50.0f, -25.0f);
		}
		break;

		case ICollision2D::Type::CIRCLE:
		{
			Circle2D* circle = reinterpret_cast<Circle2D*>(collision);
			circle->center = currPos_;
			circle->radius = 50.0f;
		}
		break;

		case ICollision2D::Type::RECT:
		{
			Rect2D* rect = reinterpret_cast<Rect2D*>(collision);
			rect->center = currPos_;
			rect->size = Vec2f(50.0f, 25.0f);
		}
		break;

		case ICollision2D::Type::ORIENTED_RECT:
		{
			OrientedRect2D* orientedRect = reinterpret_cast<OrientedRect2D*>(collision);
			orientedRect->center = currPos_;
			orientedRect->size = Vec2f(50.0f, 25.0f);
		}
		break;
		}
	}

	void DrawGrid()
	{
		Vec4f color;

		for (float x = minX_; x <= maxX_; x += strideX_)
		{
			color = (x == 0.0f) ? red_ : gray_;
			RenderManager2D::Get().DrawLine(Vec2f(x, minX_), Vec2f(x, maxY_), color);
		}

		for (float y = minY_; y <= maxY_; y += strideY_)
		{
			color = (y == 0.0f) ? blue_ : gray_;
			RenderManager2D::Get().DrawLine(Vec2f(minX_, y), Vec2f(maxX_, y), color);
		}
	}

	void DrawCollision(ICollision2D* collision, const Vec4f& color)
	{
		ICollision2D::Type type = collision->GetType();
		switch (type)
		{
		case ICollision2D::Type::POINT:
		{
			Point2D* point = reinterpret_cast<Point2D*>(collision);
			RenderManager2D::Get().DrawPoint(point->center, color);
		}
		break;

		case ICollision2D::Type::LINE:
		{
			Line2D* line = reinterpret_cast<Line2D*>(collision);
			RenderManager2D::Get().DrawLine(line->start, line->end, color);
		}
		break;

		case ICollision2D::Type::CIRCLE:
		{
			Circle2D* circle = reinterpret_cast<Circle2D*>(collision);
			RenderManager2D::Get().DrawCircleWireframe(circle->center, circle->radius, color);
		}
		break;

		case ICollision2D::Type::RECT:
		{
			Rect2D* rect = reinterpret_cast<Rect2D*>(collision);
			RenderManager2D::Get().DrawRectWireframe(rect->center, rect->size.x, rect->size.y, color);
		}
		break;

		case ICollision2D::Type::ORIENTED_RECT:
		{
			OrientedRect2D* orientedRect = reinterpret_cast<OrientedRect2D*>(collision);
			RenderManager2D::Get().DrawRectWireframe(orientedRect->center, orientedRect->size.x, orientedRect->size.y, color, orientedRect->rotate);
		}
		break;
		}
	}
	
private:
	float minX_ = -400.0f;
	float maxX_ = +400.0f;
	float strideX_ = 10.0f;
	float minY_ = -300.0f;
	float maxY_ = +300.0f;
	float strideY_ = 10.0f;

	Vec2f screenSize_;
	Vec2f currPos_;

	Vec4f red_ = Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
	Vec4f blue_ = Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
	Vec4f green_ = Vec4f(0.0f, 1.0f, 0.0f, 1.0f);
	Vec4f gray_ = Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
	Vec4f white_ = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

	int32_t select0_ = 0;
	std::array<ICollision2D*, 4> collisions0_;
	Line2D line0_;
	Circle2D circle0_;
	Rect2D aabb0_;
	OrientedRect2D obb0_;

	int32_t select1_ = 0;
	std::array<ICollision2D*, 4> collisions1_;
	Line2D line1_;
	Circle2D circle1_;
	Rect2D aabb1_;
	OrientedRect2D obb1_;
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