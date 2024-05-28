#pragma once

#include "Vec.h"


/**
 * @brief 2D 위치 정보를 가진 정점입니다.
 */
struct VertexPosition2D
{
	/**
	 * @brief 2D 위치 정보를 가진 정점의 기본 생성자입니다.
	 */
	VertexPosition2D() noexcept : position(0.0f, 0.0f) {}


	/**
	 * @brief 2D 위치 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 */
	VertexPosition2D(Vec2f&& p) noexcept : position(p) {}


	/**
	 * @brief 2D 위치 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 */
	VertexPosition2D(const Vec2f& p) noexcept : position(p) {}


	/**
	 * @brief 2D 위치 정보를 가진 정점의 생성자입니다.
	 *
	 * @param x 위치의 X좌표입니다.
	 * @param y 위치의 Y좌표입니다.
	 */
	VertexPosition2D(float x, float y) noexcept : position(x, y) {}


	/**
	 * @brief 2D 위치 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPosition2D(VertexPosition2D&& instance) noexcept
		: position(instance.position) {}


	/**
	 * @brief 2D 위치 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPosition2D(const VertexPosition2D& instance) noexcept
		: position(instance.position) {}


	/**
	 * @brief 2D 위치 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPosition2D& operator=(VertexPosition2D&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;

		return *this;
	}


	/**
	 * @brief 2D 위치 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPosition2D& operator=(const VertexPosition2D& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;

		return *this;
	}


	/**
	 * @brief 정점의 바이트 보폭 값을 얻습니다.
	 *
	 * @return 정점의 파이트 보폭(Stride) 값을 반환합니다.
	 */
	static uint32_t GetStride()
	{
		return sizeof(VertexPosition2D);
	}


	/**
	 * @brief 정점의 위치입니다.
	 */
	Vec2f position;
};


/**
 * @brief 2D 위치와 색상 정보를 가진 정점입니다.
 */
struct VertexPositionColor2D
{
	/**
	 * @brief 2D 위치와 색상 정보를 가진 정점의 기본 생성자입니다.
	 */
	VertexPositionColor2D() noexcept
		: position(0.0f, 0.0f)
		, color(0.0f, 0.0f, 0.0f, 0.0f) {}


	/**
	 * @brief 2D 위치와 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param c 정점의 색상입니다.
	 */
	VertexPositionColor2D(Vec2f&& p, Vec4f&& c) noexcept
		: position(p)
		, color(c) {}


	/**
	 * @brief 2D 위치와 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param c 정점의 색상입니다.
	 */
	VertexPositionColor2D(const Vec2f& p, const Vec4f& c) noexcept
		: position(p)
		, color(c) {}


	/**
	 * @brief 2D 위치와 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param x 위치의 X좌표입니다.
	 * @param y 위치의 Y좌표입니다.
	 * @param r 정점 색상의 R입니다.
	 * @param g 정점 색상의 G입니다.
	 * @param b 정점 색상의 B입니다.
	 * @param a 정점 색상의 A입니다.
	 */
	VertexPositionColor2D(
		float x, float y,
		float r, float g, float b, float a
	) noexcept
		: position(x, y)
		, color(r, g, b, a) {}


	/**
	 * @brief 2D 위치와 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPositionColor2D(VertexPositionColor2D&& instance) noexcept
		: position(instance.position)
		, color(instance.color) {}


	/**
	 * @brief 2D 위치와 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPositionColor2D(const VertexPositionColor2D& instance) noexcept
		: position(instance.position)
		, color(instance.color) {}


	/**
	 * @brief 2D 위치와 색상 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPositionColor2D& operator=(VertexPositionColor2D&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		color = instance.color;

		return *this;
	}


	/**
	 * @brief 2D 위치와 색상 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPositionColor2D& operator=(const VertexPositionColor2D& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		color = instance.color;

		return *this;
	}


	/**
	 * @brief 정점의 바이트 보폭 값을 얻습니다.
	 *
	 * @return 정점의 파이트 보폭(Stride) 값을 반환합니다.
	 */
	static uint32_t GetStride()
	{
		return sizeof(VertexPositionColor2D);
	}


	/**
	 * @brief 정점의 위치입니다.
	 */
	Vec2f position;


	/**
	 * @brief 정점의 색상입니다.
	 */
	Vec4f color;
};


/**
 * @brief 2D 위치와 텍스처 좌표 정보를 가진 정점입니다.
 */
struct VertexPositionUv2D
{
	/**
	 * @brief 2D 위치와 텍스처 좌표 정보를 가진 정점의 기본 생성자입니다.
	 */
	VertexPositionUv2D() noexcept
		: position(0.0f, 0.0f)
		, uv(0.0f, 0.0f) {}


	/**
	 * @brief 2D 위치와 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param position 정점의 위치입니다.
	 * @param tex 정점의 텍스처 좌표입니다.
	 */
	VertexPositionUv2D(Vec2f&& p, Vec2f&& tex) noexcept
		: position(p)
		, uv(tex) {}


	/**
	 * @brief 2D 위치와 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param position 정점의 위치입니다.
	 * @param tex 정점의 텍스처 좌표입니다.
	 */
	VertexPositionUv2D(const Vec2f& p, const Vec2f& tex) noexcept
		: position(p)
		, uv(tex) {}


