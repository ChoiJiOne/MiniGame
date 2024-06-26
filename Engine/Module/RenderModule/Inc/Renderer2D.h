#pragma once

#include <array>

#include "RenderModule.h"
#include "VertexBuffer.h"

class Shader;


/**
 * @brief 2D 렌더링을 수행하는 렌더러입니다.
 * 
 * @note 좌표계 기준은 윈도우 좌표계 기준입니다.
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
	 * @param startPosition 선의 시작점입니다.
	 * @param endPosition 선의 끝점입니다.
	 * @param color 선의 색상입니다.
	 */
	void DrawLine(const Vec2f& startPosition, const Vec2f& endPosition, const Vec4f& color);


	/**
	 * @brief 2D 선을 그립니다.
	 * 
	 * @param startPosition 선의 시작점입니다.
	 * @param startColor 선의 시작점 색상입니다.
	 * @param endPosition 선의 끝점입니다.
	 * @param endColor 선의 끝점 색상입니다.
	 */
	void DrawLine(const Vec2f& startPosition, const Vec4f& startColor, const Vec2f& endPosition, const Vec4f& endColor);


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
	 * @param fromPosition 삼각형 시작 점입니다.
	 * @param byPosition 삼각형 중간 점입니다.
	 * @param toPosition 삼각형 끝 점입니다.
	 * @param color 삼각형의 색상입니다.
	 */
	void DrawTriangle(const Vec2f& fromPosition, const Vec2f& byPosition, const Vec2f& toPosition, const Vec4f& color);

	
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


private:
	/**
	 * @brief 그리기를 수행합니다.
	 * 
	 * @param transform 변환 행렬입니다.
	 * @param drawMode 그리기 모드입니다.
	 * @param vertexCount 정점 수입니다.
	 */
	void Draw(const Mat4x4& transform, const EDrawMode& drawMode, uint32_t vertexCount);


private:
	/**
	 * @brief 도형 정점 목록의 최대 크기입니다.
	 */
	static const int32_t MAX_VERTEX_SIZE = 10000;


	/**
	 * @brief 곡선 계산 시 최대 분할 횟수입니다.
	 */
	static const int32_t MAX_SLICE_SIZE = 7;


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
