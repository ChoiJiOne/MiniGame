#pragma once

#include <array>

#include "RenderModule.h"
#include "VertexBuffer.h"

class Shader;
class TTFont;


/**
 * @brief 2D 렌더링을 수행하는 렌더러입니다.
 * 
 * @note 좌표계 기준은 다음과 같습니다.
 *     +y
 *     │
 * ────┼────x+
 *     │
 */
class Renderer2D : public IResource
{
public:
	/**
	 * @brief 2D 렌더러의 생성자입니다.
	 */
	explicit Renderer2D();


	/**
	 * @brief 2D 렌더러의 가상 소멸자입니다.
	 */
	virtual ~Renderer2D();


	/**
	 * @brief 2D 렌더러의 대입 연산자 및 복사 생성자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Renderer2D);


	/**
	 * @brief 렌더러 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 렌더러의 직교 투영 행렬을 설정합니다.
	 *
	 * @param ortho 설정할 직교 투영 행렬입니다.
	 */
	void SetOrtho(const Mat4x4& ortho) { ortho_ = ortho; }


	/**
	 * @brief 2D 점을 그립니다.
	 *
	 * @param positions 점의 배열 포인터입니다.
	 * @param size 점의 개수입니다.
	 * @param color 점의 색상입니다.
	 * @param pointSize 점의 크기입니다. 기본 값은 1.0f 입니다.
	 */
	void DrawPoint(const Vec2f* positions, uint32_t size, const Vec4f& color, float pointSize = 1.0f);


	/**
	 * @brief 2D 점을 그립니다.
	 * 
	 * @param positions 점의 배열 포인터입니다.
	 * @param colors 색상 배열 포인터입니다. 이때, 점과 색상의 수는 같아야 합니다.
	 * @param size 점의 개수입니다.
	 * @param pointSize 점의 크기입니다. 기본 값은 1.0f 입니다.
	 */
	void DrawPoint(const Vec2f* positions, const Vec4f* colors, uint32_t size, float pointSize = 1.0f);


	/**
	 * @brief 2D 점들을 연결한 선을 그립니다.
	 * 
	 * @param positions 점의 배열 포인터입니다.
	 * @param colors 색상 배열 포인터입니다. 이때, 점과 색상의 수는 같아야 합니다.
	 * @param size 점의 개수입니다.
	 */
	void DrawLine(const Vec2f* positions, const Vec4f* colors, uint32_t size);


	/**
	 * @brief 2D 선을 그립니다.
	 *
	 * @param startPos 선의 시작점입니다.
	 * @param endPos 선의 끝점입니다.
	 * @param color 선의 색상입니다.
	 */
	void DrawLine(const Vec2f& startPos, const Vec2f& endPos, const Vec4f& color);


	/**
	 * @brief 2D 선을 그립니다.
	 * 
	 * @param startPos 선의 시작점입니다.
	 * @param startColor 선의 시작점 색상입니다.
	 * @param endPos 선의 끝점입니다.
	 * @param endColor 선의 끝점 색상입니다.
	 */
	void DrawLine(const Vec2f& startPos, const Vec4f& startColor, const Vec2f& endPos, const Vec4f& endColor);


	/**
	 * @brief 2D 선들을 그립니다.
	 * 
	 * @param positions 연결되지 않은 선의 양 끝점 목록입니다.
	 * @param size 연결되지 않은 선의 양 끝점 목록의 수입니다.
	 * @param color 선의 색상입니다.
	 */
	void DrawLines(const Vec2f* positions, uint32_t size, const Vec4f& color);


	/**
	 * @brief 2D 선들을 그립니다.
	 *
	 * @param positions 연결되지 않은 선의 양 끝점 목록입니다.
	 * @param colors 색상 배열 포인터입니다. 이때, 선의 양 끝점 수와 색상의 수는 같아야 합니다.
	 * @param size 연결되지 않은 선의 양 끝점 목록의 수입니다.
	 */
	void DrawLines(const Vec2f* positions, const Vec4f* colors, uint32_t size);


	/**
	 * @brief 2D 삼각형을 그립니다.
	 * 
	 * @param fromPos 삼각형 시작 점입니다.
	 * @param byPos 삼각형 중간 점입니다.
	 * @param toPos 삼각형 끝 점입니다.
	 * @param color 삼각형의 색상입니다.
	 */
	void DrawTriangle(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color);


	/**
	 * @brief 2D 삼각형을 그립니다.
	 * 
	 * @param fromPos 삼각형 시작 점입니다.
	 * @param fromColor 삼각형 시작 점의 색상입니다.
	 * @param byPos 삼각형 중간 점입니다.
	 * @param byColor 삼각형 중간 점의 색상입니다.
	 * @param toPos 삼각형 끝 점입니다.
	 * @param toColor 삼각형 끝 점의 색상입니다.
	 */
	void DrawTriangle(const Vec2f& fromPos, const Vec4f& fromColor, const Vec2f& byPos, const Vec4f& byColor, const Vec2f& toPos, const Vec4f& toColor);


