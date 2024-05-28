#pragma once

#include "Vec.h"


/**
 * @brief 3D 위치 정보를 가진 정점입니다.
 */
struct VertexPosition3D
{
	/**
	 * @brief 3D 위치 정보를 가진 정점의 기본 생성자입니다.
	 */
	VertexPosition3D() noexcept : position(0.0f, 0.0f, 0.0f) {}


	/**
	 * @brief 3D 위치 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 */
	VertexPosition3D(Vec3f&& p) noexcept : position(p) {}


	/**
	 * @brief 3D 위치 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 */
	VertexPosition3D(const Vec3f& p) noexcept : position(p) {}


	/**
	 * @brief 3D 위치 정보를 가진 정점의 생성자입니다.
	 *
	 * @param x 위치의 X좌표입니다.
	 * @param y 위치의 Y좌표입니다.
	 * @param z 위치의 Z좌표입니다.
	 */
	VertexPosition3D(float x, float y, float z) noexcept : position(x, y, z) {}


	/**
	 * @brief 3D 위치 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPosition3D(VertexPosition3D&& instance) noexcept
		: position(instance.position) {}


	/**
	 * @brief 3D 위치 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPosition3D(const VertexPosition3D& instance) noexcept
		: position(instance.position) {}


	/**
	 * @brief 3D 위치 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPosition3D& operator=(VertexPosition3D&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;

		return *this;
	}


	/**
	 * @brief 3D 위치 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPosition3D& operator=(const VertexPosition3D& instance) noexcept
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
		return sizeof(VertexPosition3D);
	}


	/**
	 * @brief 정점의 위치입니다.
	 */
	Vec3f position;
};


/**
 * @brief 3D 위치와 색상 정보를 가진 정점입니다.
 */
struct VertexPositionColor3D
{
	/**
	 * @brief 3D 위치와 색상 정보를 가진 정점의 기본 생성자입니다.
	 */
	VertexPositionColor3D() noexcept
		: position(0.0f, 0.0f, 0.0f)
		, color(0.0f, 0.0f, 0.0f, 0.0f) {}


	/**
	 * @brief 3D 위치와 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param c 정점의 색상입니다.
	 */
	VertexPositionColor3D(Vec3f&& p, Vec4f&& c) noexcept
		: position(p)
		, color(c) {}


	/**
	 * @brief 3D 위치와 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param c 정점의 색상입니다.
	 */
	VertexPositionColor3D(const Vec3f& p, const Vec4f& c) noexcept
		: position(p)
		, color(c) {}


	/**
	 * @brief 3D 위치와 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param x 위치의 X좌표입니다.
	 * @param y 위치의 Y좌표입니다.
	 * @param z 위치의 Z좌표입니다.
	 * @param r 정점 색상의 R입니다.
	 * @param g 정점 색상의 G입니다.
	 * @param b 정점 색상의 B입니다.
	 * @param a 정점 색상의 A입니다.
	 */
	VertexPositionColor3D(
		float x, float y, float z,
		float r, float g, float b, float a
	) noexcept
		: position(x, y, z)
		, color(r, g, b, a) {}


	/**
	 * @brief 3D 위치와 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPositionColor3D(VertexPositionColor3D&& instance) noexcept
		: position(instance.position)
		, color(instance.color) {}


	/**
	 * @brief 3D 위치와 색상 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPositionColor3D(const VertexPositionColor3D& instance) noexcept
		: position(instance.position)
		, color(instance.color) {}


	/**
	 * @brief 3D 위치와 색상 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPositionColor3D& operator=(VertexPositionColor3D&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		color = instance.color;

		return *this;
	}


	/**
	 * @brief 3D 위치와 색상 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPositionColor3D& operator=(const VertexPositionColor3D& instance) noexcept
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
		return sizeof(VertexPositionColor3D);
	}


	/**
	 * @brief 정점의 위치입니다.
	 */
	Vec3f position;


	/**
	 * @brief 정점의 색상입니다.
	 */
	Vec4f color;
};



/**
 * @brief 3D 위치와 텍스처 좌표 정보를 가진 정점입니다.
 */
struct VertexPositionUv3D
{
	/**
	 * @brief 3D 위치와 텍스처 좌표 정보를 가진 정점의 기본 생성자입니다.
	 */
	VertexPositionUv3D() noexcept
		: position(0.0f, 0.0f, 0.0f)
		, uv(0.0f, 0.0f) {}


	/**
	 * @brief 3D 위치와 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param tex 정점의 색상입니다.
	 */
	VertexPositionUv3D(Vec3f&& p, Vec2f&& tex) noexcept
		: position(p)
		, uv(tex) {}


