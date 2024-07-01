#pragma once

#include <cstdint>

#include "IResource.h"


namespace GameMaker
{
/**
 * @brief 유니폼 버퍼입니다.
 */
class UniformBuffer
{
public:
	/**
	 * @brief 유니폼 버퍼의 사용 형식입니다.
	 *
	 * @see https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBufferData.xml
	 */
	enum class EUsage
	{
		STREAM  = 0x88E0,
		STATIC  = 0x88E4,
		DYNAMIC = 0x88E8,
	};
};

}