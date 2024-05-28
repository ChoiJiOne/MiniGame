#pragma once

#include <cstdint>


/**
 * @brief 애니메이션 트랙의 한 요소인 키 프레임입니다.
 */
template <uint32_t N>
struct Keyframe
{
	float value[N];
	float in[N];
	float out[N];
	float time;
};

using ScalarFrame = Keyframe<1>;
using VectorFrame = Keyframe<3>;
using QuaternionFrame = Keyframe<4>;