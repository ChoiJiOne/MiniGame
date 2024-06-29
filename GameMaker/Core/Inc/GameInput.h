#pragma once

#include <array>
#include <functional>

#include "GameMath.h"


namespace GameMaker
{
/**
 * @brief 입력 상태를 나타냅니다.
 *
 * @note 프레임에 따른 입력 상태입니다.
 * --------------------------------------
 * | 이전 프레임 | 현재 프레임 | 입력 상태 |
 * --------------------------------------
 * |     0      |     0      | NONE     |
 * |     0      |     1      | PRESSED  |
 * |     1      |     0      | RELEASED |
 * |     1      |     1      | HELD     |
 * --------------------------------------
 */
enum class EPressState : int32_t
{
	NONE     = 0x00,
	PRESSED  = 0x01,
	RELEASED = 0x02,
	HELD     = 0x03
};


/**
 * @brief 마우스의 버튼의 종류입니다.
 */
enum class EMouse : int32_t
{
	LEFT   = (1 << ((1) - 1)),
	MIDDLE = (1 << ((2) - 1)),
	RIGHT  = (1 << ((3) - 1)),
};



};