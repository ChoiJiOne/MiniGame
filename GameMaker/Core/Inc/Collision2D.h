#pragma once

#include "GameMath.h"


namespace GameMaker
{
/**
 * @brief 2D 오브젝트의 충돌 영역 인터페이스입니다.
 */
struct ICollision2D
{
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
	virtual bool Intersect(const ICollision2D* target) const = 0;
};


/**
 * @brief 2D 선 입니다.
 */
struct Line2D : public ICollision2D
{
	/**
	 * @brief 2D 선의 디폴트 생성자입니다.
	 */
	Line2D() = default;


	/**
	 * @brief 2D 선의 생성자입니다.
	 * 
	 * @param s 2D 선의 시작점입니다.
	 * @param e 2D 선의 끝점입니다.
	 */
	Line2D(const Vec2f& s, const Vec2f& e) : start(s), end(e) {}


	/**
	 * @brief 2D 선의 복사 생성자입니다.
	 * 
	 * @param instance 원소를 복사할 2D 선 인스턴스입니다.
	 */
	Line2D(Line2D&& instance) noexcept : start(instance.start), end(instance.end) {}


	/**
	 * @brief 2D 선의 복사 생성자입니다.
	 *
	 * @param instance 원소를 복사할 2D 선 인스턴스입니다.
	 */
	Line2D(const Line2D& instance) noexcept : start(instance.start), end(instance.end) {}


	/**
	 * @brief 2D 선의 빈 가상 소멸자입니다.
	 */
	virtual ~Line2D() {}


	/**
	 * @brief 2D 선의 대입 연산자입니다.
	 * 
	 * @param instance 대입 연산을 수행할 2D 선 인스턴스입니다.
	 * 
	 * @return 대입 연산을 수행한 객체의 참조자를 반환합니다.
	 */
	Line2D& operator=(Line2D&& instance) noexcept
	{
		if (this == &instance) return *this;

		start = instance.start;
		end = instance.end;

		return *this;
	}


	/**
	 * @brief 2D 선의 대입 연산자입니다.
	 *
	 * @param instance 대입 연산을 수행할 2D 선 인스턴스입니다.
	 *
	 * @return 대입 연산을 수행한 객체의 참조자를 반환합니다.
	 */
	Line2D& operator=(const Line2D& instance) noexcept
	{
		if (this == &instance) return *this;

		start = instance.start;
		end = instance.end;

		return *this;
	}

	virtual EType GetType() const override { return EType::LINE; }


	/**
	 * @brief 다른 2D 오브젝트와의 충돌 여부를 확인합니다.
	 *
	 * @param target 충돌 여부를 검사할 타겟입니다.
	 *
	 * @return 충돌이 검출되면 true, 그렇지 않으면 false를 반환합니다.
	 */
	virtual bool Intersect(const ICollision2D* target) const override;


	/**
	 * @brief 선의 시작점입니다.
	 */
	Vec2f start;


	/**
	 * @brief 선의 끝점입니다.
	 */
	Vec2f end;
};

}