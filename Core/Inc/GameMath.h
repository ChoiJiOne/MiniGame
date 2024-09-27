#pragma once

#include <chrono>
#include <cstdint>
#include <limits>
#include <random>

const float PI          = 3.141592654f;
const float TWO_PI      = 6.283185307f;
const float ONE_DIV_PI  = 0.318309886f;
const float ONE_DIV_2PI = 0.159154943f;
const float PI_DIV_2    = 1.570796327f;
const float PI_DIV_4    = 0.785398163f;
const float EPSILON     = 1.192092896e-07F;

namespace GameMath
{
	__forceinline float ToDegree(float radian)
	{
		return (radian * 180.0f) / PI;
	}

	__forceinline float ToRadian(float degree)
	{
		return (degree * PI) / 180.0f;
	}

	__forceinline bool NearZero(float value, float epsilon = EPSILON)
	{
		return (std::fabsf(value) <= epsilon);
	}

	template <typename T>
	__forceinline T Max(const T& lhs, const T& rhs)
	{
		return (lhs < rhs ? rhs : lhs);
	}

	template <typename T>
	__forceinline T Min(const T& lhs, const T& rhs)
	{
		return (lhs < rhs ? lhs : rhs);
	}

	__forceinline float Sqrt(float x)
	{
		return std::sqrtf(x);
	}

	__forceinline float Abs(float x)
	{
		return std::fabsf(x);
	}

	__forceinline float Fmod(float x, float y)
	{
		return std::fmodf(x, y);
	}

	template <typename T>
	__forceinline T Clamp(const T& value, const T& lower, const T& upper)
	{
		return Min<T>(upper, Max<T>(lower, value));
	}

	/** https://gist.github.com/publik-void/067f7f2fef32dbe5c27d6e215f824c91 */
	__forceinline float Sin(float radian)
	{
		float quotient = ONE_DIV_2PI * radian;
		if (radian >= 0.0f)
		{
			quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
		}

		float y = radian - TWO_PI * quotient;

		if (y > PI_DIV_2)
		{
			y = PI - y;
		}
		else if (y < -PI_DIV_2)
		{
			y = -PI - y;
		}

		float y2 = y * y;
		return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
	}

	/** https://gist.github.com/publik-void/067f7f2fef32dbe5c27d6e215f824c91 */
	__forceinline float Cos(float radian)
	{
		float quotient = ONE_DIV_2PI * radian;
		if (radian >= 0.0f)
		{
			quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
		}

		float y = radian - TWO_PI * quotient;
		float sign = 1.0f;

		if (y > PI_DIV_2)
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -PI_DIV_2)
		{
			y = -PI - y;
			sign = -1.0f;
		}

		float y2 = y * y;
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;

		return sign * p;
	}

	/** https://gist.github.com/publik-void/067f7f2fef32dbe5c27d6e215f824c91 */
	__forceinline float ASin(float value)
	{
		float x = std::fabsf(value);
		float omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}

		float root = sqrtf(omx);
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
		result *= root;

		if (value >= 0.0f)
		{
			return (PI_DIV_2 - result);
		}
		else
		{
			return (result - PI_DIV_2);
		}
	}

	/** https://gist.github.com/publik-void/067f7f2fef32dbe5c27d6e215f824c91 */
	__forceinline float ACos(float value)
	{
		float x = std::fabsf(value);
		float omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}

		float root = sqrtf(omx);
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
		result *= root;

		if (value >= 0.0f)
		{
			return result;
		}
		else
		{
			return PI - result;
		}
	}

	/** 값의 범위는 [minValue, maxValue] 입니다. */
	__forceinline int32_t GenerateRandomInt(int32_t minValue, int32_t maxValue)
	{
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_int_distribution<int32_t> distribution(Min<int32_t>(minValue, maxValue), Max<int32_t>(minValue, maxValue));

		return distribution(generator);
	}

	/** 값의 범위는 [minValue, maxValue] 입니다. */
	__forceinline float GenerateRandomFloat(float minValue, float maxValue)
	{
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_real_distribution<float> distribution(Min<float>(minValue, maxValue), Max<float>(minValue, maxValue));

		return distribution(generator);
	}
}

struct Vec2i
{
	Vec2i() noexcept : x(0), y(0) {}
	Vec2i(int32_t&& xx, int32_t&& yy) noexcept : x(xx), y(yy) {}
	Vec2i(const int32_t& xx, const int32_t& yy) noexcept : x(xx), y(yy) {}
	Vec2i(int32_t&& e) noexcept : x(e), y(e) {}
	Vec2i(const int32_t& e) noexcept : x(e), y(e) {}
	Vec2i(Vec2i&& v) noexcept : x(v.x), y(v.y) {}
	Vec2i(const Vec2i& v) noexcept : x(v.x), y(v.y) {}

	Vec2i& operator=(Vec2i&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;

		return *this;
	}

	Vec2i& operator=(const Vec2i& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;

		return *this;
	}

	Vec2i operator-() const
	{
		return Vec2i(-x, -y);
	}

	Vec2i operator+(Vec2i&& v) const
	{
		return Vec2i(x + v.x, y + v.y);
	}

	Vec2i operator+(const Vec2i& v) const
	{
		return Vec2i(x + v.x, y + v.y);
	}

	Vec2i operator-(Vec2i&& v) const
	{
		return Vec2i(x - v.x, y - v.y);
	}

	Vec2i operator-(const Vec2i& v) const
	{
		return Vec2i(x - v.x, y - v.y);
	}

	Vec2i operator*(Vec2i&& v) const
	{
		return Vec2i(x * v.x, y * v.y);
	}

	Vec2i operator*(const Vec2i& v) const
	{
		return Vec2i(x * v.x, y * v.y);
	}

