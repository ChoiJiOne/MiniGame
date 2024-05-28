#pragma once

/**
 * @brief 곡선의 보간 유형입니다.
 * 
 * @see https://gabormakesgames.com/blog_animation_pose.html
 */
enum class EInterpolation
{
	CONSTANT = 0x00,
	LINEAR   = 0x01,
	CUBIC    = 0x02,
};