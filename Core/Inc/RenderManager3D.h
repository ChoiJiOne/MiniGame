#pragma once

#include "IApp.h"

/** 3D 렌더 매니저는 싱글턴입니다. */
class RenderManager3D
{
public:
	DISALLOW_COPY_AND_ASSIGN(RenderManager3D);

	static RenderManager3D& Get();

private:
	friend class IApp;

	RenderManager3D() = default;
	virtual ~RenderManager3D() {}

	void Startup(); /** IApp 내부에서만 사용하는 메서드입니다. */
	void Shutdown(); /** IApp 내부에서만 사용하는 메서드입니다. */

private:
	IApp* app_ = nullptr;
};