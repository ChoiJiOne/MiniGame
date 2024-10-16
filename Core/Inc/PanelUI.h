#pragma once

#include "Collision2D.h"
#include "IEntityUI.h"

class TTFont;

class PanelUI : public IEntityUI
{
public:
	struct Layout
	{
		Vec4f backgroundColor;
		Vec4f outlineColor;
		Vec4f textColor;
		Vec2f center;
		Vec2f size;
		float side = 0.0f;

		TTFont* font = nullptr;
		std::wstring text;
	};

public:
	PanelUI(const Layout& layout);
	virtual ~PanelUI();

	DISALLOW_COPY_AND_ASSIGN(PanelUI);

	virtual void Tick(float deltaSeconds) override;
	virtual void Release() override;
	virtual Type GetType() const override { return Type::PANEL; }

	void SetText(const std::wstring& text);

private:
	friend class UIManager;

private:
	Layout layout_;
	Vec2f textSize_;
	Vec2f textPos_; /** 렌더링 시 참조할 텍스트의 위치. */
};