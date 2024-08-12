#pragma once

#include <functional>
#include <string>

#include "GameMath.h"
#include "IEntity.h"
#include "InputManager.h"

namespace GameMaker
{
class TTFont;

struct UILayout
{
	Vec2f size;
	Vec2f center;

	bool bIsRound;
	float side;

	Vec4f outlineColor;
	bool bEnableOutline;

	Vec4f disableColor;
	Vec4f enableColor;
	Vec4f pressColor;
	Vec4f releaseColor;

	std::wstring text;
	TTFont* font_ = nullptr;
	Vec4f textDisableColor;
	Vec4f textEnableColor;
	Vec4f textPressColor;
	Vec4f textReleaseColor;
};

class EntityUI : public IEntity
{
public:
	EntityUI(const UILayout& layout, bool bIsActive = true)
		: layout_(layout), bIsActive_(bIsActive) {}
	virtual ~EntityUI() {}

	DISALLOW_COPY_AND_ASSIGN(EntityUI);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Release() = 0;

	const UILayout& GetLayout() const { return layout_; }
	
	bool IsActive() const { return bIsActive_; }
	void SetActive(bool bIsActive) { bIsActive_ = bIsActive; }

protected:
	UILayout layout_;
	bool bIsActive_ = true;
};

class ButtonUI : public EntityUI 
{
public:
	enum class EState : int32_t
	{
		DISABLED = 0x00,
		ENABLED  = 0x01,
		PRESSED  = 0x02,
		RELEASED = 0x03,
	};

public:
	ButtonUI(const UILayout& layout, const EMouse& mouseButton, const std::function<void()>& clickEvent, bool bIsActive = true);
	virtual ~ButtonUI();

	DISALLOW_COPY_AND_ASSIGN(ButtonUI);

	virtual void Tick(float deltaSeconds) override;
	virtual void Release() override;

private:
	EMouse mouseButton_ = EMouse::LEFT;
	std::function<void()> clickEvent_;
	EState state_ = EState::DISABLED;
};

}