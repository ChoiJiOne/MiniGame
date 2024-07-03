#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <miniaudio.h>

#include "IResource.h"


namespace GameMaker
{
/**
 * @brief 사운드 리소스입니다.
 */
class Sound : public IResource
{
public:
	/**
	 * @brief 사운드 리소스를 생성합니다.
	 * 
	 * @param path 사운드 리소스의 경로입니다.
	 */
	explicit Sound(const std::string& path);


	/**
	 * @brief 사운드 리소스의 가상 소멸자입니다.
	 */
	virtual ~Sound();


	/**
	 * @brief 사운드 리소스의 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Sound);


	/**
	 * @brief 사운드 리소스 내부에 할당된 요소를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 사운드의 크기를 설정합니다.
	 * 
	 * @param volume 사운드의 크기입니다. 범위는 0.0 ~ 1.0 입니다.
	 */
	void SetVolume(float volume);


	/**
	 * @brief 현재 사운드의 크기를 얻습니다.
	 *
	 * @return 사운드의 크기를 얻습니다.
	 *
	 * @note 사운드 크기의 범위는 0.0 ~ 1.0 입니다.
	 */
	float GetVolume();


	/**
	 * @brief 사운드의 반복 여부를 설정합니다.
	 * @param bIsLoop 사운드 반복 여부입니다.
	 */
	void SetLooping(bool bIsLoop);


	/**
	 * @brief 사운드의 반복 여부를 얻습니다.
	 *
	 * @return 사운드가 반복한다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	bool IsLooping();


	/**
	 * @brief 사운드를 플레이합니다.
	 *
	 * @note 이전에 중지한 적이 있다면 중지한 시점부터 플레이됩니다.
	 */
	void Play();


	/**
	 * @brief 사운드가 플레이중인지 확인합니다.
	 *
	 * @return 사운드가 플레이 중이라면 true, 그렇지 않다면 false를 반환합니다.
	 */
	bool IsPlaying();


	/**
	 * @brief 사운드 플레이가 끝났는지 확인합니다.
	 *
	 * @return 사운드 플레이가 끝났다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	bool IsDone();


	/**
	 * @brief 사운드 플레이를 중지합니다.
	 */
	void Stop();


	/**
	 * @brief 사운드 플레이가 중지 되었는지 확인합니다.
	 * 
	 * @return 사운드 플레이가 중지 되었다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsStopping();


	/**
	 * @brief 사운드를 초기화합니다.
	 */
	void Reset();


private:
	/**
	 * @brief 사운드 리소스 원본입니다.
	 */
	std::unique_ptr<ma_sound> resource_ = nullptr;
};

}