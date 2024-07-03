#pragma once

#include <array>
#include <memory>
#include <string>

#include <miniaudio.h>

#include "IManager.h"


namespace GameMaker
{
/**
 * @brief 오디오 처리를 수행하는 매니저입니다.
 * 
 * @note 이 클래스는 싱글턴입니다.
 */
class AudioManager : public IManager
{
public:
	/**
	 * @brief 오디오 매니저의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(AudioManager);


	/**
	 * @brief 오디오 매니저의 참조자를 얻습니다.
	 *
	 * @return 오디오 매니저의 참조자를 반환합니다.
	 */
	static AudioManager& Get();


	/**
	 * @brief 오디오 매니저의 사용을 시작합니다.
	 *
	 * @note 이 메서드는 한 번만 호출되어야 합니다.
	 */
	virtual void Startup() override;


	/**
	 * @brief 오디오 매니저의 사용을 종료합니다.
	 *
	 * @note
	 * - 애플리케이션 종료 전에 이 메서드를 반드시 호출하여 내부 리소스를 정리해야 합니다.
	 * - 이 메서드는 반드시 한 번만 호출되어야 합니다.
	 */
	virtual void Shutdown() override;


	/**
	 * @brief 오디오 엔진의 포인터를 얻습니다.
	 * 
	 * @return 오디오 엔진의 포인터를 반환합니다.
	 */
	ma_engine* GetEngine() { return engine_.get(); }


private:
	/**
	 * @brief 오디오 매니저의 디폴트 생성자와 빈 가상 소멸자를 삽입합니다.
	 */
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(AudioManager);


private:
	/**
	 * @brief 오디오 엔진입니다.
	 */
	std::unique_ptr<ma_engine> engine_ = nullptr;
};

}