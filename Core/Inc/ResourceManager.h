#pragma once

#include <array>
#include <memory>

#include "IResource.h"

class ResourceManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(ResourceManager);

	static ResourceManager& Get();

private:
	ResourceManager() = default;
	virtual ~ResourceManager() {}

private:

};