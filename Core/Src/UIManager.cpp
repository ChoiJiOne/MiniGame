#include <json/json.h>

#include "Assertion.h"
#include "ButtonUI.h"
#include "Camera2D.h"
#include "EntityManager.h"
#include "PanelUI.h"
#include "RenderManager2D.h"
#include "TextUI.h"
#include "UIManager.h"

UIManager& UIManager::Get()
{
	static UIManager instance;
	return instance;
}

ButtonUI* UIManager::CreateButtonUI(const std::string& path, const Mouse& mouse, TTFont* font, const std::function<void()>& clickEvent)
{
	std::vector<uint8_t> buffer = GameUtils::ReadFile(path);
	std::string jsonString(buffer.begin(), buffer.end());

	Json::Value root;
	Json::Reader reader;
	ASSERT(reader.parse(jsonString, root), "Failed to parse '%s' file.", path.c_str());
	
	CHECK(!root["type"].isNull() && root["type"].isString());
	std::string type = root["type"].asString();
	CHECK(type == "button");

	auto getColorFromJson = [&](const std::string& color, Vec4f& outColor)
		{
			CHECK(!root[color].isNull());
			CHECK(!root[color]["r"].isNull() && !root[color]["g"].isNull() && !root[color]["b"].isNull() && !root[color]["a"].isNull());
			CHECK(root[color]["r"].isDouble() && root[color]["g"].isDouble() && root[color]["b"].isDouble() && root[color]["a"].isDouble());

			outColor = Vec4f(root[color]["r"].asFloat(), root[color]["g"].asFloat(), root[color]["b"].asFloat(), root[color]["a"].asFloat());
		};

	Vec4f textColor;
	getColorFromJson("textColor", textColor);

	Vec4f disableColor;
	getColorFromJson("disableColor", disableColor);

	Vec4f enableColor;
	getColorFromJson("enableColor", enableColor);

	Vec4f pressColor;
	getColorFromJson("pressColor", pressColor);

	Vec4f releaseColor;
	getColorFromJson("releaseColor", releaseColor);

	CHECK(!root["center"].isNull() && !root["center"]["x"].isNull() && !root["center"]["y"].isNull());
	CHECK(root["center"]["x"].isDouble() && root["center"]["y"].isDouble());
	Vec2f center(root["center"]["x"].asFloat(), root["center"]["y"].asFloat());

	CHECK(!root["size"].isNull() && !root["size"]["w"].isNull() && !root["size"]["h"].isNull());
	CHECK(root["size"]["w"].isDouble() && root["size"]["h"].isDouble());
	Vec2f size(root["size"]["w"].asFloat(), root["size"]["h"].asFloat());

	CHECK(!root["text"].isNull() && root["text"].isString());
	std::string text = root["text"].asString();

	CHECK(!root["side"].isNull() && root["side"].isDouble());
	float side = root["side"].asFloat();

	ButtonUI::Layout layout;
	layout.textColor = textColor;
	layout.disableColor = disableColor;
	layout.enableColor = enableColor;
	layout.pressColor = pressColor;
	layout.releaseColor = releaseColor;
	layout.center = center;
	layout.size = size;
	layout.mouse = mouse;
	layout.font = font;
	layout.text = GameUtils::Convert(text);
	layout.side = side;

	return EntityManager::Get().Create<ButtonUI>(layout, clickEvent);
}

PanelUI* UIManager::CreatePanelUI(const std::string& path, TTFont* font)
{
	std::vector<uint8_t> buffer = GameUtils::ReadFile(path);
	std::string jsonString(buffer.begin(), buffer.end());

	Json::Value root;
	Json::Reader reader;
	ASSERT(reader.parse(jsonString, root), "Failed to parse '%s' file.", path.c_str());

	CHECK(!root["type"].isNull() && root["type"].isString());
	std::string type = root["type"].asString();
	CHECK(type == "panel");

	auto getColorFromJson = [&](const std::string& color, Vec4f& outColor)
		{
			CHECK(!root[color].isNull());
			CHECK(!root[color]["r"].isNull() && !root[color]["g"].isNull() && !root[color]["b"].isNull() && !root[color]["a"].isNull());
			CHECK(root[color]["r"].isDouble() && root[color]["g"].isDouble() && root[color]["b"].isDouble() && root[color]["a"].isDouble());

			outColor = Vec4f(root[color]["r"].asFloat(), root[color]["g"].asFloat(), root[color]["b"].asFloat(), root[color]["a"].asFloat());
		};

	Vec4f backgroundColor;
	getColorFromJson("backgroundColor", backgroundColor);

	Vec4f outlineColor;
	getColorFromJson("outlineColor", outlineColor);

	Vec4f textColor;
	getColorFromJson("textColor", textColor);
	
	CHECK(!root["center"].isNull() && !root["center"]["x"].isNull() && !root["center"]["y"].isNull());
	CHECK(root["center"]["x"].isDouble() && root["center"]["y"].isDouble());
	Vec2f center(root["center"]["x"].asFloat(), root["center"]["y"].asFloat());

	CHECK(!root["size"].isNull() && !root["size"]["w"].isNull() && !root["size"]["h"].isNull());
	CHECK(root["size"]["w"].isDouble() && root["size"]["h"].isDouble());
	Vec2f size(root["size"]["w"].asFloat(), root["size"]["h"].asFloat());

	CHECK(!root["text"].isNull() && root["text"].isString());
	std::string text = root["text"].asString();

	CHECK(!root["side"].isNull() && root["side"].isDouble());
	float side = root["side"].asFloat();

	PanelUI::Layout layout;
	layout.backgroundColor = backgroundColor;
	layout.outlineColor = outlineColor;
	layout.textColor = textColor;
	layout.center = center;
	layout.size = size;
	layout.font = font;
	layout.text = GameUtils::Convert(text);
	layout.side = side;

	return EntityManager::Get().Create<PanelUI>(layout);
}

