#pragma once

#include "Collision2D.h"
#include "IEntityUI.h"

class TTFont;

class TextUI : public IEntityUI
{
public:
	struct Layout
	{
		Vec4f textColor;
		Vec2f textCenterPos; /** 텍스트의 중심 좌표 */
		TTFont* font = nullptr;
		std::wstring text;
	};

public:
	TextUI(const Layout& layout);
	virtual ~TextUI();

	DISALLOW_COPY_AND_ASSIGN(TextUI);

	virtual void Tick(float deltaSeconds) override;
	virtual void Release() override;
	virtual Type GetType() const override { return Type::TEXT; }

	void SetText(const std::wstring& text);

private:
	friend class UIManager;

private:
	Layout layout_;
	Vec2f textSize_;
	Vec2f textPos_; /** 렌더링 시 참조할 텍스트의 위치. */
};