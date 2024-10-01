#pragma once

#include "IEntity.h"

/** 2D 엔티티의 인터페이스입니다. */
class IEntity2D : public IEntity
{
public:
	IEntity2D() {}
	virtual ~IEntity2D() {}

	DISALLOW_COPY_AND_ASSIGN(IEntity2D);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};