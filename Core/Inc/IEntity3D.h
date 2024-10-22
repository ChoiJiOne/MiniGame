#pragma once

#include "IEntity.h"

/** 3D 엔티티의 인터페이스입니다. */
class IEntity3D : public IEntity
{
public:
	IEntity3D() {}
	virtual ~IEntity3D() {}

	DISALLOW_COPY_AND_ASSIGN(IEntity3D);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Release() = 0;
};