#pragma once

#include "IResource.h"

namespace GameResource
{
	/** 모든 게임 리소스는 반드시 이 함수를 통해 생성해야 함. */
	template <typename TResource, typename... Args>
	TResource* Create(Args&&... args);

	void Destroy(const IResource* resource);
}