	/**
	 * @brief 3D 위치와 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param tex 정점의 색상입니다.
	 */
	VertexPositionUv3D(const Vec3f& p, const Vec2f& tex) noexcept
		: position(p)
		, uv(tex) {}


	/**
	 * @brief 3D 위치와 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param x 위치의 X좌표입니다.
	 * @param y 위치의 Y좌표입니다.
	 * @param z 위치의 Z좌표입니다.
	 * @param u 텍스처 위치의 U좌표입니다.
	 * @param v 텍스처 위치의 V좌표입니다.
	 */
	VertexPositionUv3D(
		float x, float y, float z,
		float u, float v
	) noexcept
		: position(x, y, z)
		, uv(u, v) {}


	/**
	 * @brief 3D 위치와 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPositionUv3D(VertexPositionUv3D&& instance) noexcept
		: position(instance.position)
		, uv(instance.uv) {}


	/**
	 * @brief 3D 위치와 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param instance 복사할 정점 인스턴스입니다.
	 */
	VertexPositionUv3D(const VertexPositionUv3D& instance) noexcept
		: position(instance.position)
		, uv(instance.uv) {}


	/**
	 * @brief 3D 위치와 텍스처 좌표 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPositionUv3D& operator=(VertexPositionUv3D&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		uv = instance.uv;

		return *this;
	}


	/**
	 * @brief 3D 위치와 텍스처 좌표 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 대입할 정점 인스턴스입니다.
	 *
	 * @return 대입한 객체의 참조자를 반환합니다.
	 */
	VertexPositionUv3D& operator=(const VertexPositionUv3D& instance) noexcept
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
		return sizeof(VertexPositionUv3D);
	}


	/**
	 * @brief 정점의 위치입니다.
	 */
	Vec3f position;


	/**
	 * @brief 정점의 색상입니다.
	 */
	Vec2f uv;
};



/**
 * @brief 3D 위치, 법선 정보를 가진 정점입니다.
 */
struct VertexPositionNormal3D
{
	/**
	 * @brief 3D 위치, 법선 정보를 가진 정점의 디폴트 생성자입니다.
	 */
	VertexPositionNormal3D() noexcept
		: position(0.0f, 0.0f, 0.0f)
		, normal(0.0f, 0.0f, 0.0f) {}


	/**
	 * @brief 3D 위치, 법선 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param n 정점의 법선 벡터입니다.
	 */
	VertexPositionNormal3D(Vec3f&& p, Vec3f&& n) noexcept
		: position(p)
		, normal(n) {}


	/**
	 * @brief 3D 위치, 법선 정보를 가진 정점의 생성자입니다.
	 *
	 * @param position 정점의 위치입니다.
	 * @param normal 정점의 법선 벡터입니다.
	 * @param uv 정점의 텍스처 좌표입니다.
	 */
	VertexPositionNormal3D(const Vec3f& p, const Vec3f& n) noexcept
		: position(p)
		, normal(n) {}


	/**
	 * @brief 3D 위치, 법선 정보를 가진 정점의 생성자입니다.
	 *
	 * @param px 정점 위치의 X좌표입니다.
	 * @param py 정점 위치의 Y좌표입니다.
	 * @param pz 정점 위치의 Z좌표입니다.
	 * @param nx 정점 법선 벡터의 X성분입니다.
	 * @param ny 정점 법선 벡터의 Y성분입니다.
	 * @param nz 정점 법선 벡터의 Z성분입니다.
	 */
	VertexPositionNormal3D(
		float px, float py, float pz,
		float nx, float ny, float nz
	) noexcept
		: position(px, py, pz)
		, normal(nx, ny, nz) {}


	/**
	 * @brief 3D 위치, 법선 정보를 가진 정점의 복사 생성자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선 정보를 가진 정점 인스턴스입니다.
	 */
	VertexPositionNormal3D(VertexPositionNormal3D&& instance) noexcept
		: position(instance.position)
		, normal(instance.normal) {}


	/**
	 * @brief 3D 위치, 법선 정보를 가진 정점의 복사 생성자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선 정보를 가진 정점 인스턴스입니다.
	 */
	VertexPositionNormal3D(const VertexPositionNormal3D& instance) noexcept
		: position(instance.position)
		, normal(instance.normal) {}


	/**
	 * @brief 3D 위치, 법선 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선 정보를 가진 정점 인스턴스입니다.
	 *
	 * @return 대인한 객체의 참조자를 반환합니다.
	 */
	VertexPositionNormal3D& operator=(VertexPositionNormal3D&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		normal = instance.normal;

		return *this;
	}


