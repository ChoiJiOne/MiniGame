#pragma once

#include <cstdint>

namespace GameInput
{
/**
 * --------------------------------------
 * | 이전 프레임 | 현재 프레임 | 입력 상태 |
 * --------------------------------------
 * |     0      |     0      | NONE     |
 * |     0      |     1      | PRESSED  |
 * |     1      |     0      | RELEASED |
 * |     1      |     1      | HELD     |
 * --------------------------------------
 */
enum class Press : int32_t
{
	NONE     = 0x00,
	PRESSED  = 0x01,
	RELEASED = 0x02,
	HELD     = 0x03
};

enum class Mouse : int32_t
{
	LEFT   = (1 << ((1) - 1)),
	MIDDLE = (1 << ((2) - 1)),
	RIGHT  = (1 << ((3) - 1)),
};



}