	Vec2i& operator+=(Vec2i&& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	Vec2i& operator+=(const Vec2i& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	Vec2i& operator-=(Vec2i&& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	Vec2i& operator-=(const Vec2i& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	bool operator==(Vec2i&& v) const noexcept
	{
		return (x == v.x) && (y == v.y);
	}

	bool operator==(const Vec2i& v) const noexcept
	{
		return (x == v.x) && (y == v.y);
	}

	bool operator!=(Vec2i&& v) const noexcept
	{
		return (x != v.x) || (y != v.y);
	}

	bool operator!=(const Vec2i& v) const noexcept
	{
		return (x != v.x) || (y != v.y);
	}

	const int32_t* GetPtr() const { return &data[0]; }
	int32_t* GetPtr() { return &data[0]; }

	static inline int32_t Dot(const Vec2i& lhs, const Vec2i& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	static inline int32_t Cross(const Vec2i& lhs, const Vec2i& rhs)
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}

	static inline int32_t LengthSq(const Vec2i& v)
	{
		return v.x * v.x + v.y * v.y;
	}

	static inline float Length(const Vec2i& v)
	{
		float lengthSq = static_cast<float>(LengthSq(v));
		return GameMath::Sqrt(lengthSq);
	}

	static inline float Radian(const Vec2i& lhs, const Vec2i& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return GameMath::ACos(dot / (lengthL * lengthR));
	}

	static inline float Degree(const Vec2i& lhs, const Vec2i& rhs)
	{
		float radian = Radian(lhs, rhs);
		return GameMath::ToDegree(radian);
	}

	union
	{
		struct
		{
			int32_t x;
			int32_t y;
		};
		int32_t data[2];
	};
};

struct Vec2f
{
	Vec2f() noexcept : x(0.0f), y(0.0f) {}
	Vec2f(float&& xx, float&& yy) noexcept : x(xx), y(yy) {}
	Vec2f(const float& xx, const float& yy) noexcept : x(xx), y(yy) {}
	Vec2f(float&& e) noexcept : x(e), y(e) {}
	Vec2f(const float& e) noexcept : x(e), y(e) {}
	Vec2f(Vec2f&& v) noexcept : x(v.x), y(v.y) {}
	Vec2f(const Vec2f& v) noexcept : x(v.x), y(v.y) {}

	Vec2f& operator=(Vec2f&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;

		return *this;
	}

	Vec2f& operator=(const Vec2f& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;

		return *this;
	}

	Vec2f operator-() const
	{
		return Vec2f(-x, -y);
	}

	Vec2f operator+(Vec2f&& v) const
	{
		return Vec2f(x + v.x, y + v.y);
	}

	Vec2f operator+(const Vec2f& v) const
	{
		return Vec2f(x + v.x, y + v.y);
	}

	Vec2f operator-(Vec2f&& v) const
	{
		return Vec2f(x - v.x, y - v.y);
	}

	Vec2f operator-(const Vec2f& v) const
	{
		return Vec2f(x - v.x, y - v.y);
	}

	Vec2f operator*(Vec2f&& v) const
	{
		return Vec2f(x * v.x, y * v.y);
	}

	Vec2f operator*(const Vec2f& v) const
	{
		return Vec2f(x * v.x, y * v.y);
	}

	Vec2f& operator+=(Vec2f&& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	Vec2f& operator+=(const Vec2f& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	Vec2f& operator-=(Vec2f&& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	Vec2f& operator-=(const Vec2f& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	bool operator==(Vec2f&& v) const noexcept
	{
		return GameMath::Abs(x - v.x) <= EPSILON && GameMath::Abs(y - v.y) <= EPSILON;
	}

	bool operator==(const Vec2f& v) const noexcept
	{
		return GameMath::Abs(x - v.x) <= EPSILON && GameMath::Abs(y - v.y) <= EPSILON;
	}

	bool operator!=(Vec2f&& v) const noexcept
	{
		return GameMath::Abs(x - v.x) > EPSILON || GameMath::Abs(y - v.y) > EPSILON;
	}

	bool operator!=(const Vec2f& v) const noexcept
	{
		return GameMath::Abs(x - v.x) > EPSILON || GameMath::Abs(y - v.y) > EPSILON;
	}

	const float* GetPtr() const { return &data[0]; }
	float* GetPtr() { return &data[0]; }

	static inline float Dot(const Vec2f& lhs, const Vec2f& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	static inline float Cross(const Vec2f& lhs, const Vec2f& rhs)
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}

	static inline float LengthSq(const Vec2f& v)
	{
		return v.x * v.x + v.y * v.y;
	}

	static inline float Length(const Vec2f& v)
	{
		float lengthSq = LengthSq(v);
		return GameMath::Sqrt(lengthSq);
	}

	static inline Vec2f Normalize(const Vec2f& v)
	{
		float length = Length(v);

		if (GameMath::NearZero(length))
		{
			return v;
		}

		float invLength = 1.0f / length;
		return Vec2f(v.x * invLength, v.y * invLength);
	}

	static inline float Radian(const Vec2f& lhs, const Vec2f& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return GameMath::ACos(dot / (lengthL * lengthR));
	}

	static inline float Degree(const Vec2f& lhs, const Vec2f& rhs)
	{
		float radian = Radian(lhs, rhs);
		return GameMath::ToDegree(radian);
	}

	static inline Vec2f Project(const Vec2f& target, const Vec2f& base)
	{
		float dot = Dot(target, base);
		float lengthSq = LengthSq(base);
		float scale = dot / lengthSq;

		return Vec2f(base.x * scale, base.y * scale);
	}

	static inline Vec2f Reject(const Vec2f& target, const Vec2f& base)
	{
		Vec2f project = Project(target, base);
		return target - project;
	}

	static inline Vec2f Lerp(const Vec2f& s, const Vec2f& e, const float& t)
	{
		return s * (1.0f - t) + e * t;
	}

	static inline Vec2f Slerp(const Vec2f& s, const Vec2f& e, const float& t)
	{
		Vec2f start = Normalize(s);
		Vec2f end = Normalize(e);

		float theta = Radian(start, end);
		float sinTheta = GameMath::Sin(theta);

		float a = GameMath::Sin((1.0f - t) * theta) / sinTheta;
		float b = GameMath::Sin(t * theta) / sinTheta;

		return s * a + e * b;
	}

	static inline Vec2f Bezier(const Vec2f& s, const Vec2f& e, const Vec2f& c, const float& t)
	{
		Vec2f p = s * (1.0f - t) + c * t;
		Vec2f q = c * (1.0f - t) + e * t;

		return p * (1.0f - t) + q * t;
	}

	union
	{
		struct
		{
			float x;
			float y;
		};
		float data[2];
	};
};

struct Vec3i
{
	Vec3i() noexcept : x(0), y(0), z(0) {}
	Vec3i(int32_t&& xx, int32_t&& yy, int32_t&& zz) noexcept : x(xx), y(yy), z(zz) {}
	Vec3i(const int32_t& xx, const int32_t& yy, const int32_t& zz) noexcept : x(xx), y(yy), z(zz) {}
	Vec3i(int32_t&& e) noexcept : x(e), y(e), z(e) {}
	Vec3i(const int32_t& e) noexcept : x(e), y(e), z(e) {}
	Vec3i(Vec3i&& v) noexcept : x(v.x), y(v.y), z(v.z) {}
	Vec3i(const Vec3i& v) noexcept : x(v.x), y(v.y), z(v.z) {}

	Vec3i& operator=(Vec3i&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	Vec3i& operator=(const Vec3i& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	Vec3i operator-() const
	{
		return Vec3i(-x, -y, -z);
	}

	Vec3i operator+(Vec3i&& v) const
	{
		return Vec3i(x + v.x, y + v.y, z + v.z);
	}

	Vec3i operator+(const Vec3i& v) const
	{
		return Vec3i(x + v.x, y + v.y, z + v.z);
	}

	Vec3i operator-(Vec3i&& v) const
	{
		return Vec3i(x - v.x, y - v.y, z - v.z);
	}

	Vec3i operator-(const Vec3i& v) const
	{
		return Vec3i(x - v.x, y - v.y, z - v.z);
	}

	Vec3i operator*(Vec3i&& v) const
	{
		return Vec3i(x * v.x, y * v.y, z * v.z);
	}

	Vec3i operator*(const Vec3i& v) const
	{
		return Vec3i(x * v.x, y * v.y, z * v.z);
	}

	Vec3i& operator+=(Vec3i&& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vec3i& operator+=(const Vec3i& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vec3i& operator-=(Vec3i&& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	Vec3i& operator-=(const Vec3i& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	bool operator==(Vec3i&& v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z);
	}

	bool operator==(const Vec3i& v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z);
	}

	bool operator!=(Vec3i&& v) const
	{
		return (x != v.x) || (y != v.y) || (z != v.z);
	}

	bool operator!=(const Vec3i& v) const
	{
		return (x != v.x) || (y != v.y) || (z != v.z);
	}

	const int32_t* GetPtr() const { return &data[0]; }
	int32_t* GetPtr() { return &data[0]; }

	static inline int32_t Dot(const Vec3i& lhs, const Vec3i& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	static inline Vec3i Cross(const Vec3i& lhs, const Vec3i& rhs)
	{
		return Vec3i(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}

	static inline int32_t LengthSq(const Vec3i& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	static inline float Length(const Vec3i& v)
	{
		float lengthSq = static_cast<float>(LengthSq(v));
		return GameMath::Sqrt(lengthSq);
	}

	static inline float Radian(const Vec3i& lhs, const Vec3i& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return GameMath::ACos(dot / (lengthL * lengthR));
	}

	static inline float Degree(const Vec3i& lhs, const Vec3i& rhs)
	{
		float radian = Radian(lhs, rhs);
		return GameMath::ToDegree(radian);
	}

	union
	{
		struct
		{
			int32_t x;
			int32_t y;
			int32_t z;
		};
		int32_t data[3];
	};
};

struct Vec3f
{
	Vec3f() noexcept : x(0.0f), y(0.0f), z(0.0f) {}
	Vec3f(float&& xx, float&& yy, float&& zz) noexcept : x(xx), y(yy), z(zz) {}
	Vec3f(const float& xx, const float& yy, const float& zz) noexcept : x(xx), y(yy), z(zz) {}
	Vec3f(float&& e) noexcept : x(e), y(e), z(e) {}
	Vec3f(const float& e) noexcept : x(e), y(e), z(e) {}
	Vec3f(Vec3f&& v) noexcept : x(v.x), y(v.y), z(v.z) {}
	Vec3f(const Vec3f& v) noexcept : x(v.x), y(v.y), z(v.z) {}

	Vec3f& operator=(Vec3f&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	Vec3f& operator=(const Vec3f& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	Vec3f operator-() const
	{
		return Vec3f(-x, -y, -z);
	}

	Vec3f operator+(Vec3f&& v) const
	{
		return Vec3f(x + v.x, y + v.y, z + v.z);
	}

	Vec3f operator+(const Vec3f& v) const
	{
		return Vec3f(x + v.x, y + v.y, z + v.z);
	}

	Vec3f operator-(Vec3f&& v) const
	{
		return Vec3f(x - v.x, y - v.y, z - v.z);
	}

	Vec3f operator-(const Vec3f& v) const
	{
		return Vec3f(x - v.x, y - v.y, z - v.z);
	}

	Vec3f operator*(Vec3f&& v) const
	{
		return Vec3f(x * v.x, y * v.y, z * v.z);
	}

	Vec3f operator*(const Vec3f& v) const
	{
		return Vec3f(x * v.x, y * v.y, z * v.z);
	}

	Vec3f& operator+=(Vec3f&& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vec3f& operator+=(const Vec3f& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vec3f& operator-=(Vec3f&& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	Vec3f& operator-=(const Vec3f& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	bool operator==(Vec3f&& v) const
	{
		return GameMath::Abs(x - v.x) <= EPSILON && GameMath::Abs(y - v.y) <= EPSILON && GameMath::Abs(z - v.z) <= EPSILON;
	}

	bool operator==(const Vec3f& v) const
	{
		return GameMath::Abs(x - v.x) <= EPSILON && GameMath::Abs(y - v.y) <= EPSILON && GameMath::Abs(z - v.z) <= EPSILON;
	}

	bool operator!=(Vec3f&& v) const
	{
		return GameMath::Abs(x - v.x) > EPSILON || GameMath::Abs(y - v.y) > EPSILON || GameMath::Abs(z - v.z) > EPSILON;
	}

	bool operator!=(const Vec3f& v) const
	{
		return GameMath::Abs(x - v.x) > EPSILON || GameMath::Abs(y - v.y) > EPSILON || GameMath::Abs(z - v.z) > EPSILON;
	}

	const float* GetPtr() const { return &data[0]; }
	float* GetPtr() { return &data[0]; }

	static inline float Dot(const Vec3f& lhs, const Vec3f& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	static inline Vec3f Cross(const Vec3f& lhs, const Vec3f& rhs)
	{
		return Vec3f(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}

	static inline float LengthSq(const Vec3f& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	static inline float Length(const Vec3f& v)
	{
		float lengthSq = LengthSq(v);
		return GameMath::Sqrt(lengthSq);
	}

	static inline Vec3f Normalize(const Vec3f& v)
	{
		float length = Length(v);

		if (GameMath::NearZero(length))
		{
			return v;
		}

		float invLength = 1.0f / length;
		return Vec3f(v.x * invLength, v.y * invLength, v.z * invLength);
	}

	static inline float Radian(const Vec3f& lhs, const Vec3f& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return GameMath::ACos(dot / (lengthL * lengthR));
	}

	static inline float Degree(const Vec3f& lhs, const Vec3f& rhs)
	{
		float radian = Radian(lhs, rhs);
		return GameMath::ToDegree(radian);
	}

	static inline Vec3f Project(const Vec3f& target, const Vec3f& base)
	{
		float dot = Dot(target, base);
		float lengthSq = LengthSq(base);
		float scale = dot / lengthSq;

		return Vec3f(base.x * scale, base.y * scale, base.z * scale);
	}

	static inline Vec3f Reject(const Vec3f& target, const Vec3f& base)
	{
		Vec3f project = Project(target, base);
		return target - project;
	}

	static inline Vec3f Lerp(const Vec3f& s, const Vec3f& e, const float& t)
	{
		return s * (1.0f - t) + e * t;
	}

	static inline Vec3f Slerp(const Vec3f& s, const Vec3f& e, const float& t)
	{
		Vec3f start = Normalize(s);
		Vec3f end = Normalize(e);

		float theta = Radian(start, end);
		float sinTheta = GameMath::Sin(theta);

		float a = GameMath::Sin((1.0f - t) * theta) / sinTheta;
		float b = GameMath::Sin(t * theta) / sinTheta;

		return s * a + e * b;
	}

	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float data[3];
	};
};

struct Vec4i
{
	Vec4i() noexcept : x(0), y(0), z(0), w(0) {}
	Vec4i(int32_t&& xx, int32_t&& yy, int32_t&& zz, int32_t&& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}
	Vec4i(const int32_t& xx, const int32_t& yy, const int32_t& zz, const int32_t& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}
	Vec4i(int32_t&& e) noexcept : x(e), y(e), z(e), w(e) {}
	Vec4i(const int32_t& e) noexcept : x(e), y(e), z(e), w(e) {}
	Vec4i(Vec4i&& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}
	Vec4i(const Vec4i& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}

	Vec4i& operator=(Vec4i&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}

	Vec4i& operator=(const Vec4i& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}

	Vec4i operator-() const
	{
		return Vec4i(-x, -y, -z, -w);
	}

	Vec4i operator+(Vec4i&& v) const
	{
		return Vec4i(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vec4i operator+(const Vec4i& v) const
	{
		return Vec4i(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vec4i operator-(Vec4i&& v) const
	{
		return Vec4i(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vec4i operator-(const Vec4i& v) const
	{
		return Vec4i(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vec4i operator*(Vec4i&& v) const
	{
		return Vec4i(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	Vec4i operator*(const Vec4i& v) const
	{
		return Vec4i(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	Vec4i& operator+=(Vec4i&& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	Vec4i& operator+=(const Vec4i& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	Vec4i& operator-=(Vec4i&& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	Vec4i& operator-=(const Vec4i& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	bool operator==(Vec4i&& v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
	}

	bool operator==(const Vec4i& v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
	}

	bool operator!=(Vec4i&& v) const
	{
		return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
	}

	bool operator!=(const Vec4i& v) const
	{
		return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
	}

	const int32_t* GetPtr() const { return &data[0]; }
	int32_t* GetPtr() { return &data[0]; }

	static inline int32_t Dot(const Vec4i& lhs, const Vec4i& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	static inline int32_t LengthSq(const Vec4i& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	}

	static inline float Length(const Vec4i& v)
	{
		float lengthSq = static_cast<float>(LengthSq(v));
		return GameMath::Sqrt(lengthSq);
	}

	static inline float Radian(const Vec4i& lhs, const Vec4i& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return GameMath::ACos(dot / (lengthL * lengthR));
	}

	static inline float Degree(const Vec4i& lhs, const Vec4i& rhs)
	{
		float radian = Radian(lhs, rhs);
		return GameMath::ToDegree(radian);
	}

	union
	{
		struct
		{
			int32_t x;
			int32_t y;
			int32_t z;
			int32_t w;
		};
		int32_t data[4];
	};
};

struct Vec4f
{
	Vec4f() noexcept : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vec4f(float&& xx, float&& yy, float&& zz, float&& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}
	Vec4f(const float& xx, const float& yy, const float& zz, const float& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}
	Vec4f(float&& e) noexcept : x(e), y(e), z(e), w(e) {}
	Vec4f(const float& e) noexcept : x(e), y(e), z(e), w(e) {}
	Vec4f(Vec4f&& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}
	Vec4f(const Vec4f& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}

	Vec4f& operator=(Vec4f&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}

	Vec4f& operator=(const Vec4f& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}

	Vec4f operator-() const
	{
		return Vec4f(-x, -y, -z, -w);
	}

	Vec4f operator+(Vec4f&& v) const
	{
		return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vec4f operator+(const Vec4f& v) const
	{
		return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vec4f operator-(Vec4f&& v) const
	{
		return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vec4f operator-(const Vec4f& v) const
	{
		return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vec4f operator*(Vec4f&& v) const
	{
		return Vec4f(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	Vec4f operator*(const Vec4f& v) const
	{
		return Vec4f(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	Vec4f& operator+=(Vec4f&& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	Vec4f& operator+=(const Vec4f& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	Vec4f& operator-=(Vec4f&& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	Vec4f& operator-=(const Vec4f& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	bool operator==(Vec4f&& v) const
	{
		return GameMath::Abs(x - v.x) <= EPSILON && GameMath::Abs(y - v.y) <= EPSILON && GameMath::Abs(z - v.z) <= EPSILON && GameMath::Abs(w - v.w) <= EPSILON;
	}

	bool operator==(const Vec4f& v) const
	{
		return GameMath::Abs(x - v.x) <= EPSILON && GameMath::Abs(y - v.y) <= EPSILON && GameMath::Abs(z - v.z) <= EPSILON && GameMath::Abs(w - v.w) <= EPSILON;
	}

	bool operator!=(Vec4f&& v) const
	{
		return GameMath::Abs(x - v.x) > EPSILON || GameMath::Abs(y - v.y) > EPSILON || GameMath::Abs(z - v.z) > EPSILON || GameMath::Abs(w - v.w) > EPSILON;
	}

	bool operator!=(const Vec4f& v) const
	{
		return GameMath::Abs(x - v.x) > EPSILON || GameMath::Abs(y - v.y) > EPSILON || GameMath::Abs(z - v.z) > EPSILON || GameMath::Abs(w - v.w) > EPSILON;
	}

	const float* GetPtr() const { return &data[0]; }
	float* GetPtr() { return &data[0]; }

	static inline float Dot(const Vec4f& lhs, const Vec4f& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	static inline float LengthSq(const Vec4f& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	}

	static inline float Length(const Vec4f& v)
	{
		float lengthSq = LengthSq(v);
		return GameMath::Sqrt(lengthSq);
	}

	static inline Vec4f Normalize(const Vec4f& v)
	{
		float length = Length(v);

		if (GameMath::NearZero(length))
		{
			return v;
		}

		float invLength = 1.0f / length;
		return Vec4f(v.x * invLength, v.y * invLength, v.z * invLength, v.w * invLength);
	}

	static inline float Radian(const Vec4f& lhs, const Vec4f& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return GameMath::ACos(dot / (lengthL * lengthR));
	}

	static inline float Degree(const Vec4f& lhs, const Vec4f& rhs)
	{
		float radian = Radian(lhs, rhs);
		return GameMath::ToDegree(radian);
	}

	static inline Vec4f Project(const Vec4f& target, const Vec4f& base)
	{
		float dot = Dot(target, base);
		float lengthSq = LengthSq(base);
		float scale = dot / lengthSq;

		return Vec4f(base.x * scale, base.y * scale, base.z * scale, base.w * scale);
	}

	static inline Vec4f Reject(const Vec4f& target, const Vec4f& base)
	{
		Vec4f project = Project(target, base);
		return target - project;
	}

	static inline Vec4f Lerp(const Vec4f& s, const Vec4f& e, const float& t)
	{
		return s * (1.0f - t) + e * t;
	}

	static inline Vec4f Slerp(const Vec4f& s, const Vec4f& e, const float& t)
	{
		Vec4f start = Normalize(s);
		Vec4f end = Normalize(e);

		float theta = Radian(start, end);
		float sinTheta = GameMath::Sin(theta);

		float a = GameMath::Sin((1.0f - t) * theta) / sinTheta;
		float b = GameMath::Sin(t * theta) / sinTheta;

		return s * a + e * b;
	}

	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float data[4];
	};
};

struct Mat2x2
{
	Mat2x2() noexcept
	{
		e00 = 0.0f; e01 = 0.0f;
		e10 = 0.0f; e11 = 0.0f;
	}

	Mat2x2(
		float&& ee00, float&& ee01,
		float&& ee10, float&& ee11
	) noexcept
	{
		e00 = ee00; e01 = ee01;
		e10 = ee10; e11 = ee11;
	}

	Mat2x2(
		const float& ee00, const float& ee01,
		const float& ee10, const float& ee11
	) noexcept
	{
		e00 = ee00; e01 = ee01;
		e10 = ee10; e11 = ee11;
	}

	Mat2x2(float&& e) noexcept
	{
		e00 = e; e01 = e;
		e10 = e; e11 = e;
	}

	Mat2x2(const float& e) noexcept
	{
		e00 = e; e01 = e;
		e10 = e; e11 = e;
	}

	Mat2x2(Mat2x2&& m) noexcept
	{
		e00 = m.e00; e01 = m.e01;
		e10 = m.e10; e11 = m.e11;
	}

	Mat2x2(const Mat2x2& m) noexcept
	{
		e00 = m.e00; e01 = m.e01;
		e10 = m.e10; e11 = m.e11;
	}

	Mat2x2& operator=(Mat2x2&& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01;
		e10 = m.e10; e11 = m.e11;

		return *this;
	}

	Mat2x2& operator=(const Mat2x2& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01;
		e10 = m.e10; e11 = m.e11;

		return *this;
	}

	Mat2x2 operator-() const
	{
		return Mat2x2(
			-e00, -e01,
			-e10, -e11
		);
	}

	Mat2x2 operator+(Mat2x2&& m) const
	{
		return Mat2x2(
			e00 + m.e00, e01 + m.e01,
			e10 + m.e10, e11 + m.e11
		);
	}

	Mat2x2 operator+(const Mat2x2& m) const
	{
		return Mat2x2(
			e00 + m.e00, e01 + m.e01,
			e10 + m.e10, e11 + m.e11
		);
	}

	Mat2x2 operator-(Mat2x2&& m) const
	{
		return Mat2x2(
			e00 - m.e00, e01 - m.e01,
			e10 - m.e10, e11 - m.e11
		);
	}

	Mat2x2 operator-(const Mat2x2& m) const
	{
		return Mat2x2(
			e00 - m.e00, e01 - m.e01,
			e10 - m.e10, e11 - m.e11
		);
	}

	Mat2x2 operator*(float&& scalar) const
	{
		return Mat2x2(
			scalar * e00, scalar * e01,
			scalar * e10, scalar * e11
		);
	}

	Mat2x2 operator*(const float& scalar) const
	{
		return Mat2x2(
			scalar * e00, scalar * e01,
			scalar * e10, scalar * e11
		);
	}

	Mat2x2 operator*(Mat2x2&& m) const
	{
		return Mat2x2(
			e00 * m.e00 + e01 * m.e10,
			e00 * m.e01 + e01 * m.e11,
			e10 * m.e00 + e11 * m.e10,
			e10 * m.e01 + e11 * m.e11
		);
	}

	Mat2x2 operator*(const Mat2x2& m) const
	{
		return Mat2x2(
			e00 * m.e00 + e01 * m.e10,
			e00 * m.e01 + e01 * m.e11,
			e10 * m.e00 + e11 * m.e10,
			e10 * m.e01 + e11 * m.e11
		);
	}

	Mat2x2& operator+=(Mat2x2&& m) noexcept
	{
		e00 += m.e00; e01 += m.e01;
		e10 += m.e10; e11 += m.e11;

		return *this;
	}

	Mat2x2& operator+=(const Mat2x2& m) noexcept
	{
		e00 += m.e00; e01 += m.e01;
		e10 += m.e10; e11 += m.e11;

		return *this;
	}

	Mat2x2& operator-=(Mat2x2&& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01;
		e10 -= m.e10; e11 -= m.e11;

		return *this;
	}

	Mat2x2& operator-=(const Mat2x2& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01;
		e10 -= m.e10; e11 -= m.e11;

		return *this;
	}

	bool operator==(Mat2x2&& m) const
	{
		return GameMath::Abs(e00 - m.e00) <= EPSILON
			&& GameMath::Abs(e01 - m.e01) <= EPSILON
			&& GameMath::Abs(e10 - m.e10) <= EPSILON
			&& GameMath::Abs(e11 - m.e11) <= EPSILON;
	}

	bool operator==(const Mat2x2& m) const
	{
		return GameMath::Abs(e00 - m.e00) <= EPSILON
			&& GameMath::Abs(e01 - m.e01) <= EPSILON
			&& GameMath::Abs(e10 - m.e10) <= EPSILON
			&& GameMath::Abs(e11 - m.e11) <= EPSILON;
	}

	bool operator!=(Mat2x2&& m) const
	{
		return GameMath::Abs(e00 - m.e00) > EPSILON
			|| GameMath::Abs(e01 - m.e01) > EPSILON
			|| GameMath::Abs(e10 - m.e10) > EPSILON
			|| GameMath::Abs(e11 - m.e11) > EPSILON;
	}

	bool operator!=(const Mat2x2& m) const
	{
		return GameMath::Abs(e00 - m.e00) > EPSILON
			|| GameMath::Abs(e01 - m.e01) > EPSILON
			|| GameMath::Abs(e10 - m.e10) > EPSILON
			|| GameMath::Abs(e11 - m.e11) > EPSILON;
	}

	const float* GetPtr() const { return &data[0]; }
	float* GetPtr() { return &data[0]; }

	static inline Mat2x2 Zero()
	{
		return Mat2x2(
			0.0f, 0.0f,
			0.0f, 0.0f
		);
	}

	static inline Mat2x2 Identity()
	{
		return Mat2x2(
			1.0f, 0.0f,
			0.0f, 1.0f
		);
	}

	static inline Mat2x2 Transpose(const Mat2x2& m)
	{
		return Mat2x2(
			m.e00, m.e10,
			m.e01, m.e11
		);
	}

	static inline float Determinant(const Mat2x2& m)
	{
		return m.e00 * m.e11 - m.e01 * m.e10;
	}

	static inline Mat2x2 Inverse(const Mat2x2& m)
	{
		float oneOverDeterminant = 1.0f / Determinant(m);

		return Mat2x2(
			+m.e11 * oneOverDeterminant,
			-m.e01 * oneOverDeterminant,
			-m.e10 * oneOverDeterminant,
			+m.e00 * oneOverDeterminant
		);
	}

	union
	{
		struct
		{
			float e00; float e01;
			float e10; float e11;
		};
		float data[4];
	};
};

struct Mat3x3
{
	Mat3x3() noexcept
	{
		e00 = 0.0f; e01 = 0.0f; e02 = 0.0f;
		e10 = 0.0f; e11 = 0.0f; e12 = 0.0f;
		e20 = 0.0f; e21 = 0.0f; e22 = 0.0f;
	}

	Mat3x3(
		float&& ee00, float&& ee01, float&& ee02,
		float&& ee10, float&& ee11, float&& ee12,
		float&& ee20, float&& ee21, float&& ee22
	) noexcept
	{
		e00 = ee00; e01 = ee01; e02 = ee02;
		e10 = ee10; e11 = ee11; e12 = ee12;
		e20 = ee20; e21 = ee21; e22 = ee22;
	}

	Mat3x3(
		const float& ee00, const float& ee01, const float& ee02,
		const float& ee10, const float& ee11, const float& ee12,
		const float& ee20, const float& ee21, const float& ee22
	) noexcept
	{
		e00 = ee00; e01 = ee01; e02 = ee02;
		e10 = ee10; e11 = ee11; e12 = ee12;
		e20 = ee20; e21 = ee21; e22 = ee22;
	}

	Mat3x3(float&& e) noexcept
	{
		e00 = e; e01 = e; e02 = e;
		e10 = e; e11 = e; e12 = e;
		e20 = e; e21 = e; e22 = e;
	}

	Mat3x3(const float& e) noexcept
	{
		e00 = e; e01 = e; e02 = e;
		e10 = e; e11 = e; e12 = e;
		e20 = e; e21 = e; e22 = e;
	}

	Mat3x3(Mat3x3&& m) noexcept
	{
		e00 = m.e00; e01 = m.e01; e02 = m.e02;
		e10 = m.e10; e11 = m.e11; e12 = m.e12;
		e20 = m.e20; e21 = m.e21; e22 = m.e22;
	}

	Mat3x3(const Mat3x3& m) noexcept
	{
		e00 = m.e00; e01 = m.e01; e02 = m.e02;
		e10 = m.e10; e11 = m.e11; e12 = m.e12;
		e20 = m.e20; e21 = m.e21; e22 = m.e22;
	}

	Mat3x3& operator=(Mat3x3&& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01; e02 = m.e02;
		e10 = m.e10; e11 = m.e11; e12 = m.e12;
		e20 = m.e20; e21 = m.e21; e22 = m.e22;

		return *this;
	}

	Mat3x3& operator=(const Mat3x3& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01; e02 = m.e02;
		e10 = m.e10; e11 = m.e11; e12 = m.e12;
		e20 = m.e20; e21 = m.e21; e22 = m.e22;

		return *this;
	}

	Mat3x3 operator-() const
	{
		return Mat3x3(
			-e00, -e01, -e02,
			-e10, -e11, -e12,
			-e20, -e21, -e22
		);
	}

	Mat3x3 operator+(Mat3x3&& m) const
	{
		return Mat3x3(
			e00 + m.e00, e01 + m.e01, e02 + m.e02,
			e10 + m.e10, e11 + m.e11, e12 + m.e12,
			e20 + m.e20, e21 + m.e21, e22 + m.e22
		);
	}

	Mat3x3 operator+(const Mat3x3& m) const
	{
		return Mat3x3(
			e00 + m.e00, e01 + m.e01, e02 + m.e02,
			e10 + m.e10, e11 + m.e11, e12 + m.e12,
			e20 + m.e20, e21 + m.e21, e22 + m.e22
		);
	}

	Mat3x3 operator-(Mat3x3&& m) const
	{
		return Mat3x3(
			e00 - m.e00, e01 - m.e01, e02 - m.e02,
			e10 - m.e10, e11 - m.e11, e12 - m.e12,
			e20 - m.e20, e21 - m.e21, e22 - m.e22
		);
	}

	Mat3x3 operator-(const Mat3x3& m) const
	{
		return Mat3x3(
			e00 - m.e00, e01 - m.e01, e02 - m.e02,
			e10 - m.e10, e11 - m.e11, e12 - m.e12,
			e20 - m.e20, e21 - m.e21, e22 - m.e22
		);
	}

	Mat3x3 operator*(float&& scalar) const
	{
		return Mat3x3(
			scalar * e00, scalar * e01, scalar * e02,
			scalar * e10, scalar * e11, scalar * e12,
			scalar * e20, scalar * e21, scalar * e22
		);
	}

	Mat3x3 operator*(const float& scalar) const
	{
		return Mat3x3(
			scalar * e00, scalar * e01, scalar * e02,
			scalar * e10, scalar * e11, scalar * e12,
			scalar * e20, scalar * e21, scalar * e22
		);
	}

	Mat3x3 operator*(Mat3x3&& m) const
	{
		return Mat3x3(
			e00 * m.e00 + e01 * m.e10 + e02 * m.e20,
			e00 * m.e01 + e01 * m.e11 + e02 * m.e21,
			e00 * m.e02 + e01 * m.e12 + e02 * m.e22,
			e10 * m.e00 + e11 * m.e10 + e12 * m.e20,
			e10 * m.e01 + e11 * m.e11 + e12 * m.e21,
			e10 * m.e02 + e11 * m.e12 + e12 * m.e22,
			e20 * m.e00 + e21 * m.e10 + e22 * m.e20,
			e20 * m.e01 + e21 * m.e11 + e22 * m.e21,
			e20 * m.e02 + e21 * m.e12 + e22 * m.e22
		);
	}

	Mat3x3 operator*(const Mat3x3& m) const
	{
		return Mat3x3(
			e00 * m.e00 + e01 * m.e10 + e02 * m.e20,
			e00 * m.e01 + e01 * m.e11 + e02 * m.e21,
			e00 * m.e02 + e01 * m.e12 + e02 * m.e22,
			e10 * m.e00 + e11 * m.e10 + e12 * m.e20,
			e10 * m.e01 + e11 * m.e11 + e12 * m.e21,
			e10 * m.e02 + e11 * m.e12 + e12 * m.e22,
			e20 * m.e00 + e21 * m.e10 + e22 * m.e20,
			e20 * m.e01 + e21 * m.e11 + e22 * m.e21,
			e20 * m.e02 + e21 * m.e12 + e22 * m.e22
		);
	}

	Mat3x3& operator+=(Mat3x3&& m) noexcept
	{
		e00 += m.e00; e01 += m.e01; e02 += m.e02;
		e10 += m.e10; e11 += m.e11; e12 += m.e12;
		e20 += m.e20; e21 += m.e21; e22 += m.e22;

		return *this;
	}

	Mat3x3& operator+=(const Mat3x3& m) noexcept
	{
		e00 += m.e00; e01 += m.e01; e02 += m.e02;
		e10 += m.e10; e11 += m.e11; e12 += m.e12;
		e20 += m.e20; e21 += m.e21; e22 += m.e22;

		return *this;
	}

	Mat3x3& operator-=(Mat3x3&& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01; e02 -= m.e02;
		e10 -= m.e10; e11 -= m.e11; e12 -= m.e12;
		e20 -= m.e20; e21 -= m.e21; e22 -= m.e22;

		return *this;
	}

	Mat3x3& operator-=(const Mat3x3& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01; e02 -= m.e02;
		e10 -= m.e10; e11 -= m.e11; e12 -= m.e12;
		e20 -= m.e20; e21 -= m.e21; e22 -= m.e22;

		return *this;
	}

	bool operator==(Mat3x3&& m) const
	{
		return GameMath::Abs(e00 - m.e00) <= EPSILON
			&& GameMath::Abs(e01 - m.e01) <= EPSILON
			&& GameMath::Abs(e02 - m.e02) <= EPSILON
			&& GameMath::Abs(e10 - m.e10) <= EPSILON
			&& GameMath::Abs(e11 - m.e11) <= EPSILON
			&& GameMath::Abs(e12 - m.e12) <= EPSILON
			&& GameMath::Abs(e20 - m.e20) <= EPSILON
			&& GameMath::Abs(e21 - m.e21) <= EPSILON
			&& GameMath::Abs(e22 - m.e22) <= EPSILON;
	}

	bool operator==(const Mat3x3& m) const
	{
		return GameMath::Abs(e00 - m.e00) <= EPSILON
			&& GameMath::Abs(e01 - m.e01) <= EPSILON
			&& GameMath::Abs(e02 - m.e02) <= EPSILON
			&& GameMath::Abs(e10 - m.e10) <= EPSILON
			&& GameMath::Abs(e11 - m.e11) <= EPSILON
			&& GameMath::Abs(e12 - m.e12) <= EPSILON
			&& GameMath::Abs(e20 - m.e20) <= EPSILON
			&& GameMath::Abs(e21 - m.e21) <= EPSILON
			&& GameMath::Abs(e22 - m.e22) <= EPSILON;
	}

	bool operator!=(Mat3x3&& m) const
	{
		return GameMath::Abs(e00 - m.e00) > EPSILON
			|| GameMath::Abs(e01 - m.e01) > EPSILON
			|| GameMath::Abs(e02 - m.e02) > EPSILON
			|| GameMath::Abs(e10 - m.e10) > EPSILON
			|| GameMath::Abs(e11 - m.e11) > EPSILON
			|| GameMath::Abs(e12 - m.e12) > EPSILON
			|| GameMath::Abs(e20 - m.e20) > EPSILON
			|| GameMath::Abs(e21 - m.e21) > EPSILON
			|| GameMath::Abs(e22 - m.e22) > EPSILON;
	}

	bool operator!=(const Mat3x3& m) const
	{
		return GameMath::Abs(e00 - m.e00) > EPSILON
			|| GameMath::Abs(e01 - m.e01) > EPSILON
			|| GameMath::Abs(e02 - m.e02) > EPSILON
			|| GameMath::Abs(e10 - m.e10) > EPSILON
			|| GameMath::Abs(e11 - m.e11) > EPSILON
			|| GameMath::Abs(e12 - m.e12) > EPSILON
			|| GameMath::Abs(e20 - m.e20) > EPSILON
			|| GameMath::Abs(e21 - m.e21) > EPSILON
			|| GameMath::Abs(e22 - m.e22) > EPSILON;
	}

	const float* GetPtr() const { return &data[0]; }
	float* GetPtr() { return &data[0]; }

	static inline Mat3x3 Zero()
	{
		return Mat3x3(
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f
		);
	}

	static inline Mat3x3 Identity()
	{
		return Mat3x3(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	static inline Mat3x3 Transpose(const Mat3x3& m)
	{
		return Mat3x3(
			m.e00, m.e10, m.e20,
			m.e01, m.e11, m.e21,
			m.e02, m.e12, m.e22
		);
	}

	static inline float Determinant(const Mat3x3& m)
	{
		return m.e00 * (m.e11 * m.e22 - m.e21 * m.e12) - m.e10 * (m.e01 * m.e22 - m.e21 * m.e02) + m.e20 * (m.e01 * m.e12 - m.e11 * m.e02);
	}

	static inline Mat3x3 Inverse(const Mat3x3& m)
	{
		float oneOverDeterminant = 1.0f / Determinant(m);

		return Mat3x3(
			+(m.e11 * m.e22 - m.e21 * m.e12) * oneOverDeterminant,
			-(m.e10 * m.e22 - m.e20 * m.e12) * oneOverDeterminant,
			+(m.e10 * m.e21 - m.e20 * m.e11) * oneOverDeterminant,
			-(m.e01 * m.e22 - m.e21 * m.e02) * oneOverDeterminant,
			+(m.e00 * m.e22 - m.e20 * m.e02) * oneOverDeterminant,
			-(m.e00 * m.e21 - m.e20 * m.e01) * oneOverDeterminant,
			+(m.e01 * m.e12 - m.e11 * m.e02) * oneOverDeterminant,
			-(m.e00 * m.e12 - m.e10 * m.e02) * oneOverDeterminant,
			+(m.e00 * m.e11 - m.e10 * m.e01) * oneOverDeterminant
		);
	}

	union
	{
		struct
		{
			float e00; float e01; float e02;
			float e10; float e11; float e12;
			float e20; float e21; float e22;
		};
		float data[9];
	};
};

struct Mat4x4
{
	Mat4x4() noexcept
	{
		e00 = 0.0f; e01 = 0.0f; e02 = 0.0f; e03 = 0.0f;
		e10 = 0.0f; e11 = 0.0f; e12 = 0.0f; e13 = 0.0f;
		e20 = 0.0f; e21 = 0.0f; e22 = 0.0f; e23 = 0.0f;
		e30 = 0.0f; e31 = 0.0f; e32 = 0.0f; e33 = 0.0f;
	}

	Mat4x4(
		float&& ee00, float&& ee01, float&& ee02, float&& ee03,
		float&& ee10, float&& ee11, float&& ee12, float&& ee13,
		float&& ee20, float&& ee21, float&& ee22, float&& ee23,
		float&& ee30, float&& ee31, float&& ee32, float&& ee33
	) noexcept
	{
		e00 = ee00; e01 = ee01; e02 = ee02; e03 = ee03;
		e10 = ee10; e11 = ee11; e12 = ee12; e13 = ee13;
		e20 = ee20; e21 = ee21; e22 = ee22; e23 = ee23;
		e30 = ee30; e31 = ee31; e32 = ee32; e33 = ee33;
	}

	Mat4x4(
		const float& ee00, const float& ee01, const float& ee02, const float& ee03,
		const float& ee10, const float& ee11, const float& ee12, const float& ee13,
		const float& ee20, const float& ee21, const float& ee22, const float& ee23,
		const float& ee30, const float& ee31, const float& ee32, const float& ee33
	) noexcept
	{
		e00 = ee00; e01 = ee01; e02 = ee02; e03 = ee03;
		e10 = ee10; e11 = ee11; e12 = ee12; e13 = ee13;
		e20 = ee20; e21 = ee21; e22 = ee22; e23 = ee23;
		e30 = ee30; e31 = ee31; e32 = ee32; e33 = ee33;
	}

	Mat4x4(float&& e) noexcept
	{
		e00 = e; e01 = e; e02 = e; e03 = e;
		e10 = e; e11 = e; e12 = e; e13 = e;
		e20 = e; e21 = e; e22 = e; e23 = e;
		e30 = e; e31 = e; e32 = e; e33 = e;
	}

	Mat4x4(const float& e) noexcept
	{
		e00 = e; e01 = e; e02 = e; e03 = e;
		e10 = e; e11 = e; e12 = e; e13 = e;
		e20 = e; e21 = e; e22 = e; e23 = e;
		e30 = e; e31 = e; e32 = e; e33 = e;
	}

	Mat4x4(Mat4x4&& m) noexcept
	{
		e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = m.e03;
		e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = m.e13;
		e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = m.e23;
		e30 = m.e30; e31 = m.e31; e32 = m.e32; e33 = m.e33;
	}

	Mat4x4(const Mat4x4& m) noexcept
	{
		e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = m.e03;
		e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = m.e13;
		e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = m.e23;
		e30 = m.e30; e31 = m.e31; e32 = m.e32; e33 = m.e33;
	}

	Mat4x4& operator=(Mat4x4&& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = m.e03;
		e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = m.e13;
		e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = m.e23;
		e30 = m.e30; e31 = m.e31; e32 = m.e32; e33 = m.e33;

		return *this;
	}

	Mat4x4& operator=(const Mat4x4& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = m.e03;
		e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = m.e13;
		e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = m.e23;
		e30 = m.e30; e31 = m.e31; e32 = m.e32; e33 = m.e33;

		return *this;
	}

	Mat4x4 operator-() const
	{
		return Mat4x4(
			-e00, -e01, -e02, -e03,
			-e10, -e11, -e12, -e13,
			-e20, -e21, -e22, -e23,
			-e30, -e31, -e32, -e33
		);
	}

	Mat4x4 operator+(Mat4x4&& m) const
	{
		return Mat4x4(
			e00 + m.e00, e01 + m.e01, e02 + m.e02, e03 + m.e03,
			e10 + m.e10, e11 + m.e11, e12 + m.e12, e13 + m.e13,
			e20 + m.e20, e21 + m.e21, e22 + m.e22, e23 + m.e23,
			e30 + m.e30, e31 + m.e31, e32 + m.e32, e33 + m.e33
		);
	}

	Mat4x4 operator+(const Mat4x4& m) const
	{
		return Mat4x4(
			e00 + m.e00, e01 + m.e01, e02 + m.e02, e03 + m.e03,
			e10 + m.e10, e11 + m.e11, e12 + m.e12, e13 + m.e13,
			e20 + m.e20, e21 + m.e21, e22 + m.e22, e23 + m.e23,
			e30 + m.e30, e31 + m.e31, e32 + m.e32, e33 + m.e33
		);
	}

	Mat4x4 operator-(Mat4x4&& m) const
	{
		return Mat4x4(
			e00 - m.e00, e01 - m.e01, e02 - m.e02, e03 - m.e03,
			e10 - m.e10, e11 - m.e11, e12 - m.e12, e13 - m.e13,
			e20 - m.e20, e21 - m.e21, e22 - m.e22, e23 - m.e23,
			e30 - m.e30, e31 - m.e31, e32 - m.e32, e33 - m.e33
		);
	}

	Mat4x4 operator-(const Mat4x4& m) const
	{
		return Mat4x4(
			e00 - m.e00, e01 - m.e01, e02 - m.e02, e03 - m.e03,
			e10 - m.e10, e11 - m.e11, e12 - m.e12, e13 - m.e13,
			e20 - m.e20, e21 - m.e21, e22 - m.e22, e23 - m.e23,
			e30 - m.e30, e31 - m.e31, e32 - m.e32, e33 - m.e33
		);
	}

	Mat4x4 operator*(float&& scalar) const
	{
		return Mat4x4(
			scalar * e00, scalar * e01, scalar * e02, scalar * e03,
			scalar * e10, scalar * e11, scalar * e12, scalar * e13,
			scalar * e20, scalar * e21, scalar * e22, scalar * e23,
			scalar * e30, scalar * e31, scalar * e32, scalar * e33
		);
	}

	Mat4x4 operator*(const float& scalar) const
	{
		return Mat4x4(
			scalar * e00, scalar * e01, scalar * e02, scalar * e03,
			scalar * e10, scalar * e11, scalar * e12, scalar * e13,
			scalar * e20, scalar * e21, scalar * e22, scalar * e23,
			scalar * e30, scalar * e31, scalar * e32, scalar * e33
		);
	}

	Mat4x4 operator*(Mat4x4&& m) const
	{
		return Mat4x4(
			e00 * m.e00 + e01 * m.e10 + e02 * m.e20 + e03 * m.e30,
			e00 * m.e01 + e01 * m.e11 + e02 * m.e21 + e03 * m.e31,
			e00 * m.e02 + e01 * m.e12 + e02 * m.e22 + e03 * m.e32,
			e00 * m.e03 + e01 * m.e13 + e02 * m.e23 + e03 * m.e33,
			e10 * m.e00 + e11 * m.e10 + e12 * m.e20 + e13 * m.e30,
			e10 * m.e01 + e11 * m.e11 + e12 * m.e21 + e13 * m.e31,
			e10 * m.e02 + e11 * m.e12 + e12 * m.e22 + e13 * m.e32,
			e10 * m.e03 + e11 * m.e13 + e12 * m.e23 + e13 * m.e33,
			e20 * m.e00 + e21 * m.e10 + e22 * m.e20 + e23 * m.e30,
			e20 * m.e01 + e21 * m.e11 + e22 * m.e21 + e23 * m.e31,
			e20 * m.e02 + e21 * m.e12 + e22 * m.e22 + e23 * m.e32,
			e20 * m.e03 + e21 * m.e13 + e22 * m.e23 + e23 * m.e33,
			e30 * m.e00 + e31 * m.e10 + e32 * m.e20 + e33 * m.e30,
			e30 * m.e01 + e31 * m.e11 + e32 * m.e21 + e33 * m.e31,
			e30 * m.e02 + e31 * m.e12 + e32 * m.e22 + e33 * m.e32,
			e30 * m.e03 + e31 * m.e13 + e32 * m.e23 + e33 * m.e33
		);
	}

	Mat4x4 operator*(const Mat4x4& m) const
	{
		return Mat4x4(
			e00 * m.e00 + e01 * m.e10 + e02 * m.e20 + e03 * m.e30,
			e00 * m.e01 + e01 * m.e11 + e02 * m.e21 + e03 * m.e31,
			e00 * m.e02 + e01 * m.e12 + e02 * m.e22 + e03 * m.e32,
			e00 * m.e03 + e01 * m.e13 + e02 * m.e23 + e03 * m.e33,
			e10 * m.e00 + e11 * m.e10 + e12 * m.e20 + e13 * m.e30,
			e10 * m.e01 + e11 * m.e11 + e12 * m.e21 + e13 * m.e31,
			e10 * m.e02 + e11 * m.e12 + e12 * m.e22 + e13 * m.e32,
			e10 * m.e03 + e11 * m.e13 + e12 * m.e23 + e13 * m.e33,
			e20 * m.e00 + e21 * m.e10 + e22 * m.e20 + e23 * m.e30,
			e20 * m.e01 + e21 * m.e11 + e22 * m.e21 + e23 * m.e31,
			e20 * m.e02 + e21 * m.e12 + e22 * m.e22 + e23 * m.e32,
			e20 * m.e03 + e21 * m.e13 + e22 * m.e23 + e23 * m.e33,
			e30 * m.e00 + e31 * m.e10 + e32 * m.e20 + e33 * m.e30,
			e30 * m.e01 + e31 * m.e11 + e32 * m.e21 + e33 * m.e31,
			e30 * m.e02 + e31 * m.e12 + e32 * m.e22 + e33 * m.e32,
			e30 * m.e03 + e31 * m.e13 + e32 * m.e23 + e33 * m.e33
		);
	}

	Mat4x4& operator+=(Mat4x4&& m) noexcept
	{
		e00 += m.e00; e01 += m.e01; e02 += m.e02; e03 += m.e03;
		e10 += m.e10; e11 += m.e11; e12 += m.e12; e13 += m.e13;
		e20 += m.e20; e21 += m.e21; e22 += m.e22; e23 += m.e23;
		e30 += m.e30; e31 += m.e31; e32 += m.e32; e33 += m.e33;

		return *this;
	}

	Mat4x4& operator+=(const Mat4x4& m) noexcept
	{
		e00 += m.e00; e01 += m.e01; e02 += m.e02; e03 += m.e03;
		e10 += m.e10; e11 += m.e11; e12 += m.e12; e13 += m.e13;
		e20 += m.e20; e21 += m.e21; e22 += m.e22; e23 += m.e23;
		e30 += m.e30; e31 += m.e31; e32 += m.e32; e33 += m.e33;

		return *this;
	}

	Mat4x4& operator-=(Mat4x4&& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01; e02 -= m.e02; e03 -= m.e03;
		e10 -= m.e10; e11 -= m.e11; e12 -= m.e12; e13 -= m.e13;
		e20 -= m.e20; e21 -= m.e21; e22 -= m.e22; e23 -= m.e23;
		e30 -= m.e30; e31 -= m.e31; e32 -= m.e32; e33 -= m.e33;

		return *this;
	}

	Mat4x4& operator-=(const Mat4x4& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01; e02 -= m.e02; e03 -= m.e03;
		e10 -= m.e10; e11 -= m.e11; e12 -= m.e12; e13 -= m.e13;
		e20 -= m.e20; e21 -= m.e21; e22 -= m.e22; e23 -= m.e23;
		e30 -= m.e30; e31 -= m.e31; e32 -= m.e32; e33 -= m.e33;

		return *this;
	}

	bool operator==(Mat4x4&& m) const
	{
		return GameMath::Abs(e00 - m.e00) <= EPSILON
			&& GameMath::Abs(e01 - m.e01) <= EPSILON
			&& GameMath::Abs(e02 - m.e02) <= EPSILON
			&& GameMath::Abs(e03 - m.e03) <= EPSILON
			&& GameMath::Abs(e10 - m.e10) <= EPSILON
			&& GameMath::Abs(e11 - m.e11) <= EPSILON
			&& GameMath::Abs(e12 - m.e12) <= EPSILON
			&& GameMath::Abs(e13 - m.e13) <= EPSILON
			&& GameMath::Abs(e20 - m.e20) <= EPSILON
			&& GameMath::Abs(e21 - m.e21) <= EPSILON
			&& GameMath::Abs(e22 - m.e22) <= EPSILON
			&& GameMath::Abs(e23 - m.e23) <= EPSILON
			&& GameMath::Abs(e30 - m.e30) <= EPSILON
			&& GameMath::Abs(e31 - m.e31) <= EPSILON
			&& GameMath::Abs(e32 - m.e32) <= EPSILON
			&& GameMath::Abs(e33 - m.e33) <= EPSILON;
	}

	bool operator==(const Mat4x4& m) const
	{
		return GameMath::Abs(e00 - m.e00) <= EPSILON
			&& GameMath::Abs(e01 - m.e01) <= EPSILON
			&& GameMath::Abs(e02 - m.e02) <= EPSILON
			&& GameMath::Abs(e03 - m.e03) <= EPSILON
			&& GameMath::Abs(e10 - m.e10) <= EPSILON
			&& GameMath::Abs(e11 - m.e11) <= EPSILON
			&& GameMath::Abs(e12 - m.e12) <= EPSILON
			&& GameMath::Abs(e13 - m.e13) <= EPSILON
			&& GameMath::Abs(e20 - m.e20) <= EPSILON
			&& GameMath::Abs(e21 - m.e21) <= EPSILON
			&& GameMath::Abs(e22 - m.e22) <= EPSILON
			&& GameMath::Abs(e23 - m.e23) <= EPSILON
			&& GameMath::Abs(e30 - m.e30) <= EPSILON
			&& GameMath::Abs(e31 - m.e31) <= EPSILON
			&& GameMath::Abs(e32 - m.e32) <= EPSILON
			&& GameMath::Abs(e33 - m.e33) <= EPSILON;
	}

	bool operator!=(Mat4x4&& m) const
	{
		return GameMath::Abs(e00 - m.e00) > EPSILON
			|| GameMath::Abs(e01 - m.e01) > EPSILON
			|| GameMath::Abs(e02 - m.e02) > EPSILON
			|| GameMath::Abs(e03 - m.e03) > EPSILON
			|| GameMath::Abs(e10 - m.e10) > EPSILON
			|| GameMath::Abs(e11 - m.e11) > EPSILON
			|| GameMath::Abs(e12 - m.e12) > EPSILON
			|| GameMath::Abs(e13 - m.e13) > EPSILON
			|| GameMath::Abs(e20 - m.e20) > EPSILON
			|| GameMath::Abs(e21 - m.e21) > EPSILON
			|| GameMath::Abs(e22 - m.e22) > EPSILON
			|| GameMath::Abs(e23 - m.e23) > EPSILON
			|| GameMath::Abs(e30 - m.e30) > EPSILON
			|| GameMath::Abs(e31 - m.e31) > EPSILON
			|| GameMath::Abs(e32 - m.e32) > EPSILON
			|| GameMath::Abs(e33 - m.e33) > EPSILON;
	}

	bool operator!=(const Mat4x4& m) const
	{
		return GameMath::Abs(e00 - m.e00) > EPSILON
			|| GameMath::Abs(e01 - m.e01) > EPSILON
			|| GameMath::Abs(e02 - m.e02) > EPSILON
			|| GameMath::Abs(e03 - m.e03) > EPSILON
			|| GameMath::Abs(e10 - m.e10) > EPSILON
			|| GameMath::Abs(e11 - m.e11) > EPSILON
			|| GameMath::Abs(e12 - m.e12) > EPSILON
			|| GameMath::Abs(e13 - m.e13) > EPSILON
			|| GameMath::Abs(e20 - m.e20) > EPSILON
			|| GameMath::Abs(e21 - m.e21) > EPSILON
			|| GameMath::Abs(e22 - m.e22) > EPSILON
			|| GameMath::Abs(e23 - m.e23) > EPSILON
			|| GameMath::Abs(e30 - m.e30) > EPSILON
			|| GameMath::Abs(e31 - m.e31) > EPSILON
			|| GameMath::Abs(e32 - m.e32) > EPSILON
			|| GameMath::Abs(e33 - m.e33) > EPSILON;
	}

	const float* GetPtr() const { return &e00; }
	float* GetPtr() { return &e00; }

	static inline Mat4x4 Zero()
	{
		return Mat4x4(
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		);
	}

	static inline Mat4x4 Identity()
	{
		return Mat4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	static inline Mat4x4 Transpose(const Mat4x4& m)
	{
		return Mat4x4(
			m.e00, m.e10, m.e20, m.e30,
			m.e01, m.e11, m.e21, m.e31,
			m.e02, m.e12, m.e22, m.e32,
			m.e03, m.e13, m.e23, m.e33
		);
	}

	static float inline Determinant(const Mat4x4& m)
	{
		float subFactor00 = m.e22 * m.e33 - m.e32 * m.e23;
		float subFactor01 = m.e21 * m.e33 - m.e31 * m.e23;
		float subFactor02 = m.e21 * m.e32 - m.e31 * m.e22;
		float subFactor03 = m.e20 * m.e33 - m.e30 * m.e23;
		float subFactor04 = m.e20 * m.e32 - m.e30 * m.e22;
		float subFactor05 = m.e20 * m.e31 - m.e30 * m.e21;

		float cof0 = +(m.e11 * subFactor00 - m.e12 * subFactor01 + m.e13 * subFactor02);
		float cof1 = -(m.e10 * subFactor00 - m.e12 * subFactor03 + m.e13 * subFactor04);
		float cof2 = +(m.e10 * subFactor01 - m.e11 * subFactor03 + m.e13 * subFactor05);
		float cof3 = -(m.e10 * subFactor02 - m.e11 * subFactor04 + m.e12 * subFactor05);

		return m.e00 * cof0 + m.e01 * cof1 + m.e02 * cof2 + m.e03 * cof3;
	}

	static inline Mat4x4 Inverse(const Mat4x4& m)
	{
		float oneOverDeterminant = 1.0f / Determinant(m);

		float coef00 = m.e22 * m.e33 - m.e32 * m.e23;
		float coef02 = m.e12 * m.e33 - m.e32 * m.e13;
		float coef03 = m.e12 * m.e23 - m.e22 * m.e13;

		float coef04 = m.e21 * m.e33 - m.e31 * m.e23;
		float coef06 = m.e11 * m.e33 - m.e31 * m.e13;
		float coef07 = m.e11 * m.e23 - m.e21 * m.e13;

		float coef08 = m.e21 * m.e32 - m.e31 * m.e22;
		float coef10 = m.e11 * m.e32 - m.e31 * m.e12;
		float coef11 = m.e11 * m.e22 - m.e21 * m.e12;

		float coef12 = m.e20 * m.e33 - m.e30 * m.e23;
		float coef14 = m.e10 * m.e33 - m.e30 * m.e13;
		float coef15 = m.e10 * m.e23 - m.e20 * m.e13;

		float coef16 = m.e20 * m.e32 - m.e30 * m.e22;
		float coef18 = m.e10 * m.e32 - m.e30 * m.e12;
		float coef19 = m.e10 * m.e22 - m.e20 * m.e12;

		float coef20 = m.e20 * m.e31 - m.e30 * m.e21;
		float coef22 = m.e10 * m.e31 - m.e30 * m.e11;
		float coef23 = m.e10 * m.e21 - m.e20 * m.e11;

		Vec4f fac0(coef00, coef00, coef02, coef03);
		Vec4f fac1(coef04, coef04, coef06, coef07);
		Vec4f fac2(coef08, coef08, coef10, coef11);
		Vec4f fac3(coef12, coef12, coef14, coef15);
		Vec4f fac4(coef16, coef16, coef18, coef19);
		Vec4f fac5(coef20, coef20, coef22, coef23);

		Vec4f vec0(m.e10, m.e00, m.e00, m.e00);
		Vec4f vec1(m.e11, m.e01, m.e01, m.e01);
		Vec4f vec2(m.e12, m.e02, m.e02, m.e02);
		Vec4f vec3(m.e13, m.e03, m.e03, m.e03);

		Vec4f inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
		Vec4f inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
		Vec4f inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
		Vec4f inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

		Vec4f signA(+1.0f, -1.0f, +1.0f, -1.0f);
		Vec4f signB(-1.0f, +1.0f, -1.0f, +1.0f);

		Vec4f col0 = (inv0 * signA) * oneOverDeterminant;
		Vec4f col1 = (inv1 * signB) * oneOverDeterminant;
		Vec4f col2 = (inv2 * signA) * oneOverDeterminant;
		Vec4f col3 = (inv3 * signB) * oneOverDeterminant;

		return Mat4x4(
			col0.x, col0.y, col0.z, col0.w,
			col1.x, col1.y, col1.z, col1.w,
			col2.x, col2.y, col2.z, col2.w,
			col3.x, col3.y, col3.z, col3.w
		);
	}

	static inline Mat4x4 Translation(float x, float y, float z)
	{
		return Mat4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x, y, z, 1.0f
		);
	}

	static inline Mat4x4 Translation(const Vec3f& p)
	{
		return Mat4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			p.x, p.y, p.z, 1.0f
		);
	}

	static inline Mat4x4 Scale(float xScale, float yScale, float zScale)
	{
		return Mat4x4(
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, zScale, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	static inline Mat4x4 Scale(const Vec3f& scale)
	{
		return Mat4x4(
			scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	static inline Mat4x4 RotateX(float radian)
	{
		float c = GameMath::Cos(radian);
		float s = GameMath::Sin(radian);

		return Mat4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, c, s, 0.0f,
			0.0f, -s, c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	static inline Mat4x4 RotateY(float radian)
	{
		float c = GameMath::Cos(radian);
		float s = GameMath::Sin(radian);

		return Mat4x4(
			c, 0.0f, -s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			s, 0.0f, c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	static inline Mat4x4 RotateZ(float radian)
	{
		float c = GameMath::Cos(radian);
		float s = GameMath::Sin(radian);

		return Mat4x4(
			c, s, 0.0f, 0.0f,
			-s, c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	/** 로드리게스 회전 공식 https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula */
	static inline Mat4x4 Rotate(float radian, const Vec3f& axis)
	{
		float c = GameMath::Cos(radian);
		float s = GameMath::Sin(radian);
		Vec3f r = Vec3f::Normalize(axis);

		return Mat4x4(
			c + r.x * r.x * (1.0f - c), r.y * r.x * (1.0f - c) + r.z * s, r.z * r.x * (1.0f - c) - r.y * s, 0.0f,
			r.x * r.y * (1.0f - c) - r.z * s, c + r.y * r.y * (1.0f - c), r.z * r.y * (1.0f - c) + r.x * s, 0.0f,
			r.x * r.z * (1.0f - c) + r.y * s, r.y * r.z * (1.0f - c) - r.x * s, c + r.z * r.z * (1.0f - c), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	static inline Mat4x4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float width = (right - left);
		float sumLR = (right + left);
		float height = (top - bottom);
		float sumTB = (top + bottom);
		float depth = (zFar - zNear);
		float sumNF = (zFar + zNear);

		return Mat4x4(
			2.0f / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / height, 0.0f, 0.0f,
			0.0f, 0.0f, -2.0f / depth, 0.0f,
			-sumLR / width, -sumTB / height, -sumNF / depth, 1.0f
		);
	}

	static inline Mat4x4 Perspective(float fov, float aspect, float nearZ, float farZ)
	{
		float halfFov = fov / 2.0f;
		float tanHalfFovy = GameMath::Sin(halfFov) / GameMath::Cos(halfFov);

		return Mat4x4(
			1.0f / (aspect * tanHalfFovy), 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f / (tanHalfFovy), 0.0f, 0.0f,
			0.0f, 0.0f, -(farZ + nearZ) / (farZ - nearZ), -1.0f,
			0.0f, 0.0f, -(2.0f * farZ * nearZ) / (farZ - nearZ), 0.0f
		);
	}

	static inline Mat4x4 LookAt(const Vec3f& eyePosition, const Vec3f& focusPosition, const Vec3f& upDirection)
	{
		Vec3f f = -Vec3f::Normalize(focusPosition - eyePosition);
		Vec3f s = Vec3f::Normalize(Vec3f::Cross(upDirection, f));
		Vec3f u = Vec3f::Cross(f, s);
		Vec3f t = Vec3f(-Vec3f::Dot(s, eyePosition), -Vec3f::Dot(u, eyePosition), -Vec3f::Dot(f, eyePosition));

		return Mat4x4(
			s.x, u.x, f.x, 0.0f,
			s.y, u.y, f.y, 0.0f,
			s.z, u.z, f.z, 0.0f,
			t.x, t.y, t.z, 1.0f
		);
	}

	union
	{
		struct
		{
			float e00; float e01; float e02; float e03;
			float e10; float e11; float e12; float e13;
			float e20; float e21; float e22; float e23;
			float e30; float e31; float e32; float e33;
		};
		float data[16];
	};
};

static inline Vec2f operator*(const Vec2f& v, const Mat2x2& m)
{
	return Vec2f(
		v.x * m.e00 + v.y * m.e10,
		v.x * m.e01 + v.y * m.e11
	);
}

static inline Vec2f operator*(const Mat2x2& m, const Vec2f& v)
{
	return Vec2f(
		m.e00 * v.x + m.e01 * v.y,
		m.e10 * v.x + m.e11 * v.y
	);
}

static inline Vec3f operator*(const Vec3f& v, const Mat3x3& m)
{
	return Vec3f(
		v.x * m.e00 + v.y * m.e10 + v.z * m.e20,
		v.x * m.e01 + v.y * m.e11 + v.z * m.e21,
		v.x * m.e02 + v.y * m.e12 + v.z * m.e22
	);
}

static inline Vec3f operator*(const Mat3x3& m, const Vec3f& v)
{
	return Vec3f(
		m.e00 * v.x + m.e01 * v.y + m.e02 * v.z,
		m.e10 * v.x + m.e11 * v.y + m.e12 * v.z,
		m.e20 * v.x + m.e21 * v.y + m.e22 * v.z
	);
}

static inline Vec4f operator*(const Vec4f& v, const Mat4x4& m)
{
	return Vec4f(
		v.x * m.e00 + v.y * m.e10 + v.z * m.e20 + v.w * m.e30,
		v.x * m.e01 + v.y * m.e11 + v.z * m.e21 + v.w * m.e31,
		v.x * m.e02 + v.y * m.e12 + v.z * m.e22 + v.w * m.e32,
		v.x * m.e03 + v.y * m.e13 + v.z * m.e23 + v.w * m.e33
	);
}

static inline Vec4f operator*(const Mat4x4& m, const Vec4f& v)
{
	return Vec4f(
		m.e00 * v.x + m.e01 * v.y + m.e02 * v.z + m.e03 * v.w,
		m.e10 * v.x + m.e11 * v.y + m.e12 * v.z + m.e13 * v.w,
		m.e20 * v.x + m.e21 * v.y + m.e22 * v.z + m.e23 * v.w,
		m.e30 * v.x + m.e31 * v.y + m.e32 * v.z + m.e33 * v.w
	);
}

/** https://gabormakesgames.com/quaternions.html */
struct Quat
{
	Quat() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quat(float&& xx, float&& yy, float&& zz, float&& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}
	Quat(const float& xx, const float& yy, const float& zz, const float& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}
	Quat(Quat&& q) noexcept : x(q.x), y(q.y), z(q.z), w(q.w) {}
	Quat(const Quat& q) noexcept : x(q.x), y(q.y), z(q.z), w(q.w) {}

	Quat& operator=(Quat&& q) noexcept
	{
		if (this == &q) return *this;

		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;

		return *this;
	}

	Quat& operator=(const Quat& q) noexcept
	{
		if (this == &q) return *this;

		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;

		return *this;
	}

	Quat operator-() const
	{
		return Quat(-x, -y, -z, -w);
	}

	Quat operator+(Quat&& q) const
	{
		return Quat(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	Quat operator+(const Quat& q) const
	{
		return Quat(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	Quat operator-(Quat&& q) const
	{
		return Quat(x - q.x, y - q.y, z - q.z, w - q.w);
	}

	Quat operator-(const Quat& q) const
	{
		return Quat(x - q.x, y - q.y, z - q.z, w - q.w);
	}

	Quat operator*(float s) const
	{
		return Quat(x * s, y * s, z * s, w * s);
	}

	Quat operator*(Quat&& q) const
	{
		return Quat(
			+q.x * w + q.y * z - q.z * y + q.w * x,
			-q.x * z + q.y * w + q.z * x + q.w * y,
			+q.x * y - q.y * x + q.z * w + q.w * z,
			-q.x * x - q.y * y - q.z * z + q.w * w
		);
	}

	Quat operator*(const Quat& q) const
	{
		return Quat(
			+q.x * w + q.y * z - q.z * y + q.w * x,
			-q.x * z + q.y * w + q.z * x + q.w * y,
			+q.x * y - q.y * x + q.z * w + q.w * z,
			-q.x * x - q.y * y - q.z * z + q.w * w
		);
	}

	Vec3f operator*(Vec3f&& vec) const
	{
		return v * 2.0f * Vec3f::Dot(v, vec) + vec * (w * w - Vec3f::Dot(v, v)) + Vec3f::Cross(v, vec) * 2.0f * w;
	}

	Vec3f operator*(const Vec3f& vec) const
	{
		return v * 2.0f * Vec3f::Dot(v, vec) + vec * (w * w - Vec3f::Dot(v, v)) + Vec3f::Cross(v, vec) * 2.0f * w;
	}

	Quat& operator+=(Quat&& q) noexcept
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}

	Quat& operator+=(const Quat& q) noexcept
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}

	Quat& operator-=(Quat&& q) noexcept
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
	}

	Quat& operator-=(const Quat& q) noexcept
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
	}

	Quat& operator*=(float s) noexcept
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;

		return *this;
	}

	bool operator==(Quat&& q) const
	{
		return GameMath::Abs(x - q.x) <= EPSILON && GameMath::Abs(y - q.y) <= EPSILON && GameMath::Abs(z - q.z) <= EPSILON && GameMath::Abs(w - q.w) <= EPSILON;
	}

	bool operator==(const Quat& q) const
	{
		return GameMath::Abs(x - q.x) <= EPSILON && GameMath::Abs(y - q.y) <= EPSILON && GameMath::Abs(z - q.z) <= EPSILON && GameMath::Abs(w - q.w) <= EPSILON;
	}

	bool operator!=(Quat&& q) const
	{
		return GameMath::Abs(x - q.x) > EPSILON || GameMath::Abs(y - q.y) > EPSILON || GameMath::Abs(z - q.z) > EPSILON || GameMath::Abs(w - q.w) > EPSILON;
	}

	bool operator!=(const Quat& q) const
	{
		return GameMath::Abs(x - q.x) > EPSILON || GameMath::Abs(y - q.y) > EPSILON || GameMath::Abs(z - q.z) > EPSILON || GameMath::Abs(w - q.w) > EPSILON;
	}

	static inline Quat AxisRadian(const Vec3f& axis, float radian)
	{
		float s = GameMath::Sin(radian * 0.5f);
		float c = GameMath::Cos(radian * 0.5f);
		Vec3f norm = Vec3f::Normalize(axis);

		return Quat(norm.x * s, norm.y * s, norm.z * s, c);
	}

	static inline Quat AxisAngle(const Vec3f& axis, float angle)
	{
		float radian = GameMath::ToRadian(angle);

		float s = GameMath::Sin(radian * 0.5f);
		float c = GameMath::Cos(radian * 0.5f);
		Vec3f norm = Vec3f::Normalize(axis);

		return Quat(norm.x * s, norm.y * s, norm.z * s, c);
	}

	static inline Vec3f Axis(const Quat& q)
	{
		return Vec3f::Normalize(Vec3f(q.x, q.y, q.z));
	}

	static inline float Radian(const Quat& q)
	{
		return 2.0f * GameMath::ACos(q.w);
	}

	static inline float Angle(const Quat& q)
	{
		return GameMath::ToDegree(2.0f * GameMath::ACos(q.w));
	}

	static inline float Dot(const Quat& lhs, const Quat& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	static inline float LengthSq(const Quat& q)
	{
		return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	}

	static inline float Length(const Quat& q)
	{
		float lengthSq = Quat::LengthSq(q);
		return GameMath::Sqrt(lengthSq);
	}

	static inline Quat Normalize(const Quat& q)
	{
		float length = Quat::Length(q);

		if (GameMath::NearZero(length))
		{
			return Quat();
		}

		float invLength = 1.0f / length;
		return Quat(q.x * invLength, q.y * invLength, q.z * invLength, q.w * invLength);
	}

	static inline Quat Conjugate(const Quat& q)
	{
		return Quat(-q.x, -q.y, -q.z, q.w);
	}

	static inline Quat Inverse(const Quat& q)
	{
		float invLengthSq = 1.0f / Quat::LengthSq(q);
		return Quat(-q.x * invLengthSq, -q.y * invLengthSq, -q.z * invLengthSq, q.w * invLengthSq);
	}

	static inline Quat Zero()
	{
		return Quat(0.0f, 0.0f, 0.0f, 0.0f);
	}

	static inline Quat Identity()
	{
		return Quat(0.0f, 0.0f, 0.0f, 1.0f);
	}

	static inline Quat Rotate(const Vec3f& s, const Vec3f& e)
	{
		Vec3f start = Vec3f::Normalize(s);
		Vec3f end = Vec3f::Normalize(e);
		Quat quat;

		if (start == end)
		{
			return Quat();
		}

		if (start == -end)
		{
			Vec3f ortho;

			if (GameMath::Abs(start.y) < GameMath::Abs(start.x))
			{
				ortho = Vec3f(0.0f, 1.0f, 0.0f);
			}
			else if (GameMath::Abs(start.z) < GameMath::Abs(start.y) && GameMath::Abs(start.z) < GameMath::Abs(start.x))
			{
				ortho = Vec3f(0.0f, 0.0f, 1.0f);
			}
			else
			{
				ortho = Vec3f(1.0f, 0.0f, 0.0f);
			}

			Vec3f axis = Vec3f::Normalize(Vec3f::Cross(start, ortho));
			return AxisRadian(axis, 0.0f);
		}

		Vec3f half = Vec3f::Normalize(start + end);
		Vec3f axis = Vec3f::Cross(start, half);
		return Quat(axis.x, axis.y, axis.z, Vec3f::Dot(start, half));
	}

	static inline Quat Lerp(const Quat& s, const Quat& e, const float& t)
	{
		return s * (1.0f - t) + e * t;
	}

	static inline Quat Nlerp(const Quat& s, const Quat& e, const float& t)
	{
		return Normalize(Lerp(s, e, t));
	}

	static inline Quat Slerp(const Quat& s, const Quat& e, const float& t)
	{
		return Pow(Inverse(s) * e, t) * s;
	}

	static inline Quat Nslerp(const Quat& s, const Quat& e, const float& t)
	{
		return Normalize(Slerp(s, e, t));
	}

	static inline Quat Pow(const Quat& q, const float power)
	{
		float radian = Quat::Radian(q);
		Vec3f axis = Quat::Axis(q);

		float c = GameMath::Cos(power * radian * 0.5f);
		float s = GameMath::Sin(power * radian * 0.5f);

		return Quat(axis.x * s, axis.y * s, axis.z * s, c);
	}

	static inline Quat LookRotate(const Vec3f& direction, const Vec3f& up)
	{
		Vec3f f = Vec3f::Normalize(direction);
		Vec3f u = Vec3f::Normalize(up);
		Vec3f r = Vec3f::Cross(u, f);

		u = Vec3f::Cross(f, r);

		Quat f2d = Quat::Rotate(Vec3f(0.0f, 0.0f, 1.0f), f);
		Vec3f objectUp = f2d * Vec3f(0.0f, 1.0f, 0.0f);
		Quat u2u = Quat::Rotate(objectUp, u);

		return Quat::Normalize(f2d * u2u);
	}

	static inline Mat4x4 ToMat(const Quat& q)
	{
		Vec3f r = q * Vec3f(1.0f, 0.0f, 0.0f);
		Vec3f u = q * Vec3f(0.0f, 1.0f, 0.0f);
		Vec3f f = q * Vec3f(0.0f, 0.0f, 1.0f);

		return Mat4x4(
			r.x, r.y, r.z, 0.0f,
			u.x, u.y, u.z, 0.0f,
			f.x, f.y, f.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	static inline Quat ToQuat(const Mat4x4& m)
	{
		Vec3f u = Vec3f::Normalize(Vec3f(m.e10, m.e11, m.e12));
		Vec3f f = Vec3f::Normalize(Vec3f(m.e20, m.e21, m.e22));
		Vec3f r = Vec3f::Cross(u, f);
		u = Vec3f::Cross(f, r);

		return LookRotate(f, u);
	}

	bool IsSameOrientation(const Quat& lhs, const Quat& rhs)
	{
		return (GameMath::Abs(lhs.x - rhs.x) <= EPSILON && GameMath::Abs(lhs.y - rhs.y) <= EPSILON && GameMath::Abs(lhs.z - rhs.z) <= EPSILON && GameMath::Abs(lhs.w - rhs.w) <= EPSILON)
			|| (GameMath::Abs(lhs.x + rhs.x) <= EPSILON && GameMath::Abs(lhs.y + rhs.y) <= EPSILON && GameMath::Abs(lhs.z + rhs.z) <= EPSILON && GameMath::Abs(lhs.w + rhs.w) <= EPSILON);
	}

	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			Vec3f v;
			float w;
		};
		float data[4];
	};
};

struct Transform
{
	Transform() noexcept
		: position(0.0f, 0.0f, 0.0f)
		, rotate(0.0f, 0.0f, 0.0f, 1.0f)
		, scale(1.0f, 1.0f, 1.0f) {}

	Transform(Vec3f&& p, Quat&& r, Vec3f&& s) noexcept
		: position(p)
		, rotate(r)
		, scale(s) {}

	Transform(const Vec3f& p, const Quat& r, const Vec3f& s) noexcept
		: position(p)
		, rotate(r)
		, scale(s) {}

	Transform(Transform&& instance) noexcept
		: position(instance.position)
		, rotate(instance.rotate)
		, scale(instance.scale) {}

	Transform(const Transform& instance) noexcept
		: position(instance.position)
		, rotate(instance.rotate)
		, scale(instance.scale) {}

	Transform& operator=(Transform&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		rotate = instance.rotate;
		scale = instance.scale;

		return *this;
	}

	Transform& operator=(const Transform& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		rotate = instance.rotate;
		scale = instance.scale;

		return *this;
	}

	bool operator==(Transform&& transform) noexcept
	{
		return position == transform.position && rotate == transform.rotate && scale == transform.scale;
	}

	bool operator==(const Transform& transform) noexcept
	{
		return position == transform.position && rotate == transform.rotate && scale == transform.scale;
	}

	bool operator!=(Transform&& transform) noexcept
	{
		return position != transform.position || rotate != transform.rotate || scale != transform.scale;
	}

	bool operator!=(const Transform& transform) noexcept
	{
		return position != transform.position || rotate != transform.rotate || scale != transform.scale;
	}

	static inline Mat4x4 ToMat(const Transform& transform)
	{
		Vec3f x = transform.rotate * Vec3f(1.0f, 0.0f, 0.0f);
		Vec3f y = transform.rotate * Vec3f(0.0f, 1.0f, 0.0f);
		Vec3f z = transform.rotate * Vec3f(0.0f, 0.0f, 1.0f);

		x = x * transform.scale.x;
		y = y * transform.scale.y;
		z = z * transform.scale.z;

		Vec3f p = transform.position;

		return Mat4x4(
			x.x, x.y, x.z, 0.0f,
			y.x, y.y, y.z, 0.0f,
			z.x, z.y, z.z, 0.0f,
			p.x, p.y, p.z, 1.0f
		);
	}

	static inline Transform ToTransform(const Mat4x4& m)
	{
		Transform t;

		t.position = Vec3f(m.e30, m.e31, m.e32);
		t.rotate = Quat::ToQuat(m);

		Mat4x4 rotateScale(
			m.e00, m.e01, m.e02, 0.0f,
			m.e10, m.e11, m.e12, 0.0f,
			m.e20, m.e21, m.e22, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		Mat4x4 invRotateScale = Quat::ToMat(Quat::Inverse(t.rotate));
		Mat4x4 scaleKew = invRotateScale * rotateScale;

		t.scale = Vec3f(scaleKew.e00, scaleKew.e11, scaleKew.e22);

		return t;
	}

	static inline Transform Combine(const Transform& lhs, const Transform& rhs)
	{
		Transform t;

		t.scale = lhs.scale * rhs.scale;
		t.rotate = rhs.rotate * lhs.rotate;

		t.position = lhs.rotate * (lhs.scale * rhs.position);
		t.position = lhs.position + t.position;

		return t;
	}

	static inline Transform Inverse(const Transform& transform)
	{
		Transform inv;

		inv.rotate = Quat::Inverse(transform.rotate);

		inv.scale.x = GameMath::Abs(transform.scale.x) < EPSILON ? 0.0f : 1.0f / transform.scale.x;
		inv.scale.y = GameMath::Abs(transform.scale.y) < EPSILON ? 0.0f : 1.0f / transform.scale.y;
		inv.scale.z = GameMath::Abs(transform.scale.z) < EPSILON ? 0.0f : 1.0f / transform.scale.z;

		Vec3f invTranslation = -transform.position;
		inv.position = inv.rotate * (inv.scale * invTranslation);

		return inv;
	}

	static inline Transform Mix(const Transform& s, const Transform& e, const float& t)
	{
		Quat eRotate = e.rotate;

		if (Quat::Dot(s.rotate, eRotate) < 0.0f)
		{
			eRotate = -eRotate;
		}

		return Transform(
			Vec3f::Lerp(s.position, e.position, t),
			Quat::Nlerp(s.rotate, eRotate, t),
			Vec3f::Lerp(s.scale, e.scale, t)
		);
	}

	Vec3f position;
	Quat rotate;
	Vec3f scale;
};