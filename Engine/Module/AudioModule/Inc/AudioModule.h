#pragma once

#include <array>
#include <memory>
#include <string>

struct ma_engine;
struct ma_sound;


/**
 * @brief 사운드 리소스의 ID입니다.
 */
using SoundID = int32_t;


/**
 * @brief 오디오 관련 기능을 제공하는 모듈입니다.
 */
class AudioModule
{
public:
	/**
	 * @brief 오디오 모듈 에러 목록입니다.
	 */
	enum class Errors
	{
		OK     = 0,
		FAILED = -1,  /* A generic error. */
		ERR_INVALID_ARGS = -2,
		ERR_INVALID_OPERATION = -3,
		ERR_OUT_OF_MEMORY = -4,
		ERR_OUT_OF_RANGE = -5,
		ERR_ACCESS_DENIED = -6,
		ERR_DOES_NOT_EXIST = -7,
		ERR_ALREADY_EXISTS = -8,
		ERR_TOO_MANY_OPEN_FILES = -9,
		ERR_INVALID_FILE = -10,
		ERR_TOO_BIG = -11,
		ERR_PATH_TOO_LONG = -12,
		ERR_NAME_TOO_LONG = -13,
		ERR_NOT_DIRECTORY = -14,
		ERR_IS_DIRECTORY = -15,
		ERR_DIRECTORY_NOT_EMPTY = -16,
		ERR_AT_END = -17,
		ERR_NO_SPACE = -18,
		ERR_BUSY = -19,
		ERR_IO_ERROR = -20,
		ERR_INTERRUPT = -21,
		ERR_UNAVAILABLE = -22,
		ERR_ALREADY_IN_USE = -23,
		ERR_BAD_ADDRESS = -24,
		ERR_BAD_SEEK = -25,
		ERR_BAD_PIPE = -26,
		ERR_DEADLOCK = -27,
		ERR_TOO_MANY_LINKS = -28,
		ERR_NOT_IMPLEMENTED = -29,
		ERR_NO_MESSAGE = -30,
		ERR_BAD_MESSAGE = -31,
		ERR_NO_DATA_AVAILABLE = -32,
		ERR_INVALID_DATA = -33,
		ERR_TIMEOUT = -34,
		ERR_NO_NETWORK = -35,
		ERR_NOT_UNIQUE = -36,
		ERR_NOT_SOCKET = -37,
		ERR_NO_ADDRESS = -38,
		ERR_BAD_PROTOCOL = -39,
		ERR_PROTOCOL_UNAVAILABLE = -40,
		ERR_PROTOCOL_NOT_SUPPORTED = -41,
		ERR_PROTOCOL_FAMILY_NOT_SUPPORTED = -42,
		ERR_ADDRESS_FAMILY_NOT_SUPPORTED = -43,
		ERR_SOCKET_NOT_SUPPORTED = -44,
		ERR_CONNECTION_RESET = -45,
		ERR_ALREADY_CONNECTED = -46,
		ERR_NOT_CONNECTED = -47,
		ERR_CONNECTION_REFUSED = -48,
		ERR_NO_HOST = -49,
		ERR_IN_PROGRESS = -50,
		ERR_CANCELLED = -51,
		ERR_MEMORY_ALREADY_MAPPED = -52,
		ERR_CRC_MISMATCH = -100, /* General non-standard errors. */
		ERR_FORMAT_NOT_SUPPORTED = -200, /* General miniaudio-specific errors. */
		ERR_DEVICE_TYPE_NOT_SUPPORTED = -201,
		ERR_SHARE_MODE_NOT_SUPPORTED = -202,
		ERR_NO_BACKEND = -203,
		ERR_NO_DEVICE = -204,
		ERR_API_NOT_FOUND = -205,
		ERR_INVALID_DEVICE_CONFIG = -206,
		ERR_LOOP = -207,
		ERR_BACKEND_NOT_ENABLED = -208,
		ERR_DEVICE_NOT_INITIALIZED = -300, /* State errors. */
		ERR_DEVICE_ALREADY_INITIALIZED = -301,
		ERR_DEVICE_NOT_STARTED = -302,
		ERR_DEVICE_NOT_STOPPED = -303,
		ERR_FAILED_TO_INIT_BACKEND = -400, /* Operation errors. */
		ERR_FAILED_TO_OPEN_BACKEND_DEVICE = -401,
		ERR_FAILED_TO_START_BACKEND_DEVICE = -402,
		ERR_FAILED_TO_STOP_BACKEND_DEVICE = -403
	};