	/**
	 * @brief 2D 와이어 프레임 삼각형을 그립니다.
	 *
	 * @param fromPos 삼각형 시작 점입니다.
	 * @param byPos 삼각형 중간 점입니다.
	 * @param toPos 삼각형 끝 점입니다.
	 * @param color 삼각형의 색상입니다.
	 */
	void DrawTriangleWireframe(const Vec2f& fromPos, const Vec2f& byPos, const Vec2f& toPos, const Vec4f& color);


	/**
	 * @brief 2D 와이어 프레임 삼각형을 그립니다.
	 *
	 * @param fromPos 삼각형 시작 점입니다.
	 * @param fromColor 삼각형 시작 점의 색상입니다.
	 * @param byPos 삼각형 중간 점입니다.
	 * @param byColor 삼각형 중간 점의 색상입니다.
	 * @param toPos 삼각형 끝 점입니다.
	 * @param toColor 삼각형 끝 점의 색상입니다.
	 */
	void DrawTriangleWireframe(const Vec2f& fromPos, const Vec4f& fromColor, const Vec2f& byPos, const Vec4f& byColor, const Vec2f& toPos, const Vec4f& toColor);


	/**
	 * @brief 2D 직사각형을 그립니다.
	 * 
	 * @param center 직사각형의 중심 좌표입니다.
	 * @param w 직사각형의 가로 크기입니다.
	 * @param h 직사각형의 세로 크기입니다.
	 * @param color 직사각형의 색상입니다.
	 * @param rotate 직사각형의 회전 각도입니다. 기본 값은 0.0입니다.
	 */
	void DrawRect(const Vec2f& center, float w, float h, const Vec4f& color, float rotate = 0.0f);


	/**
	 * @brief 2D 와이어프레임 직사각형을 그립니다.
	 * 
	 * @param center 직사각형의 중심 좌표입니다.
	 * @param w 직사각형의 가로 크기입니다.
	 * @param h 직사각형의 세로 크기입니다.
	 * @param color 직사각형의 색상입니다.
	 * @param rotate 직사각형의 회전 각도입니다. 기본 값은 0.0입니다.
	 */
	void DrawRectWireframe(const Vec2f& center, float w, float h, const Vec4f& color, float rotate = 0.0f);


	/**
	 * @brief 2D 모서리가 둥근 직사각형을 그립니다.
	 * 
	 * @param center 직사각형의 중심 좌표입니다.
	 * @param w 직사각형의 가로 크기입니다.
	 * @param h 직사각형의 세로 크기입니다.
	 * @param side 사각형의 둥근 부분의 크기입니다.
	 * @param color 직사각형의 색상입니다.
	 * @param rotate 직사각형의 라디안 회전 각도입니다. 기본 값은 0.0입니다.
	 */
	void DrawRoundRect(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate = 0.0f);


	/**
	 * @brief 2D 모서리가 둥근 와이어 프레임 직사각형을 그립니다.
	 *
	 * @param center 직사각형의 중심 좌표입니다.
	 * @param w 직사각형의 가로 크기입니다.
	 * @param h 직사각형의 세로 크기입니다.
	 * @param side 사각형의 둥근 부분의 크기입니다.
	 * @param color 직사각형의 색상입니다.
	 * @param rotate 직사각형의 라디안 회전 각도입니다. 기본 값은 0.0입니다.
	 */
	void DrawRoundRectWireframe(const Vec2f& center, float w, float h, float side, const Vec4f& color, float rotate = 0.0f);


