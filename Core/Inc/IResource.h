#pragma once

#include "Macro.h"


namespace GameMaker
{ 
/**
 * 이 클래스는 엔진 내의 리소스 인터페이스로, 엔진 내의 리소스 (텍스처, 메쉬, 렌더러, 폰트 등등)는 반드시 이 인터페이스를 상속받아야 합니다.
 */
class IResource
{
public:
	IResource() = default;
	virtual ~IResource() {}

	DISALLOW_COPY_AND_ASSIGN(IResource);

	virtual void Release() = 0;

	bool IsInitialized() const { return bIsInitialized_; }

protected:
	bool bIsInitialized_ = false;
};

}