#pragma once

#include "IEntity.h"

/** UI 엔티티의 인터페이스입니다. */
class IEntityUI : public IEntity
{
public:
	IEntityUI() {}
	virtual ~IEntityUI() {}

	DISALLOW_COPY_AND_ASSIGN(IEntityUI);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Release() = 0;
};