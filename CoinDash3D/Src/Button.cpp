#include <json/json.h>

#include "Assertion.h"
#include "FileModule.h"
#include "GeometryRenderer2D.h"
#include "RenderModule.h"
#include "TextRenderer.h"

#include "Button.h"
#include "StringUtils.h"

Button::Button(const std::string& path, TTFont* font, const EMouseButton& mouseButton, const std::function<void()>& clickEvent, GeometryRenderer2D* geometryRenderer, TextRenderer* textRenderer)
	: font_(font)
	, mouseButton_(mouseButton)
	, clickEvent_(clickEvent)
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
	CHECK(root["textColor"].isArray() && !root["textColor"].isNull());
	CHECK(root["disableColor"].isArray() && !root["disableColor"].isNull());
	CHECK(root["enableColor"].isArray() && !root["enableColor"].isNull());
	CHECK(root["pressColor"].isArray() && !root["pressColor"].isNull());
	CHECK(root["releaseColor"].isArray() && !root["releaseColor"].isNull());

	layout_.width = root["width"].asFloat();
	layout_.height = root["height"].asFloat();
	layout_.side = root["side"].asFloat();
	layout_.center.x = root["x"].asFloat();
	layout_.center.y = root["y"].asFloat();
	layout_.text = StringUtils::Convert(std::string(root["text"].asCString()));
	layout_.outlineColor = Vec4f(root["outlineColor"][0].asFloat(), root["outlineColor"][1].asFloat(), root["outlineColor"][2].asFloat(), root["outlineColor"][3].asFloat());
	layout_.textColor = Vec4f(root["textColor"][0].asFloat(), root["textColor"][1].asFloat(), root["textColor"][2].asFloat(), root["textColor"][3].asFloat());
	layout_.disableColor = Vec4f(root["disableColor"][0].asFloat(), root["disableColor"][1].asFloat(), root["disableColor"][2].asFloat(), root["disableColor"][3].asFloat());
	layout_.enableColor = Vec4f(root["enableColor"][0].asFloat(), root["enableColor"][1].asFloat(), root["enableColor"][2].asFloat(), root["enableColor"][3].asFloat());
	layout_.pressColor = Vec4f(root["pressColor"][0].asFloat(), root["pressColor"][1].asFloat(), root["pressColor"][2].asFloat(), root["pressColor"][3].asFloat());
	layout_.releaseColor = Vec4f(root["releaseColor"][0].asFloat(), root["releaseColor"][1].asFloat(), root["releaseColor"][2].asFloat(), root["releaseColor"][3].asFloat());

	bIsInitialized_ = true;
}

Button::Button(const Layout& layout, TTFont* font, const EMouseButton& mouseButton, const std::function<void()>& clickEvent)
	: layout_(layout)
	, font_(font)
	, mouseButton_(mouseButton)
	, clickEvent_(clickEvent)
{
	bIsInitialized_ = true;
}

Button::~Button()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Button::Tick(float deltaSeconds)
{
	EPressState pressState = InputController::GetKeyPressState(static_cast<EKey>(mouseButton_));
	state_ = GetMouseButtonState(pressState, state_);

	if (state_ == EState::RELEASED)
	{
		clickEvent_();
	}
}

void Button::Render()
{
	Vec4f backgroundColor;
	switch (state_)
	{
	case EState::DISABLED:
		backgroundColor = layout_.disableColor;
		break;

	case EState::ENABLED:
		backgroundColor = layout_.enableColor;
		break;

	case EState::PRESSED:
		backgroundColor = layout_.pressColor;
		break;

	case EState::RELEASED:
		backgroundColor = layout_.releaseColor;
		break;
	}

	geometryRenderer2D_->DrawRoundRectangle2D(layout_.center, layout_.width, layout_.height, layout_.side, 0.0f, backgroundColor);
	geometryRenderer2D_->DrawWireframeRoundRectangle2D(layout_.center, layout_.width, layout_.height, layout_.side, 0.0f, layout_.outlineColor);
	if (!layout_.text.empty())
	{
		textRenderer_->DrawText2D(font_, layout_.text, layout_.center, EStartPivot::CENTER, layout_.textColor);
	}
}

void Button::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}

Button::EState Button::GetMouseButtonState(const EPressState& pressState, const EState& state)
{
	if (DetectMouseCursorInButton())
	{
		if (state == EState::DISABLED)
		{
			if (pressState == EPressState::HELD || pressState == EPressState::PRESSED)
			{
				return EState::DISABLED;
			}
			else // pressState == EPressState::NONE || pressState == EPressState::RELEASED
			{
				return EState::ENABLED;
			}
		}
		else if (state == EState::ENABLED)
		{
			if (pressState == EPressState::HELD || pressState == EPressState::PRESSED)
			{
				return EState::PRESSED;
			}
			else
			{
				return EState::ENABLED;
			}
		}
		else if (state == EState::PRESSED)
		{
			if (pressState == EPressState::RELEASED)
			{
				return EState::RELEASED;
			}
			else if (pressState == EPressState::HELD || pressState == EPressState::PRESSED)
			{
				return EState::PRESSED;
			}
			else // pressState == EPressState::NONE
			{
				return EState::ENABLED;
			}
		}
		else // state == EState::RELEASED
		{
			if (pressState == EPressState::NONE || pressState == EPressState::RELEASED)
			{
				return EState::ENABLED;
			}
			else // pressState == EPressState::HELD || pressState == EPressState::PRESSED
			{
				return EState::PRESSED;
			}
		}
	}
	else
	{
		if (state == EState::PRESSED && pressState == EPressState::HELD)
		{
			return EState::PRESSED;
		}
		else
		{
			return EState::DISABLED;
		}
	}

	return EState::DISABLED;
}

bool Button::DetectMouseCursorInButton()
{
	CursorPos cursorPosition = InputController::GetCurrCursorPos();
	Vec2i cursorPos(cursorPosition.x, cursorPosition.y);

	Vec2f minPosition = layout_.center - Vec2f(layout_.width, layout_.height) * 0.5f;
	Vec2f maxPosition = layout_.center + Vec2f(layout_.width, layout_.height) * 0.5f;
	Vec2f currentCursorPosition = Vec2f(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));

	return (minPosition.x <= currentCursorPosition.x && currentCursorPosition.x <= maxPosition.x)
		&& (minPosition.y <= currentCursorPosition.y && currentCursorPosition.y <= maxPosition.y);
}