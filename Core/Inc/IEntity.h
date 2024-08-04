#pragma once

#include "Macro.h"


namespace GameMaker
{
class IEntity
{
public:
	IEntity() = default;
	virtual ~IEntity() {}

	DISALLOW_COPY_AND_ASSIGN(IEntity);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Release() = 0;

	bool IsInitialized() const { return bIsInitialized_; }

protected:
	bool bIsInitialized_ = false;
};

}