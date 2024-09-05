#pragma warning(push)
#pragma warning(disable: 26812)
#pragma warning(disable: 26451)

#include <mimalloc.h>

#include "Assertion.h"
#include "GLTFUtils.h"

void* MallocGLTF(void* user, size_t sz)
{
	(void)(user); /** 무시. 사용하지 않음 */
	return mi_malloc(sz);
}

void FreeGLTF(void* user, void* ptr)
{
	(void)(user); /** 무시. 사용하지 않음 */
	mi_free(ptr);
}

cgltf_data* GLTFUtils::Load(const std::string& path)
{
	cgltf_result result = cgltf_result_success;
	cgltf_options options = {};
	options.memory.alloc_func = MallocGLTF;
	options.memory.free_func = FreeGLTF;
	options.memory.user_data = nullptr;

	cgltf_data* data = nullptr;

	result = cgltf_parse_file(&options, path.c_str(), &data);
	ASSERT(result == cgltf_result_success, "Failed to parse '%s' file", path.c_str());

	result = cgltf_load_buffers(&options, data, path.c_str());
	ASSERT(result == cgltf_result_success, "Failed to load '%s' file", path.c_str());

	result = cgltf_validate(data);
	ASSERT(result == cgltf_result_success, "Invalid '%s' file", path.c_str());

	return data;
}

void GLTFUtils::Free(cgltf_data* data)
{
	if (data)
	{
		cgltf_free(data);
		data = nullptr;
	}
}

#pragma warning(pop)