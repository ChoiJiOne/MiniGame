#include <json/json.h>

#include "Assertion.h"
#include "ButtonUI.h"
#include "Camera2D.h"
#include "EntityManager.h"
#include "GameUtils.h"
#include "PanelUI.h"
#include "RenderManager2D.h"
#include "TextUI.h"
#include "UIManager.h"

UIManager UIManager::instance_;

bool ReadJsonFile(const std::string& path, Json::Value& outRoot, std::string& outResult)
{
	bool bSucceed = false;

	std::vector<uint8_t> buffer = GameUtils::ReadFile(path);
	std::string jsonString(buffer.begin(), buffer.end());

	Json::Reader reader;
	bSucceed = reader.parse(jsonString, outRoot);
	outResult = GameUtils::PrintF("%s to parse '%s' json file.", bSucceed ? "Succeed" : "Failed", path.c_str());

	return bSucceed;
}

bool GetStringFromJson(const Json::Value& root, const std::string& name, std::string& outName)
{
	bool bIsInValid = root["type"].isNull();
	bool bIsStringInvalid = !root["type"].isString();
	if (bIsInValid || bIsStringInvalid)
	{
		return false;
	}

	outName = root[name].asString();
	return true;
}

bool GetWStringFromJson(const Json::Value& root, const std::string& name, std::wstring& outName)
{
	bool bIsInValid = root["type"].isNull();
	bool bIsStringInvalid = !root["type"].isString();
	if (bIsInValid || bIsStringInvalid)
	{
		return false;
	}

	outName = GameUtils::Convert(root[name].asString());
	return true;
}

bool GetColorFromJson(const Json::Value& root, const std::string& name, Vec4f& outColor)
{
	bool bIsInvalid = root[name].isNull() || root[name]["r"].isNull() || root[name]["g"].isNull() || root[name]["b"].isNull() || root[name]["a"].isNull();
	bool bIsColorInvalid = !root[name]["r"].isDouble() || !root[name]["g"].isDouble() || !root[name]["b"].isDouble() || !root[name]["a"].isDouble();
	if (bIsInvalid || bIsColorInvalid)
	{
		return false;
	}

	float r = root[name]["r"].asFloat();
	float g = root[name]["g"].asFloat();
	float b = root[name]["b"].asFloat();
	float a = root[name]["a"].asFloat();
	outColor = Vec4f(r, g, b, a);

	return true;
}

bool GetVec2FromJson(const Json::Value& root, const std::string& name, Vec2f& outVec2)
{
	bool bIsInvalid = root[name].isNull() || root[name]["x"].isNull() || root[name]["y"].isNull();
	bool bIsVec2Invalid = !root[name]["x"].isDouble() || !root[name]["y"].isDouble();
	if (bIsInvalid || bIsVec2Invalid)
	{
		return false;
	}

	float x = root[name]["x"].asFloat();
	float y = root[name]["y"].asFloat();
	outVec2 = Vec2f(x, y);

	return true;
}

bool GetFloatFromJson(const Json::Value& root, const std::string& name, float& outFloat)
{
	bool bIsInvalid = root[name].isNull();
	bool bIsFloatInvalid = !root[name].isDouble();
	if (bIsInvalid || bIsFloatInvalid)
	{
		return false;
	}

	outFloat = root[name].asFloat();
	return true;
}

UIManager& UIManager::GetRef()
{
	return instance_;
}

UIManager* UIManager::GetPtr()
{
	return &instance_;
}

ButtonUI* UIManager::CreateButtonUI(const std::string& path, const Mouse& mouse, TTFont* font, const std::function<void()>& clickEvent)
{
	Json::Value root;
	std::string message;
	bool bSucceed = ReadJsonFile(path, root, message);
	ASSERT(bSucceed, "%s", message.c_str());

	std::string type;
	CHECK(GetStringFromJson(root, "type", type) && type == "button");

	ButtonUI::Layout layout;
	layout.mouse = mouse;
	layout.font = font;

	CHECK(GetColorFromJson(root, "textColor", layout.textColor));
	CHECK(GetColorFromJson(root, "disableColor", layout.disableColor));
	CHECK(GetColorFromJson(root, "enableColor", layout.enableColor));
	CHECK(GetColorFromJson(root, "pressColor", layout.pressColor));
	CHECK(GetColorFromJson(root, "releaseColor", layout.releaseColor));
	CHECK(GetVec2FromJson(root, "center", layout.center));
	CHECK(GetVec2FromJson(root, "size", layout.size));
	CHECK(GetWStringFromJson(root, "text", layout.text));
	CHECK(GetFloatFromJson(root, "side", layout.side));

	return entityMgr_->Create<ButtonUI>(layout, clickEvent);
}

