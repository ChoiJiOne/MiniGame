#pragma once

#include "MathModule.h"
#include "Vec.h"


/**
 * @brief 경계 축 정렬 상자(Axis aligned bounding box)입니다.
 */
struct AABB
{
	/**
	 * @brief 경계 축 정렬 상자의 기본 생성자입니다.
	 */
	AABB() = default;


	/**
	 * @brief 경계 축 정렬 상자의 생성자입니다.
	 *
	 * @param cc 경계 축 정렬 상자의 중심 좌표입니다.
	 * @param ee 경계 축 정렬 상자의 X/Y/Z 축 방향으로의 크기입니다.
	 */
	AABB(const Vec3f& cc, const Vec3f& ee)
		: center(cc)
		, extents(ee) {}


	/**
	 * @brief 경계 축 정렬 상자의 복사 생성자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 */
	AABB(AABB&& instance) noexcept
		: center(instance.center)
		, extents(instance.extents) {}


	/**
	 * @brief 경계 축 정렬 상자의 복사 생성자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 */
	AABB(const AABB& instance) noexcept
		: center(instance.center)
		, extents(instance.extents) {}


	/**
	 * @brief 경계 축 정렬 상자의 대입 연산자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	AABB& operator=(AABB&& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;
		extents = instance.extents;

		return *this;
	}


	/**
	 * @brief 경계 축 정렬 상자의 대입 연산자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	AABB& operator=(const AABB& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;
		extents = instance.extents;

		return *this;
	}


	/**
	 * @brief 경계 영역의 최소값을 얻습니다.
	 * 
	 * @param instance 경계 영역의 최소값을 얻을 인스턴스입니다.
	 *
	 * @return 경계 영역의 최소값을 반환합니다.
	 */
	static Vec3f GetMin(const AABB& aabb) { return aabb.center - aabb.extents * 0.5f; }


	/**
	 * @brief 경계 영역의 최댓값을 얻습니다.
	 *
	 * @param instance 경계 영역의 최소값을 얻을 인스턴스입니다.
	 * 
	 * @return 경계 영역의 최댓값을 반환합니다.
	 */
	static Vec3f GetMax(const AABB& aabb) { return aabb.center + aabb.extents * 0.5f; }


	/**
	 * @brief 경계 영역 내의 가장 가까운 점을 찾습니다.
	 * 
	 * @param aabb 가까운 점을 찾을 경계 영역입니다.
	 * @param position 가까운 점입니다.
	 * 
	 * @return 경계 영역 내의 가장 가까운 점을 반환합니다.
	 */
	static Vec3f Closest(const AABB& aabb, const Vec3f& position)
	{
		Vec3f minPos = AABB::GetMin(aabb);
		Vec3f maxPos = AABB::GetMax(aabb);

		Vec3f closest = position;
		closest.x = (closest.x < minPos.x) ? minPos.x : closest.x;
		closest.y = (closest.y < minPos.y) ? minPos.y : closest.y;
		closest.z = (closest.z < minPos.z) ? minPos.z : closest.z;

		closest.x = (closest.x > maxPos.x) ? maxPos.x : closest.x;
		closest.y = (closest.y > maxPos.y) ? maxPos.y : closest.y;
		closest.z = (closest.z > maxPos.z) ? maxPos.z : closest.z;

		return closest;
	}


	/**
	 * @brief 경계 축 정렬 상자의 중심 좌표입니다.
	 */
	Vec3f center;


	/**
	 * @brief 경계 축 정렬 상자의 크기입니다.
	 */
	Vec3f extents;
};


/**
 * @brief 경계 구(Sphere)입니다.
 */
struct Sphere
{
	/**
	 * @brief 경계 구의 기본 생성자입니다.
	 */
	Sphere() = default;


	/**
	 * @brief 경계 구의 생성자입니다.
	 *
	 * @param cc 경계 구의 중심 좌표입니다.
	 * @param rr 경계 구의 반지름 길이입니다.
	 */
	Sphere(const Vec3f & cc, float rr)
		: center(cc)
		, radius(rr) {}


