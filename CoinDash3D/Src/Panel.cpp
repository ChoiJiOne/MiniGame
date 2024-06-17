#include <json/json.h>

#include "Assertion.h"
#include "FileModule.h"
#include "GeometryRenderer2D.h"
#include "RenderModule.h"
#include "TextRenderer.h"

#include "Panel.h"
#include "StringUtils.h"

Panel::Panel(const std::string& path, TTFont* font, GeometryRenderer2D* geometryRenderer, TextRenderer* textRenderer)
	: font_(font)
	, geometryRenderer2D_(geometryRenderer)
	, textRenderer_(textRenderer)
{
	std::vector<uint8_t> jsonBuffer;
	ASSERT(FileModule::ReadFile(path, jsonBuffer) == FileModule::Errors::OK, "failed to read %s file", path.c_str());

	std::string jsonString(jsonBuffer.begin(), jsonBuffer.end());

	Json::Value root;
	Json::Reader reader;
	CHECK(reader.parse(jsonString, root));

	CHECK(root["width"].asFloat() && !root["width"].isNull());
	CHECK(root["height"].asFloat() && !root["height"].isNull());
	CHECK(root["side"].asFloat() && !root["side"].isNull());
	CHECK(root["x"].asFloat() && !root["x"].isNull());
	CHECK(root["y"].asFloat() && !root["y"].isNull());
	CHECK(root["text"].isString() && !root["text"].isNull());
	CHECK(root["outlineColor"].isArray() && !root["outlineColor"].isNull());
	CHECK(root["backgroundColor"].isArray() && !root["backgroundColor"].isNull());
	CHECK(root["textColor"].isArray() && !root["textColor"].isNull());

	layout_.width = root["width"].asFloat();
	layout_.height = root["height"].asFloat();
	layout_.side = root["side"].asFloat();
	layout_.center.x = root["x"].asFloat();
	layout_.center.y = root["y"].asFloat();
	layout_.text = StringUtils::Convert(std::string(root["text"].asCString()));
	layout_.outlineColor = Vec4f(root["outlineColor"][0].asFloat(), root["outlineColor"][1].asFloat(), root["outlineColor"][2].asFloat(), root["outlineColor"][3].asFloat());
	layout_.backgroundColor = Vec4f(root["backgroundColor"][0].asFloat(), root["backgroundColor"][1].asFloat(), root["backgroundColor"][2].asFloat(), root["backgroundColor"][3].asFloat());
	layout_.textColor = Vec4f(root["textColor"][0].asFloat(), root["textColor"][1].asFloat(), root["textColor"][2].asFloat(), root["textColor"][3].asFloat());

	bIsInitialized_ = true;
}

Panel::Panel(const Layout& layout, TTFont* font)
	: layout_(layout)
	, font_(font)
{
	bIsInitialized_ = true;
}

Panel::~Panel()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Panel::Tick(float deltaSeconds)
{
}

void Panel::Render()
{
	geometryRenderer2D_->DrawRoundRectangle2D(layout_.center, layout_.width, layout_.height, layout_.side, 0.0f, layout_.backgroundColor);
	geometryRenderer2D_->DrawWireframeRoundRectangle2D(layout_.center, layout_.width, layout_.height, layout_.side, 0.0f, layout_.outlineColor);
	if (!layout_.text.empty())
	{
		textRenderer_->DrawText2D(font_, layout_.text, layout_.center, EStartPivot::CENTER, layout_.textColor);
	}
}

void Panel::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}