#pragma once

#include <array>
#include <functional>
#include <memory>
#include <map>
#include <string>
#include <vector>

#include "Macro.h"

class IGameScene;

class IApp
{
public:
	IApp(const char* title, int32_t x, int32_t y, int32_t w, int32_t h, bool bIsResizble, bool bIsFullscreen);
	virtual ~IApp();

	DISALLOW_COPY_AND_ASSIGN(IApp); /** 명시적으로 복사 생성자 및 대입 연산자 삭제 */

	virtual void Startup() = 0;
	virtual void Shutdown() = 0;
	
	void Run();

	void SetCurrentScene(IGameScene* scene) { currentScene_ = scene; }
	void AddSceneByName(const std::string& name, IGameScene* scene);
	void DeleteScenesByName(const std::string& name);

	template <typename TScene>
	TScene* GetSceneByName(const std::string& name)
	{
		auto it = scenes_.find(name);
		if (it == scenes_.end())
		{
			return nullptr; // can't find.
		}

		return reinterpret_cast<TScene*>(it->second);
	}

	static IApp* Get();

protected:
	void CreateAppWindow(const char* title, int32_t x, int32_t y, int32_t w, int32_t h, bool bIsResizble, bool bIsFullscreen);
	
protected:
	static IApp* instance_;

	void* window_ = nullptr;
	
	IGameScene* currentScene_ = nullptr;
	std::map<std::string, IGameScene*> scenes_;
};