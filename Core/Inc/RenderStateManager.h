#pragma once

#include "Macro.h"

/** 렌더링 상태 매니저는 싱글턴입니다. */
class RenderStateManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(RenderStateManager);

	static RenderStateManager& GetRef();
	static RenderStateManager* GetPtr();

private:
	friend class IApp;

	RenderStateManager() = default;
	virtual ~RenderStateManager() {}

private:
	static RenderStateManager instance_;
};