	/**
	 * @brief 경계 구의 복사 생성자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 */
	Sphere(Sphere&& instance) noexcept
		: center(instance.center)
		, radius(instance.radius) {}


	/**
	 * @brief 경계 구의 복사 생성자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 */
	Sphere(const Sphere& instance) noexcept
		: center(instance.center)
		, radius(instance.radius) {}


	/**
	 * @brief 경계 구의 대입 연산자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	Sphere& operator=(Sphere&& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;
		radius = instance.radius;

		return *this;
	}


	/**
	 * @brief 경계 구의 대입 연산자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	Sphere& operator=(const Sphere& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;
		radius = instance.radius;

		return *this;
	}


	/**
	 * @brief 경계 영역 내의 가장 가까운 점을 찾습니다.
	 *
	 * @param sphere 가까운 점을 찾을 경계 영역입니다.
	 * @param position 가까운 점입니다.
	 *
	 * @return 경계 영역 내의 가장 가까운 점을 반환합니다.
	 */
	static Vec3f Closest(const Sphere& sphere, const Vec3f& position)
	{
		Vec3f direction = Vec3f::Normalize(position - sphere.center);
		return sphere.center + direction * sphere.radius;
	}


	/**
	 * @brief 경계 구의 중심 좌표입니다.
	 */
	Vec3f center;


	/**
	 * @brief 경계 구의 반지름 길이입니다.
	 */
	float radius;
};


/**
 * @brief 광선(Ray)입니다.
 */
struct Ray
{
	/**
	 * @brief 광선의 기본 생성자입니다.
	 */
	Ray() = default;


	/**
	 * @brief 광선의 생성자입니다.
	 *
	 * @param o 광선의 위치입니다.
	 * @param d 광선의 방향입니다.
	 */
	Ray(const Vec3f& o, const Vec3f& d)
		: origin(o)
		, direction(Vec3f::Normalize(d)) {}


	/**
	 * @brief 광선의 복사 생성자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 */
	Ray(Ray&& instance) noexcept
		: origin(instance.origin)
		, direction(instance.direction) {}


	/**
	 * @brief 광선의 복사 생성자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 */
	Ray(const Ray& instance) noexcept
		: origin(instance.origin)
		, direction(instance.direction) {}



	/**
	 * @brief 광선의 대입 연산자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	Ray& operator=(Ray&& instance) noexcept
	{
		if (this == &instance) return *this;

		origin = instance.origin;
		direction = instance.direction;

		return *this;
	}


	/**
	 * @brief 광선의 대입 연산자입니다.
	 *
	 * @param instance 내부 프로퍼티를 복사할 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	Ray& operator=(const Ray& instance) noexcept
	{
		if (this == &instance) return *this;

		origin = instance.origin;
		direction = instance.direction;

		return *this;
	}


	/**
	 * @brief 광선의 위치입니다.
	 */
	Vec3f origin;


	/**
	 * @brief 광선의 방향입니다.
	 */
	Vec3f direction;
};


/**
 * @brief 충돌 처리를 수행합니다.
 * 
 * @note 이 클래스의 모든 멤버 변수와 메서드는 정적(static) 타입입니다.
 */
class Collision
{
public:
	/**
	 * @brief 경계 구와 경계 구 사이의 충돌 여부를 검사합니다.
	 * 
	 * @param sphere0 충돌 여부를 검사할 경계 구입니다.
	 * @param sphere1 충돌 처리를 검사할 또 다른 구입니다.
	 * 
	 * @return 경계 구와 경계 구 사이의 충돌이 감지되면 true, 그렇지 않으면 false를 반환합니다.
	 */
	static bool SphereToSphere(const Sphere& sphere0, const Sphere& sphere1)
	{
		float lengthSq = Vec3f::LengthSq(sphere0.center - sphere1.center);
		float sumRadius = (sphere0.radius + sphere1.radius);

		return lengthSq <= (sumRadius * sumRadius);
	}


