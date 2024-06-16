#pragma once


/**
 * @brief 타입(클래스 혹은 구조체)의 복사 생성자 및 대입 연산자를 사용하지 못하도록 삭제합니다.
 *
 * @note 이 매크로가 정의되어 있지 않은 타입은 반드시 명시적으로 복사 생성자와 대입 연산자를 복사 생성자를 정의해야 합니다.
 *
 * @param TypeName 복사 생성자 및 대입 연산자를 삭제할 타입의 이름입니다.
 */
#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)    \
TypeName(TypeName&&) = delete;                \
TypeName(const TypeName&) = delete;           \
TypeName& operator=(TypeName&&) = delete;     \
TypeName& operator=(const TypeName) = delete;
#endif


/**
 * @brief 씬 인터페이스입니다.
 */
class IScene
{
public:
	/**
	 * @brief 씬 인터페이스의 생성자입니다.
	 */
	IScene() {}


	/**
	 * @brief 씬 인터페이스의 가상 소멸자입니다.
	 */
	virtual ~IScene() {}


	/**
	 * @brief 씬 인터페이스의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(IScene);


	/**
	 * @brief 씬의 한 프레임을 수행합니다.
	 *
	 * @param deltaSeconds 초 단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) = 0;


	/**
	 * @brief 씬에 진입합니다.
	 */
	virtual void Enter() = 0;


	/**
	 * @brief 씬에서 나갑니다.
	 */
	virtual void Exit() = 0;


	/**
	 * @brief 씬 전환이 감지되었는지 확인합니다.
	 *
	 * @return 씬 전환이 감지되었다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsDetectSwitch() const { return bDetectSwitch_; }


	/**
	 * @brief 씬에 연결된 씬을 얻습니다.
	 *
	 * @return 씬에 연결된 씬의 포인터를 반환합니다.
	 */
	virtual IScene* GetLink() { return link_; }


	/**
	 * @brief 씬을 연결합니다.
	 *
	 * @param link 연결할 씬의 포인터입니다.
	 */
	virtual void SetLink(IScene* link) { link_ = link; }


protected:
	/**
	 * @brief 씬에 진입했는지 확인합니다.
	 */
	bool bIsEnter_ = false;


	/**
	 * @brief 씬 전환이 감지되었는지 확인합니다.
	 */
	bool bDetectSwitch_ = false;


	/**
	 * @brief 연결된 씬입니다.
	 */
	IScene* link_ = nullptr;
};