TextUI* UIManager::CreateTextUI(const std::string& path, TTFont* font)
{
	std::vector<uint8_t> buffer = GameUtils::ReadFile(path);
	std::string jsonString(buffer.begin(), buffer.end());

	Json::Value root;
	Json::Reader reader;
	ASSERT(reader.parse(jsonString, root), "Failed to parse '%s' file.", path.c_str());

	CHECK(!root["type"].isNull() && root["type"].isString());
	std::string type = root["type"].asString();
	CHECK(type == "text");

	auto getColorFromJson = [&](const std::string& color, Vec4f& outColor)
		{
			CHECK(!root[color].isNull());
			CHECK(!root[color]["r"].isNull() && !root[color]["g"].isNull() && !root[color]["b"].isNull() && !root[color]["a"].isNull());
			CHECK(root[color]["r"].isDouble() && root[color]["g"].isDouble() && root[color]["b"].isDouble() && root[color]["a"].isDouble());

			outColor = Vec4f(root[color]["r"].asFloat(), root[color]["g"].asFloat(), root[color]["b"].asFloat(), root[color]["a"].asFloat());
		};

	CHECK(!root["text"].isNull() && root["text"].isString());
	std::string text = root["text"].asString();

	Vec4f textColor;
	getColorFromJson("textColor", textColor);

	CHECK(!root["center"].isNull() && !root["center"]["x"].isNull() && !root["center"]["y"].isNull());
	CHECK(root["center"]["x"].isDouble() && root["center"]["y"].isDouble());
	Vec2f center(root["center"]["x"].asFloat(), root["center"]["y"].asFloat());

	TextUI::Layout layout;
	layout.textColor = textColor;
	layout.textCenterPos = center;
	layout.font = font;
	layout.text = GameUtils::Convert(text);

	return EntityManager::Get().Create<TextUI>(layout);
}

void UIManager::BatchTickUIEntity(IEntityUI** entities, uint32_t count, float deltaSeconds)
{
	if (count == 0)
	{
		return;
	}

	for (uint32_t index = 0; index < count; ++index)
	{
		entities[index]->Tick(deltaSeconds);
	}
}

void UIManager::BatchRenderUIEntity(IEntityUI** entities, uint32_t count)
{
	if (count == 0)
	{
		return;
	}

	RenderManager2D& renderMgr = RenderManager2D::Get();
	renderMgr.Begin(uiCamera_);
	{
		for (uint32_t index = 0; index < count; ++index)
		{
			const IEntityUI::Type& type = entities[index]->GetType();
			switch (type)
			{
			case IEntityUI::Type::TEXT:
			{
				TextUI* text = reinterpret_cast<TextUI*>(entities[index]);
				renderMgr.DrawString(text->layout_.font, text->layout_.text, text->textPos_, text->layout_.textColor);
			}
			break;

			case IEntityUI::Type::PANEL:
			{
				PanelUI* panel = reinterpret_cast<PanelUI*>(entities[index]);
				renderMgr.DrawRoundRect(panel->layout_.center, panel->layout_.size.x, panel->layout_.size.y, panel->layout_.side, panel->layout_.backgroundColor, 0.0f);
				renderMgr.DrawRoundRectWireframe(panel->layout_.center, panel->layout_.size.x, panel->layout_.size.y, panel->layout_.side, panel->layout_.outlineColor, 0.0f);
				renderMgr.DrawString(panel->layout_.font, panel->layout_.text, panel->textPos_, panel->layout_.textColor);
			}
			break;

			case IEntityUI::Type::BUTTON:
			{
				ButtonUI* button = reinterpret_cast<ButtonUI*>(entities[index]);
				const Rect2D& bound = button->bound_;
				const ButtonUI::Layout& layout = button->layout_;
				const Vec4f& color = button->stateColors_.at(button->state_);

				renderMgr.DrawRoundRect(bound.center, bound.size.x, bound.size.y, layout.side, color, 0.0f);
				renderMgr.DrawString(layout.font, layout.text, button->textPos_, layout.textColor);
			}
			break;
			}
		}
	}
	renderMgr.End();
}

void UIManager::Startup()
{
	uiCamera_ = Camera2D::CreateScreenCamera();
}

void UIManager::Shutdown()
{
	EntityManager::Get().Destroy(uiCamera_);
	uiCamera_ = nullptr;
}