	/**
	 * @brief 3D 위치, 법선 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선 정보를 가진 정점 인스턴스입니다.
	 *
	 * @return 대인한 객체의 참조자를 반환합니다.
	 */
	VertexPositionNormal3D& operator=(const VertexPositionNormal3D& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		normal = instance.normal;

		return *this;
	}


	/**
	 * @brief 정점의 바이트 보폭 값을 얻습니다.
	 *
	 * @return 정점의 파이트 보폭(Stride) 값을 반환합니다.
	 */
	static uint32_t GetStride()
	{
		return sizeof(VertexPositionNormal3D);
	}


	/**
	 * @brief 정점의 위치입니다.
	 */
	Vec3f position;


	/**
	 * @brief 정점의 법선 벡터입니다.
	 */
	Vec3f normal;
};


/**
 * @brief 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점입니다.
 */
struct VertexPositionNormalUv3D
{
	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점의 디폴트 생성자입니다.
	 */
	VertexPositionNormalUv3D() noexcept
		: position(0.0f, 0.0f, 0.0f)
		, normal(0.0f, 0.0f, 0.0f)
		, uv(0.0f, 0.0f) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param n 정점의 법선 벡터입니다.
	 * @param tex 정점의 텍스처 좌표입니다.
	 */
	VertexPositionNormalUv3D(Vec3f&& p, Vec3f&& n, Vec2f&& tex) noexcept
		: position(p)
		, normal(n)
		, uv(tex) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param position 정점의 위치입니다.
	 * @param normal 정점의 법선 벡터입니다.
	 * @param uv 정점의 텍스처 좌표입니다.
	 */
	VertexPositionNormalUv3D(const Vec3f& p, const Vec3f& n, const Vec2f& tex) noexcept
		: position(p)
		, normal(n)
		, uv(tex) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점의 생성자입니다.
	 *
	 * @param px 정점 위치의 X좌표입니다.
	 * @param py 정점 위치의 Y좌표입니다.
	 * @param pz 정점 위치의 Z좌표입니다.
	 * @param nx 정점 법선 벡터의 X성분입니다.
	 * @param ny 정점 법선 벡터의 Y성분입니다.
	 * @param nz 정점 법선 벡터의 Z성분입니다.
	 * @param u 정점 텍스처 좌표의 U성분입니다.
	 * @param v 정점 텍스처 좌표의 V성분입니다.
	 */
	VertexPositionNormalUv3D(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float u, float v
	) noexcept
		: position(px, py, pz)
		, normal(nx, ny, nz)
		, uv(u, v) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점의 복사 생성자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점 인스턴스입니다.
	 */
	VertexPositionNormalUv3D(VertexPositionNormalUv3D&& instance) noexcept
		: position(instance.position)
		, normal(instance.normal)
		, uv(instance.uv) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점의 복사 생성자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점 인스턴스입니다.
	 */
	VertexPositionNormalUv3D(const VertexPositionNormalUv3D& instance) noexcept
		: position(instance.position)
		, normal(instance.normal)
		, uv(instance.uv) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점 인스턴스입니다.
	 *
	 * @return 대인한 객체의 참조자를 반환합니다.
	 */
	VertexPositionNormalUv3D& operator=(VertexPositionNormalUv3D&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		normal = instance.normal;
		uv = instance.uv;

		return *this;
	}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선, 텍스처 좌표 정보를 가진 정점 인스턴스입니다.
	 *
	 * @return 대인한 객체의 참조자를 반환합니다.
	 */
	VertexPositionNormalUv3D& operator=(const VertexPositionNormalUv3D& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		normal = instance.normal;
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
		return sizeof(VertexPositionNormalUv3D);
	}


	/**
	 * @brief 정점의 위치입니다.
	 */
	Vec3f position;


	/**
	 * @brief 정점의 법선 벡터입니다.
	 */
	Vec3f normal;


	/**
	 * @brief 정점의 텍스처 좌표입니다.
	 */
	Vec2f uv;
};


/**
 * @brief 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점입니다.
 */
