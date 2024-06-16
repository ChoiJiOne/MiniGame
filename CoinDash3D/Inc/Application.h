#pragma once

#include "AudioModule.h"
#include "CrashModule.h"
#include "FileModule.h"
#include "GameModule.h"
#include "MathModule.h"
#include "PlatformModule.h"
#include "RenderModule.h"


/**
 * @brief 어플리케이션을 초기화/실행/해제하는 클래스입니다.
 */
class Application
{
public:
	/**
	 * @brief 어플리케이션의 생성자입니다.
	 */
	Application();


	/**
	 * @brief 어플리케이션의 가상 소멸자입니다.
	 */
	virtual ~Application();


	/**
	 * @brief 어플레케이션의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Application);


	/**
	 * @brief 어플리케이션을 초기화합니다.
	 */
	void Init();


	/**
	 * @brief 어플리케이션을 실행합니다.
	 */
	void Run();


private:

};