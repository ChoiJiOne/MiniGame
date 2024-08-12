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

	std::string text;
	Vec4f textColor;

	bool bIsRound;
	float side;

	Vec4f outlineColor;
	Vec4f disableColor;
	Vec4f enableColor;
	Vec4f pressColor;
	Vec4f releaseColor;
};



}