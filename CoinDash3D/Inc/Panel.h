#pragma once

#include <string>

#include "Entity2D.h"
#include "Vec.h"

class GeometryRenderer2D;
class TTFont;
class TextRenderer;


/**
 * @brief 패널 엔티티입니다.
 */
class Panel : public Entity2D
{
public:
	/**
	 * @brief 패널의 레이아웃입니다.
	 */
	struct Layout
	{
		float width;
		float height;
		float side;
		Vec2f center;
		std::wstring text;
		Vec4f outlineColor;
		Vec4f backgroundColor;
		Vec4f textColor;
	};


public:
	/**
	 * @brief 패널의 생성자입니다.
	 *
	 * @param path 패널의 레이아웃 리소스 경로입니다.
	 * @param font 패널의 폰트 리소스입니다.
	 * @param geometryRenderer 2D 기하 도형을 렌더링할 때 사용할 렌더러입니다.
	 * @param textRenderer 텍스트 렌더링에 사용할 렌더러입니다.
	 */
	explicit Panel(const std::string& path, TTFont* font, GeometryRenderer2D* geometryRenderer, TextRenderer* textRenderer);


	/**
	 * @brief 패널의 생성자입니다.
	 *
	 * @param layout 패널의 레이아웃입니다.
	 * @param font 패널의 폰트 리소스입니다.
	 */
	explicit Panel(const Layout& layout, TTFont* font);


	/**
	 * @brief 패널의 가상 소멸자입니다.
	 */
	virtual ~Panel();


	/**
	 * @brief 패널의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Panel);


	/**
	 * @brief 패널을 업데이트합니다.
	 *
	 * @param deltaSeconds 델타 시간 값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 패널을 화면에 그립니다.
	 */
	virtual void Render() override;


	/**
	 * @brief 패널 내의 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


protected:
	/**
	 * @brief 패널의 레이아웃입니다.
	 */
	Layout layout_;


	/**
	 * @brief 패널에 표시할 텍스트의 폰트 리소스입니다.
	 */
	TTFont* font_ = nullptr;


	/**
	 * @brief 2D 기하 도형을 렌더링할 때 사용할 렌더러입니다.
	 */
	GeometryRenderer2D* geometryRenderer2D_ = nullptr;


	/**
	 * @brief 텍스트 렌더링에 사용할 렌더러입니다.
	 */
	TextRenderer* textRenderer_ = nullptr;
};