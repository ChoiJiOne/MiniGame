#pragma once

#include <array>
#include <vector>

#include "RenderModule.h"
#include "Shader.h"
#include "Vertex2D.h"
#include "VertexBuffer.h"


/**
 * @brief 2D 기본 도형을 그리는 렌더러입니다.
 *
 * @note
 * - 이 렌더러 지원하는 2D 기본 도형은 점, 선, 삼각형, 사각형, 원, 타원입니다.
 * - 2D 기본 도형 중 삼각형을 렌더링 할 때 삼각형의 방향은 오른손 기준 CCW입니다.
 * - 2차원 좌표계는 윈도우 좌표계를 기준으로 따릅니다.
 */
class GeometryRenderer2D : public Shader
{
public:
	/**
	 * @brief 2D 기본 도형을 그리는 렌더러의 생성자입니다.
	 */
	explicit GeometryRenderer2D();


	/**
	 * @brief 2D 기본 도형을 그리는 렌더러의 가상 소멸자입니다.
	 *
	 * @note 셰이더의 내부에서 할당된 요소를 해제하기 위해서는 반드시 Release를 호출해야 합니다.
	 */
	virtual ~GeometryRenderer2D();


	/**
	 * @brief 2D 기본 도형을 그리는 렌더러의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(GeometryRenderer2D);


	/**
	 * @brief 렌더러의 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 직교 투영 행렬을 설정합니다.
	 * 
	 * @param ortho 설정할 직교 투영 행렬입니다.
	 */
	void SetOrtho(const Mat4x4& ortho) { ortho_ = ortho; }


	/**
	 * @brief 2D 점들을 그립니다.
	 * 
	 * @param positions 2D 점들입니다.
	 * @param color 점들의 색상입니다.
	 * @param pointSize 점의 크기입니다. 기본 값은 1.0f입니다.
	 *
	 * @note 2D 점들의 개수는 MAX_VERTEX_SIZE(10000)의 크기를 넘을 수 없습니다.
	 */
	void DrawPoints2D(const std::vector<Vec2f>& positions, const Vec4f& color, float pointSize = 1.0f);


	/**
	 * @brief 점들을 연결한 2D 선을 그립니다.
	 * 
	 * @param positions 2D 점들입니다.
	 * @param color 점들의 색상입니다.
	 *
	 * @note 2D 점들의 개수는 MAX_VERTEX_SIZE(10000)의 크기를 넘을 수 없습니다.
	 */
	void DrawConnectPoints2D(const std::vector<Vec2f>& positions, const Vec4f& color);


	/**
	 * @brief 2D 선을 그립니다.
	 * 
	 * @param fromPosition 선의 시작점입니다.
	 * @param toPosition 선의 끝점입니다.
	 * @param color 선의 색상입니다.
	 */
	void DrawLine2D(const Vec2f& fromPosition, const Vec2f& toPosition, const Vec4f& color);


	/**
	 * @brief 2D 선을 그립니다.
	 *
	 * @param ortho 직교 투영 행렬입니다.
	 * @param fromPosition 선의 시작점입니다.
	 * @param fromColor 선의 시작점 색상입니다.
	 * @param toPosition 선의 끝점입니다.
	 * @param toColor 선의 끝점 색상입니다.
	 */
	void DrawLine2D(const Vec2f& fromPosition, const Vec4f& fromColor, const Vec2f& toPosition, const Vec4f& toColor);


	/**
	 * @brief 2D 선들을 그립니다.
	 * 
	 * @param positions 연결되지 않은 선의 점 목록입니다.
	 * @param color 선의 색상입니다.
	 */
	void DrawLines2D(const std::vector<Vec2f>& positions, const Vec4f& color);


	/**
	 * @brief 2D 삼각형을 그립니다.
	 * 
	 * @param fromPosition 삼각형 시작 점입니다.
	 * @param byPosition 삼각형 중간 점입니다.
	 * @param toPosition 삼각형 끝 점입니다.
	 * @param color 삼각형의 색상입니다.
	 */
	void DrawTriangle2D(const Vec2f& fromPosition, const Vec2f& byPosition, const Vec2f& toPosition, const Vec4f& color);


	/**
	 * @brief 2D 삼각형을 그립니다.
	 * 
	 * @param fromPosition 삼각형 시작 점입니다.
	 * @param fromColor 삼각형 시작 점의 색상입니다.
	 * @param byPosition 삼각형 중간 점입니다.
	 * @param byColor 삼각형 중간 점의 색상입니다.
	 * @param toPosition 삼각형 끝 점입니다.
	 * @param toColor 삼각형 끝 점의 색상입니다.
	 */
	void DrawTriangle2D(
		const Vec2f& fromPosition, const Vec4f& fromColor,
		const Vec2f& byPosition, const Vec4f& byColor,
		const Vec2f& toPosition, const Vec4f& toColor
	);


	/**
	 * @brief 2D 와이어 프레임 삼각형을 그립니다.
	 * 
	 * @param fromPosition 삼각형 시작 점입니다.
	 * @param byPosition 삼각형 중간 점입니다.
	 * @param toPosition 삼각형 끝 점입니다.
	 * @param color 삼각형의 색상입니다.
	 */
	void DrawWireframeTriangle2D(const Vec2f& fromPosition, const Vec2f& byPosition, const Vec2f& toPosition, const Vec4f& color);


