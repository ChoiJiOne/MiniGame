#pragma once

#include <string>
#include <vector>

#include <cgltf.h>

#include "GameMath.h"

namespace GLTFUtils
{
	cgltf_data* Load(const std::string& path);
	void Free(cgltf_data* data);

	std::vector<std::string> GetAllNodeNames(cgltf_data* data);
}