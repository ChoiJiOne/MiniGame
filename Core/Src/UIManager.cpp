#include <json/json.h>

#include "Assertion.h"
#include "ButtonUI.h"
#include "Camera2D.h"
#include "EntityManager.h"
#include "UIManager.h"

UIManager& UIManager::Get()
{
	static UIManager instance;
	return instance;
}

ButtonUI* UIManager::Create(const std::string& path, const Mouse& mouse, TTFont* font, const std::function<void()>& clickEvent)
{
	std::vector<uint8_t> buffer = GameUtils::ReadFile(path);
	std::string jsonString(buffer.begin(), buffer.end());

	Json::Value root;
	Json::Reader reader;
	ASSERT(reader.parse(jsonString, root), "Failed to parse '%s' file.", path.c_str());

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

void UIManager::Startup()
{
	uiCamera_ = Camera2D::CreateScreenCamera();
}

void UIManager::Shutdown()
{
	EntityManager::Get().Destroy(uiCamera_);
	uiCamera_ = nullptr;
}