	/**
	 * @brief 2D 원을 그립니다.
	 * 
	 * @param center 원의 중심 좌표입니다.
	 * @param radius 원의 반지름 길이입니다.
	 * @param color 원의 색상입니다.
	 * @param sliceCount 원의 둘레 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void DrawCircle(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount = 300);


	/**
	 * @brief 2D 와이어 프레임 원을 그립니다.
	 *
	 * @param center 원의 중심 좌표입니다.
	 * @param radius 원의 반지름 길이입니다.
	 * @param color 원의 색상입니다.
	 * @param sliceCount 원의 둘레 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void DrawCircleWireframe(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount = 300);


	/**
	 * @brief 2D 타원을 그립니다.
	 *
	 * @param center 타원의 중심 좌표입니다.
	 * @param xAxis 타원의 x축 길이입니다. 만약, 이 길이가 y축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param yAxis 타원의 y축 길이입니다. 만약, 이 길이가 x축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param color 타원의 색상입니다.
	 * @param rotate 타원의 라디안 회전 각도입니다. 기본 값은 0.0입니다.
	 * @param sliceCount 타원 둘레의 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void DrawEllipse(const Vec2f& center, float xAxis, float yAxis, const Vec4f& color, float rotate = 0.0f, int32_t sliceCount = 300);


	/**
	 * @brief 2D 와이어 프레임 타원을 그립니다.
	 *
	 * @param center 타원의 중심 좌표입니다.
	 * @param xAxis 타원의 x축 길이입니다. 만약, 이 길이가 y축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param yAxis 타원의 y축 길이입니다. 만약, 이 길이가 x축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param color 타원의 색상입니다.
	 * @param rotate 타원의 라디안 회전 각도입니다. 기본 값은 0.0입니다.
	 * @param sliceCount 타원 둘레의 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void DrawEllipseWireframe(const Vec2f& center, float xAxis, float yAxis, const Vec4f& color, float rotate = 0.0f, int32_t sliceCount = 300);


	/**
	 * @brief 2D 문자열을 그립니다.
	 * 
	 * @param font 폰트 리소스입니다.
	 * @param text 렌더링할 텍스트입니다.
	 * @param pos 렌더링할 텍스트의 왼쪽 상단 좌표입니다.
	 * @param color 텍스트의 RGBA 색상입니다.
	 */
	void DrawString(const TTFont* font, const std::wstring& text, const Vec2f& pos, const Vec4f& color);


private:
	/**
	 * @brief 2D 렌더러 내부에서 사용하는 정점입니다.
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
		 * @param t 정점의 텍스처 좌표입니다.
		 * @param c 정점의 색상입니다.
		 */
		Vertex(Vec2f&& p, Vec2f& t, Vec4f&& c) noexcept
			: position(p)
			, uv(t)
			, color(c) {}


		/**
		 * @brief 정점의 생성자입니다.
		 *
		 * @param p 정점의 위치입니다.
		 * @param t 정점의 텍스처 좌표입니다.
		 * @param c 정점의 색상입니다.
		 */
		Vertex(const Vec2f& p, const Vec2f& t, const Vec4f& c) noexcept
			: position(p)
			, uv(t)
			, color(c) {}


		/**
		 * @brief 정점의 생성자입니다.
		 *
		 * @param x 정점 위치의 X좌표입니다.
		 * @param y 정점 위치의 Y좌표입니다.
		 * @param u 정점 텍스처의 U좌표입니다.
		 * @param v 정점 텍스처의 V좌표입니다.
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
		 * @brief 정점의 생성자입니다.
		 *
		 * @param instance 복사할 정점 인스턴스입니다.
		 */
		Vertex(Vertex&& instance) noexcept
			: position(instance.position)
			, uv(instance.uv)
			, color(instance.color) {}


		/**
		 * @brief 정점의 생성자입니다.
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
	 * @brief 그리기 모드의 종류입니다.
	 */
	enum class EMode
	{
		GEOMETRY = 0x00,
		STRING   = 0x01,
	};


private:
	/**
	 * @brief 렌더링할 텍스트에 맞게 버텍스 버퍼를 설정합니다.
	 * 
	 * @param font 폰트 리소스입니다.
	 * @param text 렌더링할 텍스트입니다.
	 * @param pos 텍스트 영역의 왼쪽 상단 좌표입니다.
	 * @param color 텍스트의 색상입니다.
	 *
	 * @return 텍스트에 맞는 버텍스의 수를 반환합니다.
	 */
	uint32_t SetGlyphVertexBuffer(const TTFont* font, const std::wstring& text, const Vec2f& pos, const Vec4f& color);


	/**
	 * @brief 그리기를 수행합니다.
	 * 
	 * @param transform 변환 행렬입니다.
	 * @param drawMode 그리기 모드입니다.
	 * @param vertexCount 정점 수입니다.
	 * @param mode 현재 그리기 모드입니다.
	 */
	void Draw(const Mat4x4& transform, const EDrawMode& drawMode, uint32_t vertexCount, const EMode& mode);


private:
	/**
	 * @brief 도형 정점 목록의 최대 크기입니다.
	 */
	static const int32_t MAX_VERTEX_SIZE = 10000;


	/**
	 * @brief 베지어 곡선 계산 시 최대 분할 횟수입니다.
	 */
	static const int32_t MAX_SLICE_SIZE = 20;


	/**
	 * @brief 도형의 정점 목록입니다.
	 */
	std::array<Vertex, MAX_VERTEX_SIZE> vertices_;


	/**
	 * @brief 렌더러의 직교 투영 행렬입니다.
	 */
	Mat4x4 ortho_;


	/**
	 * @brief 점의 크기입니다.
	 */
	float pointSize_ = 1.0f;


	/**
	 * @brief 정점 버퍼 목록의 오브젝트입니다.
	 */
	uint32_t vertexArrayObject_ = 0;


	/**
	 * @brief 정점 버퍼입니다.
	 */
	VertexBuffer* vertexBuffer_ = nullptr;


	/**
	 * @brief 렌더링 시 사용할 셰이더입니다.
	 */
	Shader* shader_ = nullptr;
};
