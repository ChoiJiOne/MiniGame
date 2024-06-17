#pragma once

#include <array>

#include "IResource.h"
#include "RenderModule.h"
#include "VertexBuffer.h"

class FrameBuffer;
class Shader;


/**
 * @brief 프레임 버퍼에 후처리 효과를 적용합니다.
 */
class PostEffectComposer : public IResource
{
public:
	/**
	 * @brief 프레임 버퍼에 후처리 효과를 적용하는 객체의 디폴트 생성자입니다.
	 */
	explicit PostEffectComposer();


	/**
	 * @brief 프레임 버퍼에 후처리 효과를 적용하는 객체의 가상 소멸자입니다.
	 *
	 * @note 내부에서 할당된 요소를 해제하기 위해서는 반드시 Release를 호출해야 합니다.
	 */
	virtual ~PostEffectComposer();


	/**
	 * @brief 프레임 버퍼에 후처리 효과를 적용하는 객체의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(PostEffectComposer);


	/**
	 * @brief 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 프레임 버퍼 렌더링을 수행합니다.
	 * 
	 * @param framebuffer 전송할 프레임 버퍼입니다.
	 * @param index 프레임 버퍼의 컬러 버퍼 인덱스입니다.
	 */
	void Blit(FrameBuffer* framebuffer, uint32_t index);


private:
	/**
	 * @brief 내부에서만 사용하는 정점입니다.
	 */
	struct Vertex
	{
		/**
		 * @brief 정점의 기본 생성자입니다.
		 */
		Vertex() noexcept : position(0.0f, 0.0f) {}


		/**
		 * @brief 정점의 생성자입니다.
		 *
		 * @param p 정점의 위치입니다.
		 */
		Vertex(Vec2f&& p) noexcept : position(p) {}


		/**
		 * @brief 정점의 생성자입니다.
		 *
		 * @param p 정점의 위치입니다.
		 */
		Vertex(const Vec2f& p) noexcept : position(p) {}


		/**
		 * @brief 정점의 생성자입니다.
		 *
		 * @param x 위치의 X좌표입니다.
		 * @param y 위치의 Y좌표입니다.
		 */
		Vertex(float x, float y) noexcept : position(x, y) {}


		/**
		 * @brief 정점의 생성자입니다.
		 *
		 * @param instance 복사할 정점 인스턴스입니다.
		 */
		Vertex(Vertex&& instance) noexcept
			: position(instance.position) {}


		/**
		 * @brief 정점의 생성자입니다.
		 *
		 * @param instance 복사할 정점 인스턴스입니다.
		 */
		Vertex(const Vertex& instance) noexcept
			: position(instance.position) {}


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
	};


private:
	/**
	 * @brief 정점 목록의 최대 크기입니다.
	 */
	static const int32_t MAX_VERTEX_SIZE = 6;


	/**
	 * @brief 정점 버퍼 목록의 오브젝트입니다.
	 */
	uint32_t vertexArrayObject_ = 0;


	/**
	 * @brief 내부에서 사용할 정점 버퍼입니다.
	 */
	VertexBuffer* vertexBuffer_ = nullptr;


	/**
	 * @brief 프레임 버퍼의 블릿을 수행하는 셰이더입니다.
	 */
	Shader* blit_ = nullptr;


	/**
	 * @brief 프레임 버퍼의 바인드 슬롯입니다.
	 */
	static const uint32_t FRAME_BUFFER_BIND_SLOT = 0;
};