	/**
	 * @brief 2D 와이어 프레임 삼각형을 그립니다.
	 * 
	 * @param fromPosition 삼각형 시작 점입니다.
	 * @param fromColor 삼각형 시작 점의 색상입니다.
	 * @param byPosition 삼각형 중간 점입니다.
	 * @param byColor 삼각형 중간 점의 색상입니다.
	 * @param toPosition 삼각형 끝 점입니다.
	 * @param toColor 삼각형 끝 점의 색상입니다.
	 */
	void DrawWireframeTriangle2D(
		const Vec2f& fromPosition, const Vec4f& fromColor,
		const Vec2f& byPosition, const Vec4f& byColor,
		const Vec2f& toPosition, const Vec4f& toColor
	);


	/**
	 * @brief 2D 직사각형을 그립니다.
	 * 
	 * @param center 직사각형의 중심 좌표입니다.
	 * @param width 직사각형의 가로 크기입니다.
	 * @param heigt 직사각형의 세로 크기입니다.
	 * @param rotate 직사각형의 라디안 회전 각도입니다.
	 * @param color 직사각형의 색상입니다.
	 */
	void DrawRectangle2D(const Vec2f& center, float width, float height, float rotate, const Vec4f& color);


	/**
	 * @brief 2D 와이어 프레임 직사각형을 그립니다.
	 * 
	 * @param center 직사각형의 중심 좌표입니다.
	 * @param width 직사각형의 가로 크기입니다.
	 * @param heigt 직사각형의 세로 크기입니다.
	 * @param rotate 직사각형의 라디안 회전 각도입니다.
	 * @param color 직사각형의 색상입니다.
	 */
	void DrawWireframeRectangle2D(const Vec2f& center, float width, float height, float rotate, const Vec4f& color);


	/**
	 * @brief 모서리가 둥근 2D 사각형을 그립니다.
	 * 
	 * @param center 직사각형의 중심 좌표입니다.
	 * @param width 직사각형의 가로 크기입니다.
	 * @param heigt 직사각형의 세로 크기입니다.
	 * @param side 사각형의 둥근 부분의 크기입니다.
	 * @param rotate 직사각형의 라디안 회전 각도입니다.
	 * @param color 직사각형의 색상입니다.
	 */
	void DrawRoundRectangle2D(const Vec2f& center, float width, float height, float side, float rotate, const Vec4f& color);


	/**
	 * @brief 모서리가 둥근 2D 와이어 프레임 사각형을 그립니다.
	 * 
	 * @param center 직사각형의 중심 좌표입니다.
	 * @param width 직사각형의 가로 크기입니다.
	 * @param heigt 직사각형의 세로 크기입니다.
	 * @param side 사각형의 둥근 부분의 크기입니다.
	 * @param rotate 직사각형의 라디안 회전 각도입니다.
	 * @param color 직사각형의 색상입니다.
	 */
	void DrawWireframeRoundRectangle2D(const Vec2f& center, float width, float height, float side, float rotate, const Vec4f& color);


	/**
	 * @brief 2D 원을 그립니다.
	 * 
	 * @param center 원의 중심 좌표입니다.
	 * @param radius 원의 반지름 길이입니다.
	 * @param color 원의 색상입니다.
	 * @param sliceCount 원의 둘레 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void DrawCircle2D(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount = 300);


	/**
	 * @brief 2D 와이어 프레임 원을 그립니다.
	 * 
	 * @param center 원의 중심 좌표입니다.
	 * @param radius 원의 반지름 길이입니다.
	 * @param color 원의 색상입니다.
	 * @param sliceCount 원 둘레의 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void DrawWireframeCircle2D(const Vec2f& center, float radius, const Vec4f& color, int32_t sliceCount = 300);


	/**
	 * @brief 2D 타원을 그립니다.
	 * 
	 * @param center 타원의 중심 좌표입니다.
	 * @param xAxis 타원의 x축 길이입니다. 만약, 이 길이가 y축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param yAxis 타원의 y축 길이입니다. 만약, 이 길이가 x축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param rotate 타원의 라디안 회전 각도입니다.
	 * @param color 타원의 색상입니다.
	 * @param sliceCount 타원 둘레의 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void DrawEllipse2D(const Vec2f& center, float xAxis, float yAxis, float rotate, const Vec4f& color, int32_t sliceCount = 300);


	/**
	 * @brief 2D 와이어 프레임 타원을 그립니다.
	 * 
	 * @param center 타원의 중심 좌표입니다.
	 * @param xAxis 타원의 x축 길이입니다. 만약, 이 길이가 y축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param yAxis 타원의 y축 길이입니다. 만약, 이 길이가 x축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param rotate 타원의 라디안 회전 각도입니다.
	 * @param color 타원의 색상입니다.
	 * @param sliceCount 타원 둘레의 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void DrawWireframeEllipse2D(const Vec2f& center, float xAxis, float yAxis, float rotate, const Vec4f& color, int32_t sliceCount = 300);


private:
	/**
	 * @brief 2D 기본 도형 그리기를 수행합니다.
	 *
	 * @param transform 변환 행렬입니다.
	 * @param drawMode 그리기 모드입니다.
	 * @param vertexCount 정점 수입니다.
	 */
	void DrawGeometry2D(const Mat4x4& transform, const EDrawMode& drawMode, uint32_t vertexCount);


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
	std::array<VertexPositionColor2D, MAX_VERTEX_SIZE> vertices_;


	/**
	 * @brief 정점 버퍼 목록의 오브젝트입니다.
	 */
	uint32_t vertexArrayObject_ = 0;


	/**
	 * @brief 셰이더 내에서 사용할 정점 버퍼입니다.
	 */
	VertexBuffer vertexBuffer_;


	/**
	 * @brief 직교 투영 행렬입니다.
	 */
	Mat4x4 ortho_;


	/**
	 * @brief 포인트 크기입니다.
	 */
	float pointSize_ = 1.0f;
};