struct VertexPositionNormalUvSkin3D
{
	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점의 디폴트 생성자입니다.
	 */
	VertexPositionNormalUvSkin3D() noexcept
		: position(0.0f, 0.0f, 0.0f)
		, normal(0.0f, 0.0f, 0.0f)
		, uv(0.0f, 0.0f)
		, weight(0.0f, 0.0f, 0.0f, 0.0f)
		, joints(0, 0, 0, 0) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param n 정점의 법선 벡터입니다.
	 * @param tex 정점의 텍스처 좌표입니다.
	 * @param w 정점의 가중치입니다.
	 * @param j 정점의 뼈대 인덱스입니다.
	 */
	VertexPositionNormalUvSkin3D(Vec3f&& p, Vec3f&& n, Vec2f&& tex, Vec4f&& w, Vec4i&& j) noexcept
		: position(p)
		, normal(n)
		, uv(tex)
		, weight(w)
		, joints(j) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점의 생성자입니다.
	 *
	 * @param p 정점의 위치입니다.
	 * @param n 정점의 법선 벡터입니다.
	 * @param tex 정점의 텍스처 좌표입니다.
	 * @param w 정점의 가중치입니다.
	 * @param j 정점의 뼈대 인덱스입니다.
	 */
	VertexPositionNormalUvSkin3D(const Vec3f& p, const Vec3f& n, const Vec2f& tex, const Vec4f& w, const Vec4i& j) noexcept
		: position(p)
		, normal(n)
		, uv(tex)
		, weight(w)
		, joints(j) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점의 생성자입니다.
	 *
	 * @param px 정점 위치의 X좌표입니다.
	 * @param py 정점 위치의 Y좌표입니다.
	 * @param pz 정점 위치의 Z좌표입니다.
	 * @param nx 정점 법선 벡터의 X성분입니다.
	 * @param ny 정점 법선 벡터의 Y성분입니다.
	 * @param nz 정점 법선 벡터의 Z성분입니다.
	 * @param u 정점 텍스처 좌표의 U성분입니다.
	 * @param v 정점 텍스처 좌표의 V성분입니다.
	 * @param wx 정점 가중치의 X성분입니다.
	 * @param wy 정점 가중치의 Y성분입니다.
	 * @param wz 정점 가중치의 Z성분입니다.
	 * @param ww 정점 가중치의 W성분입니다.
	 * @param jx 정점 뼈대 인덱스의 X성분입니다.
	 * @param jy 정점 뼈대 인덱스의 Y성분입니다.
	 * @param jz 정점 뼈대 인덱스의 Z성분입니다.
	 * @param jw 정점 뼈대 인덱스의 W성분입니다.
	 */
	VertexPositionNormalUvSkin3D(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float u, float v,
		float wx, float wy, float wz, float ww,
		int32_t jx, int32_t jy, int32_t jz, int32_t jw
	) noexcept
		: position(px, py, pz)
		, normal(nx, ny, nz)
		, uv(u, v)
		, weight(wx, wy, wz, ww)
		, joints(jx, jy, jz, jw) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점의 복사 생성자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점 인스턴스입니다.
	 */
	VertexPositionNormalUvSkin3D(VertexPositionNormalUvSkin3D&& instance) noexcept
		: position(instance.position)
		, normal(instance.normal)
		, uv(instance.uv)
		, weight(instance.weight)
		, joints(instance.joints) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점의 복사 생성자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점 인스턴스입니다.
	 */
	VertexPositionNormalUvSkin3D(const VertexPositionNormalUvSkin3D& instance) noexcept
		: position(instance.position)
		, normal(instance.normal)
		, uv(instance.uv)
		, weight(instance.weight)
		, joints(instance.joints) {}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점 인스턴스입니다.
	 *
	 * @return 대인한 객체의 참조자를 반환합니다.
	 */
	VertexPositionNormalUvSkin3D& operator=(VertexPositionNormalUvSkin3D&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		normal = instance.normal;
		uv = instance.uv;
		weight = instance.weight;
		joints = instance.joints;

		return *this;
	}


	/**
	 * @brief 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점의 대입 연산자입니다.
	 *
	 * @param instance 복사를 수행할 3D 위치, 법선, 텍스처 좌표, 스키닝 정보를 가진 정점 인스턴스입니다.
	 *
	 * @return 대인한 객체의 참조자를 반환합니다.
	 */
	VertexPositionNormalUvSkin3D& operator=(const VertexPositionNormalUvSkin3D& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		normal = instance.normal;
		uv = instance.uv;
		weight = instance.weight;
		joints = instance.joints;

		return *this;
	}


	/**
	 * @brief 정점의 바이트 보폭 값을 얻습니다.
	 *
	 * @return 정점의 파이트 보폭(Stride) 값을 반환합니다.
	 */
	static uint32_t GetStride()
	{
		return sizeof(VertexPositionNormalUvSkin3D);
	}


	/**
	 * @brief 정점의 위치입니다.
	 */
	Vec3f position;


	/**
	 * @brief 정점의 법선 벡터입니다.
	 */
	Vec3f normal;


	/**
	 * @brief 정점의 텍스처 좌표입니다.
	 */
	Vec2f uv;


	/**
	 * @brief 정점의 가중치입니다.
	 */
	Vec4f weight;


	/**
	 * @brief 정점의 뼈대 인덱스입니다.
	 */
	Vec4i joints;
};