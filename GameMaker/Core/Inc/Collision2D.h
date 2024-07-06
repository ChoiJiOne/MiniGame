#pragma once

#include "GameMath.h"


namespace GameMaker
{
/**
 * @brief 2D 오브젝트의 충돌 영역 인터페이스입니다.
 */
class ICollision2D
{
public:
	/**
	 * @brief 2D 충돌 처리 영역의 종류입니다.
	 */
	enum class EType
	{
		NONE          = 0x00,
		POINT         = 0x01,
		LINE          = 0x02,
		CIRCLE        = 0x03,
		RECT          = 0x04, // AABB
		ORIENTED_RECT = 0x05, // OBB
	};


public:
	/**
	 * @brief 2D 오브젝트의 충돌 영역 인터페이스의 디폴트 생성자입니다.
	 */
	ICollision2D() = default;


	/**
	 * @brief 2D 오브젝트의 충돌 영역 인터페이스의 가상 소멸자입니다.
	 */
	virtual ~ICollision2D() {}


	/**
	 * @brief 2D 오브젝트의 충돌 영역 종류를 얻습니다.
	 * 
	 * @return 2D 오브젝트의 충돌 영역 종류를 반환합니다.
	 */
	virtual EType GetType() const = 0;


	/**
	 * @brief 다른 2D 오브젝트와의 충돌 여부를 확인합니다.
	 * 
	 * @param target 충돌 여부를 검사할 타겟입니다.
	 * 
	 * @return 충돌이 검출되면 true, 그렇지 않으면 false를 반환합니다.
	 */
	virtual bool Intersect(const ICollision2D* target) = 0;
};


}