	/**
	 * @brief 오디오 모듈을 초기화합니다.
	 * 
	 * @return 오디오 모듈 초기화에 성공하면 Errors 열겨형의 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors Init();


	/**
	 * @brief 오디오 모듈의 초기화를 해제합니다.
	 */
	static void Uninit();


	/**
	 * @brief 사운드 리소스를 생성합니다.
	 * 
	 * @param path 사운드 리소스의 경로입니다.
	 * 
	 * @return 생성된 사운드 리소스의 ID를 반환합니다.
	 */
	static SoundID CreateSound(const std::string& path);


	/**
	 * @brief 사운드 리소스를 파괴합니다.
	 * 
	 * @param soundID 파괴할 사운드 리소스의 ID입니다.
	 */
	static void DestroySound(const SoundID& soundID);


	/**
	 * @brief 사운드의 크기를 설정합니다.
	 *
	 * @param soundID 사운드 리소스의 ID입니다.
	 * @param volume 사운드의 크기입니다. 범위는 0.0 ~ 1.0 입니다.
	 */
	static void SetSoundVolume(const SoundID& soundID, float volume);


	/**
	 * @brief 현재 사운드의 크기를 얻습니다.
	 *
	 * @param soundID 사운드 리소스의 ID입니다.
	 * 
	 * @return 사운드의 크기를 얻습니다.
	 * 
	 * @note 사운드 크기의 범위는 0.0 ~ 1.0 입니다.
	 */
	static float GetSoundVolume(const SoundID& soundID);


	/**
	 * @brief 사운드의 반복 여부를 설정합니다.
	 *
	 * @param soundID 사운드 리소스의 ID입니다.
	 * @param bIsLoop 사운드 반복 여부입니다.
	 */
	static void SetSoundLooping(const SoundID& soundID, bool bIsLoop);


	/**
	 * @brief 사운드의 반복 여부를 얻습니다.
	 *
	 * @param soundID 사운드 리소스의 ID입니다.
	 * 
	 * @return 사운드가 반복한다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	static bool IsSoundLooping(const SoundID& soundID);


	/**
	 * @brief 사운드를 플레이합니다.
	 * 
	 * @param soundID 사운드 리소스의 ID입니다.
	 *
	 * @note 이전에 중지한 적이 있다면 중지한 시점부터 플레이됩니다.
	 */
	static void PlaySound(const SoundID& soundID);


	/**
	 * @brief 사운드가 플레이중인지 확인합니다.
	 * 
	 * @param soundID 사운드 리소스의 ID입니다.
	 *
	 * @return 사운드가 플레이 중이라면 true, 그렇지 않다면 false를 반환합니다.
	 */
	static bool IsSoundPlaying(const SoundID& soundID);


	/**
	 * @brief 사운드 플레이가 끝났는지 확인합니다.
	 * 
	 * @param soundID 사운드 리소스의 ID입니다.
	 *
	 * @return 사운드 플레이가 끝났다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	static bool IsDoneSound(const SoundID& soundID);


	/**
	 * @brief 사운드 플레이를 중지합니다.
	 * 
	 * @param soundID 사운드 리소스의 ID입니다.
	 */
	static void StopSound(const SoundID& soundID);


	/**
	 * @brief 사운드 플레이가 중지 되었는지 확인합니다.
	 * 
	 * @param soundID 사운드 리소스의 ID입니다.
	 *
	 * @return 사운드 플레이가 중지 되었다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	static bool IsSoundStopping(const SoundID& soundID);


	/**
	 * @brief 사운드를 초기화합니다.
	 * 
	 * @param soundID 사운드 리소스의 ID입니다.
	 */
	static void ResetSound(const SoundID& soundID);

	
private:
	/**
	 * @brief 오디오 모듈이 초기화되었는지 확인합니다.
	 */
	static bool bIsInitialize;


	/**
	 * @brief 오디오 엔진입니다.
	 */
	static std::unique_ptr<ma_engine> engine;


	/**
	 * @brief 사운드 리소스의 최대 생성 수입니다.
	 */
	static const uint32_t MAX_SOUND_RESOURCE_SIZE = 200;


	/**
	 * @brief 캐시 크기입니다.
	 */
	static uint32_t cacheSize;


	/**
	 * @brief 사운드 리소스의 캐시입니다.
	 */
	static std::array<std::unique_ptr<ma_sound>, MAX_SOUND_RESOURCE_SIZE> cache;
};