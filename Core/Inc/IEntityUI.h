#pragma once

#include "IEntity.h"

/** UI 엔티티의 인터페이스입니다. */
class IEntityUI : public IEntity
{
public:
	enum class Type
	{
		NONE   = 0x00,
		TEXT   = 0x01,
		PANEL  = 0x02,
		BUTTON = 0x03,
	};

public:
	IEntityUI() {}
	virtual ~IEntityUI() {}

	DISALLOW_COPY_AND_ASSIGN(IEntityUI);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Release() = 0;
	virtual Type GetType() const = 0;

protected:
	Type type_ = Type::NONE;
};