	/**
	 * @brief 경계 구와 AABB 사이의 충돌 여부를 검사합니다.
	 * 
	 * @param sphere 충돌 여부를 검사할 경계 구입니다.
	 * @param aabb 충돌 여부를 검사할 AABB입니다.
	 * 
	 * @return 경계 구와 AABB 사이의 충돌이 감지되면 true, 그렇지 않으면 false를 반환합니다.
	 */
	static bool SphereToAABB(const Sphere& sphere, const AABB& aabb)
	{
		Vec3f closest = AABB::Closest(aabb, sphere.center);
		float distSq = Vec3f::LengthSq(sphere.center - closest);
		float radiusSq = sphere.radius * sphere.radius;

		return distSq <= radiusSq;
	}


	/**
	 * @brief AABB와 AABB 사이의 충돌 여부를 검사합니다.
	 * 
	 * @param aabb0 충돌 여부를 검사할 AABB입니다.
	 * @param aabb1 충돌 여부를 검사할 또 다른 AABB입니다.
	 */
	static bool AABBToAABB(const AABB& aabb0, const AABB& aabb1)
	{
		Vec3f minPos0 = AABB::GetMin(aabb0);
		Vec3f maxPos0 = AABB::GetMax(aabb0);

		Vec3f minPos1 = AABB::GetMin(aabb1);
		Vec3f maxPos1 = AABB::GetMax(aabb1);

		if (minPos0.x > maxPos1.x || maxPos0.x < minPos1.x)
		{
			return false;
		}

		if (minPos0.y > maxPos1.y || maxPos0.y < minPos1.y)
		{
			return false;
		}

		if (minPos0.z > maxPos1.z || maxPos0.z < minPos1.z)
		{
			return false;
		}

		return true;
	}


	/**
	 * @brief 경계 구와 광선이 충돌하는지 확인합니다.
	 * 
	 * @param ray 경계 구와 충돌하는지 확인할 광선입니다.
	 * @param sphere 광선과 충돌하는지 확인할 경계 구입니다.
	 * 
	 * @return 경계 구와 광선이 충돌한다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	static bool Raycast(const Ray& ray, const Sphere& sphere)
	{
		Vec3f e = sphere.center - ray.origin;
		
		float r2 = sphere.radius * sphere.radius;
		float e2 = Vec3f::Dot(e, e);
		float a = Vec3f::Dot(e, ray.direction);
		float a2 = a * a;
		float b2 = e2 - a2;
		float f = MathModule::Sqrt(MathModule::Abs(r2) - b2);
		float t = a - f;

		return r2 - (e2 - a2) >= 0.0f;
	}


	/**
	 * @brief AABB와 광선이 충돌하는지 확인합니다.
	 * 
	 * @param ray AABB와 충돌하는지 확인할 광선입니다.
	 * @param aabb 광선과 충돌하는지 확인할 AABB입니다.
	 *
	 * @return AABB와 광선이 충돌한다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	static bool Raycast(const Ray& ray, const AABB& aabb)
	{
		Vec3f minPos = AABB::GetMin(aabb);
		Vec3f maxPos = AABB::GetMax(aabb);

		Vec3f direction = ray.direction;
		direction.x = MathModule::NearZero(direction.x) ? 0.00001f : direction.x;
		direction.y = MathModule::NearZero(direction.y) ? 0.00001f : direction.y;
		direction.z = MathModule::NearZero(direction.z) ? 0.00001f : direction.z;

		float t1 = (minPos.x - ray.origin.x) / direction.x;
		float t2 = (maxPos.x - ray.origin.x) / direction.x;
		float t3 = (minPos.y - ray.origin.y) / direction.y;
		float t4 = (maxPos.y - ray.origin.y) / direction.y;
		float t5 = (minPos.z - ray.origin.z) / direction.z;
		float t6 = (maxPos.z - ray.origin.z) / direction.z;

		float tmin = MathModule::Max(MathModule::Max(MathModule::Min(t1, t2), MathModule::Min(t3, t4)), MathModule::Min(t5, t6));
		float tmax = MathModule::Min(MathModule::Min(MathModule::Max(t1, t2), MathModule::Max(t3, t4)), MathModule::Max(t5, t6));

		return !((tmax < 0.0f || tmin > tmax));
	}
};