	/**
	 * @brief 2D 위치와 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param x 위치의 X좌표입니다.
	 * @param y 위치의 Y좌표입니다.
	 * @param u 텍스처 위치의 U좌표입니다.
	 * @param v 텍스처 위치의 V좌표입니다.
	 */
	VertexPositionUv2D(
		float x, float y,
		float u, float v
	) noexcept
		: position(x, y)
		, uv(u, v) {}


	/**
	 * @brief 2D 위치와 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPositionUv2D(VertexPositionUv2D&& instance) noexcept
		: position(instance.position)
		, uv(instance.uv) {}


	/**
	 * @brief 2D 위치와 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPositionUv2D(const VertexPositionUv2D& instance) noexcept
		: position(instance.position)
		, uv(instance.uv) {}


	/**
	 * @brief 2D 위치와 텍스처 좌표 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPositionUv2D& operator=(VertexPositionUv2D&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		uv = instance.uv;

		return *this;
	}


	/**
	 * @brief 2D 위치와 텍스처 좌표 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPositionUv2D& operator=(const VertexPositionUv2D& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		uv = instance.uv;

		return *this;
	}


	/**
	 * @brief 정점의 바이트 보폭 값을 얻습니다.
	 *
	 * @return 정점의 파이트 보폭(Stride) 값을 반환합니다.
	 */
	static uint32_t GetStride()
	{
		return sizeof(VertexPositionUv2D);
	}


	/**
	 * @brief 정점의 위치입니다.
	 */
	Vec2f position;


	/**
	 * @brief 정점의 텍스처 좌표입니다.
	 */
	Vec2f uv;
};


/**
 * @brief 2D 위치, 텍스처 좌표, 색상 정보를 가진 정점입니다.
 */
struct VertexPositionUvColor2D
{
	/**
	 * @brief 2D 위치, 텍스처 좌표, 색상 정보를 가진 정점의 기본 생성자입니다.
	 */
	VertexPositionUvColor2D() noexcept
		: position(0.0f, 0.0f)
		, uv(0.0f, 0.0f)
		, color(0.0f, 0.0f, 0.0f, 0.0f) {}


	/**
	 * @brief 2D 위치, 텍스처 좌표, 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param tex 정점의 텍스처 좌표입니다.
	 * @param c 정점의 색상입니다
	 */
	VertexPositionUvColor2D(Vec2f&& p, Vec2f&& tex, Vec4f&& c) noexcept
		: position(p)
		, uv(tex)
		, color(c) {}


	/**
	 * @brief 2D 위치, 텍스처 좌표, 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param tex 정점의 텍스처 좌표입니다.
	 * @param c 정점의 색상입니다
	 */
	VertexPositionUvColor2D(const Vec2f& p, const Vec2f& tex, const Vec4f& c) noexcept
		: position(p)
		, uv(tex)
		, color(c) {}


	/**
	 * @brief 2D 위치, 텍스처 좌표, 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param x 위치의 X좌표입니다.
	 * @param y 위치의 Y좌표입니다.
	 * @param u 텍스처 위치의 U좌표입니다.
	 * @param v 텍스처 위치의 V좌표입니다.
	 * @param r 정점 색상의 R입니다.
	 * @param g 정점 색상의 G입니다.
	 * @param b 정점 색상의 B입니다.
	 * @param a 정점 색상의 A입니다.
	 */
	VertexPositionUvColor2D(
		float x, float y,
		float u, float v,
		float r, float g, float b, float a
	) noexcept
		: position(x, y)
		, uv(u, v)
		, color(r, g, b, a) {}


	/**
	 * @brief 2D 위치, 텍스처 좌표, 색상 정보를 가진 정점의 복사 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPositionUvColor2D(VertexPositionUvColor2D&& instance) noexcept
		: position(instance.position)
		, uv(instance.uv)
		, color(instance.color) {}


	/**
	 * @brief 2D 위치, 텍스처 좌표, 색상 정보를 가진 정점의 복사 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPositionUvColor2D(const VertexPositionUvColor2D& instance) noexcept
		: position(instance.position)
		, uv(instance.uv)
		, color(instance.color) {}


	/**
	 * @brief 2D 위치, 텍스처 좌표, 색상 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPositionUvColor2D& operator=(VertexPositionUvColor2D&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		uv = instance.uv;
		color = instance.color;

		return *this;
	}


	/**
	 * @brief 2D 위치, 텍스처 좌표, 색상 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPositionUvColor2D& operator=(const VertexPositionUvColor2D& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		uv = instance.uv;
		color = instance.color;

		return *this;
	}


	/**
	 * @brief 정점의 바이트 보폭 값을 얻습니다.
	 *
	 * @return 정점의 파이트 보폭(Stride) 값을 반환합니다.
	 */
	static uint32_t GetStride()
	{
		return sizeof(VertexPositionUvColor2D);
	}


	/**
	 * @brief 정점의 위치입니다.
	 */
	Vec2f position;


	/**
	 * @brief 정점의 텍스처 좌표입니다.
	 */
	Vec2f uv;


	/**
	 * @brief 정점의 색상입니다.
	 */
	Vec4f color;
};