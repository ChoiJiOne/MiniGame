#pragma once

#include <array>

#include "RenderModule.h"
#include "VertexBuffer.h"

class Shader;
class TTFont;


/**
 * @brief 텍스트의 시작 위치 기준입니다.
 */
enum class EStartPivot
{
	LEFT_TOP     = 0x00,
	LEFT_BOTTOM  = 0x01,
	RIGHT_TOP    = 0x02,
	RIGHT_BOTTOM = 0x03,
	CENTER       = 0x04,
};


/**
 * @brief 글리프를 이용해서 텍스트를 렌더링하는 렌더러입니다.
 */
class TextRenderer : public IResource
{
public:
	/**
	 * @brief 텍스트를 그리는 렌더러의 디폴트 생성자입니다.
	 */
	explicit TextRenderer();


	/**
	 * @brief 텍스트를 렌더링하는 렌더러의 가상 소멸자입니다.
	 *
	 * @note 렌더러의 내부에서 할당된 요소를 해제하기 위해서는 반드시 Release를 호출해야 합니다.
	 */
	virtual ~TextRenderer();


	/**
	 * @brief 2D 텍스트를 렌더링하는 렌더러의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(TextRenderer);


	/**
	 * @brief 렌더러 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 직교 투영 행렬을 설정합니다.
	 *
	 * @param ortho 설정할 직교 투영 행렬입니다.
	 */
	void SetOrtho(const Mat4x4& ortho) { ortho_ = ortho; }


	/**
	 * @brief 2D 텍스트를 그립니다.
	 *
	 * @param font 폰트 리소스입니다.
	 * @param text 렌더링할 텍스트입니다.
	 * @param position 텍스트의 좌표입니다.
	 * @param startPivot 텍스트 시작 좌표의 기준입니다.
	 * @param color 텍스트의 RGBA 색상입니다.
	 */
	void DrawText2D(const TTFont* font, const std::wstring& text, const Vec2f& position, const EStartPivot& startPivot, Vec4f& color);


private:
	/**
	 * @brief 텍스트 렌더링을 수행하는 렌더러 내부에서만 사용하는 정점입니다.
	 */
	struct Vertex
	{
		/**
		 * @brief 정점의 기본 생성자입니다.
		 */
		Vertex() noexcept
			: position(0.0f, 0.0f)
			, uv(0.0f, 0.0f)
			, color(0.0f, 0.0f, 0.0f, 0.0f) {}


		/**
		 * @brief 정점의 생성자입니다.
		 *
		 * @param p 정점의 위치입니다.
		 * @param tex 정점의 텍스처 좌표입니다.
		 * @param c 정점의 색상입니다
		 */
		Vertex(Vec2f&& p, Vec2f&& tex, Vec4f&& c) noexcept
			: position(p)
			, uv(tex)
			, color(c) {}


		/**
		 * @brief 정점의 생성자입니다.
		 *
		 * @param p 정점의 위치입니다.
		 * @param tex 정점의 텍스처 좌표입니다.
		 * @param c 정점의 색상입니다
		 */
		Vertex(const Vec2f& p, const Vec2f& tex, const Vec4f& c) noexcept
			: position(p)
			, uv(tex)
			, color(c) {}


		/**
		 * @brief 정점의 생성자입니다.
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
		Vertex(
			float x, float y,
			float u, float v,
			float r, float g, float b, float a
		) noexcept
			: position(x, y)
			, uv(u, v)
			, color(r, g, b, a) {}


		/**
		 * @brief 정점의 복사 생성자입니다.
		 *
		 * @param instance 복사할 정점 인스턴스입니다.
		 */
		Vertex(Vertex&& instance) noexcept
			: position(instance.position)
			, uv(instance.uv)
			, color(instance.color) {}


		/**
		 * @brief 정점의 복사 생성자입니다.
		 *
		 * @param instance 복사할 정점 인스턴스입니다.
		 */
		Vertex(const Vertex& instance) noexcept
			: position(instance.position)
			, uv(instance.uv)
			, color(instance.color) {}


		/**
		 * @brief 정점의 대입 연산자입니다.
		 *
		 * @param instance 대입할 정점 인스턴스입니다.
		 *
		 * @return 대입한 객체의 참조자를 반환합니다.
		 */
		Vertex& operator=(Vertex&& instance) noexcept
		{
			if (this == &instance) return *this;

			position = instance.position;
			uv = instance.uv;
			color = instance.color;

			return *this;
		}


		/**
		 * @brief 정점의 대입 연산자입니다.
		 *
		 * @param instance 대입할 정점 인스턴스입니다.
		 *
		 * @return 대입한 객체의 참조자를 반환합니다.
		 */
		Vertex& operator=(const Vertex& instance) noexcept
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
			return sizeof(Vertex);
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


	/**
	 * @brief 2D 텍스트를 그립니다.
	 *
	 * @param font 폰트 리소스입니다.
	 * @param text 렌더링할 텍스트입니다.
	 * @param alignment 텍스트 좌표의 정렬 기준입니다.
	 * @param position 텍스트의 좌표입니다.
	 * @param color 텍스트의 RGBA 색상입니다.
	 */
	void DrawText2D(const TTFont* font, const std::wstring& text, const Vec2f& position, const Vec4f& color);


	/**
	 * @brief 텍스트에 맞게 버텍스 버퍼를 업데이트합니다.
	 *
	 * @param font 폰트 리소스입니다.
	 * @param text 렌더링할 텍스트입니다.
	 * @param startPosition 텍스트 영역의 왼쪽 상단 좌표입니다.
	 * @param color 텍스트의 색상입니다.
	 *
	 * @return 텍스트에 맞는 버텍스의 수를 반환합니다.
	 */
	uint32_t UpdateGlyphVertexBuffer(const TTFont* font, const std::wstring& text, const Vec2f& startPosition, const Vec4f& color);


private:
	/**
	 * @brief 최대 문자열의 길이입니다.
	 */
	static const int32_t MAX_STRING_LEN = 1000;


	/**
	 * @brief 글리프 정점 목록의 최대 크기입니다.
	 *
	 * @note
	 * - 문자 하나 그리는 데 필요한 정점의 수 : 6개
	 * - 만약, 최대 문자열의 길이가 1000이라면, 필요한 정점의 수 : 6 X 1000 = 6000개
	 * - 즉, 이 글리프 셰이더가 지원하는 최대 문자열의 길이는 1000이다.
	 */
	static const int32_t MAX_VERTEX_SIZE = MAX_STRING_LEN * 6;


	/**
	 * @brief 정점 목록입니다.
	 */
	std::array<Vertex, MAX_VERTEX_SIZE> vertices_;

	
	/**
	 * @brief 직교 투영 행렬입니다.
	 */
	Mat4x4 ortho_;


	/**
	 * @brief 정점 버퍼 목록의 오브젝트입니다.
	 */
	uint32_t vertexArrayObject_ = 0;


	/**
	 * @brief 정점 버퍼입니다.
	 */
	VertexBuffer* vertexBuffer_;


	/**
	 * @brief 렌더링 시 사용할 셰이더입니다.
	 */
	Shader* shader_ = nullptr;
};