PanelUI* UIManager::CreatePanelUI(const std::string& path, TTFont* font)
{
	Json::Value root;
	std::string message;
	bool bSucceed = ReadJsonFile(path, root, message);
	ASSERT(bSucceed, "%s", message.c_str());

	std::string type;
	CHECK(GetStringFromJson(root, "type", type) && type == "panel");

	PanelUI::Layout layout;
	layout.font = font;

	CHECK(GetColorFromJson(root, "backgroundColor", layout.backgroundColor));
	CHECK(GetColorFromJson(root, "outlineColor", layout.outlineColor));
	CHECK(GetColorFromJson(root, "textColor", layout.textColor));
	CHECK(GetVec2FromJson(root, "center", layout.center));
	CHECK(GetVec2FromJson(root, "size", layout.size));
	CHECK(GetWStringFromJson(root, "text", layout.text));
	CHECK(GetFloatFromJson(root, "side", layout.side));

	return entityMgr_->Create<PanelUI>(layout);
}

TextUI* UIManager::CreateTextUI(const std::string& path, TTFont* font)
{
	Json::Value root;
	std::string message;
	bool bSucceed = ReadJsonFile(path, root, message);
	ASSERT(bSucceed, "%s", message.c_str());

	std::string type;
	CHECK(GetStringFromJson(root, "type", type) && type == "text");

	TextUI::Layout layout;
	layout.font = font;

	CHECK(GetColorFromJson(root, "textColor", layout.textColor));
	CHECK(GetVec2FromJson(root, "center", layout.textCenterPos));
	CHECK(GetWStringFromJson(root, "text", layout.text));

	return entityMgr_->Create<TextUI>(layout);
}

void UIManager::BatchRenderUIEntity(IEntityUI** entities, uint32_t count)
{
	if (count == 0)
	{
		return;
	}

	render2dMgr_->Begin(uiCamera_);
	{
		PassRoundRect(entities, count);
		PassRoundRectWireframe(entities, count);
		PassString(entities, count);
	}
	render2dMgr_->End();
}

void UIManager::Startup()
{
	entityMgr_ = EntityManager::GetPtr();
	render2dMgr_ = RenderManager2D::GetPtr();
	uiCamera_ = Camera2D::CreateScreenCamera();
}

void UIManager::Shutdown()
{
	entityMgr_->Destroy(uiCamera_);
	uiCamera_ = nullptr;

	render2dMgr_ = nullptr;
	entityMgr_ = nullptr;
}

void UIManager::PassRoundRect(IEntityUI** entities, uint32_t count)
{
	for (uint32_t index = 0; index < count; ++index)
	{
		const IEntityUI::Type& type = entities[index]->GetType();
		switch (type)
		{
		case IEntityUI::Type::TEXT:
			// Nothing...
			break;

		case IEntityUI::Type::PANEL:
		{
			PanelUI* panel = reinterpret_cast<PanelUI*>(entities[index]);
			render2dMgr_->DrawRoundRect(panel->layout_.center, panel->layout_.size.x, panel->layout_.size.y, panel->layout_.side, panel->layout_.backgroundColor, 0.0f);
		}
		break;

		case IEntityUI::Type::BUTTON:
		{
			ButtonUI* button = reinterpret_cast<ButtonUI*>(entities[index]);
			const Rect2D& bound = button->bound_;
			const ButtonUI::Layout& layout = button->layout_;
			const Vec4f& color = button->stateColors_.at(button->state_);

			render2dMgr_->DrawRoundRect(bound.center, bound.size.x, bound.size.y, layout.side, color, 0.0f);
		}
		break;
		}
	}
}

void UIManager::PassRoundRectWireframe(IEntityUI** entities, uint32_t count)
{
	for (uint32_t index = 0; index < count; ++index)
	{
		const IEntityUI::Type& type = entities[index]->GetType();
		switch (type)
		{
		case IEntityUI::Type::TEXT:
			// Nothing...
			break;

		case IEntityUI::Type::PANEL:
		{
			PanelUI* panel = reinterpret_cast<PanelUI*>(entities[index]);
			render2dMgr_->DrawRoundRectWireframe(panel->layout_.center, panel->layout_.size.x, panel->layout_.size.y, panel->layout_.side, panel->layout_.outlineColor, 0.0f);
		}
		break;

		case IEntityUI::Type::BUTTON:
			// Nothing...
			break;
		}
	}
}

void UIManager::PassString(IEntityUI** entities, uint32_t count)
{
	for (uint32_t index = 0; index < count; ++index)
	{
		const IEntityUI::Type& type = entities[index]->GetType();
		switch (type)
		{
		case IEntityUI::Type::TEXT:
		{
			TextUI* text = reinterpret_cast<TextUI*>(entities[index]);
			render2dMgr_->DrawString(text->layout_.font, text->layout_.text, text->textPos_, text->layout_.textColor);
		}
		break;

		case IEntityUI::Type::PANEL:
		{
			PanelUI* panel = reinterpret_cast<PanelUI*>(entities[index]);
			render2dMgr_->DrawString(panel->layout_.font, panel->layout_.text, panel->textPos_, panel->layout_.textColor);
		}
		break;

		case IEntityUI::Type::BUTTON:
		{
			ButtonUI* button = reinterpret_cast<ButtonUI*>(entities[index]);
			render2dMgr_->DrawString(button->layout_.font, button->layout_.text, button->textPos_, button->layout_.textColor);
		}
		break;
		}
	}
}
