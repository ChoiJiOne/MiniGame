#pragma warning(push)
#pragma warning(disable: 26812)
#pragma warning(disable: 26451)

#include "Assertion.h"
#include "MathModule.h"

#include "GLTFLoader.h"

template <typename T, uint32_t N>
void GLTFLoader::TrackFromChannel(Track<T, N>& inOutTrack, const cgltf_animation_channel* channel)
{
	cgltf_animation_sampler* sampler = channel->sampler;

	EInterpolation interpolation = EInterpolation::CONSTANT;
	if (channel->sampler->interpolation == cgltf_interpolation_type_linear)
	{
		interpolation = EInterpolation::LINEAR;
	}
	else if (channel->sampler->interpolation == cgltf_interpolation_type_cubic_spline)
	{
		interpolation = EInterpolation::CUBIC;
	}

	bool isSamplerCubic = (interpolation == EInterpolation::CUBIC);
	inOutTrack.SetInterpolation(interpolation);

	std::vector<float> timelineFloats;
	GetScalarValues(timelineFloats, 1, sampler->input);

	std::vector<float> valueFloats;
	GetScalarValues(valueFloats, N, sampler->output);

	uint32_t numKeyframes = static_cast<uint32_t>(sampler->input->count);
	uint32_t numberOfValuesPerKeyframe = valueFloats.size() / timelineFloats.size();
	inOutTrack.Resize(numKeyframes);

	for (uint32_t index = 0; index < numKeyframes; ++index)
	{
		Keyframe<N>& keyframe = inOutTrack[index];

		int32_t baseIndex = index * numberOfValuesPerKeyframe;
		int32_t offset = 0;

		keyframe.time = timelineFloats[index];

		for (int32_t component = 0; component < N; ++component)
		{
			keyframe.in[component] = isSamplerCubic ? valueFloats[baseIndex + offset++] : 0.0f;
		}

		for (int32_t component = 0; component < N; ++component)
		{
			keyframe.value[component] = valueFloats[baseIndex + offset++];
		}

		for (int32_t component = 0; component < N; ++component)
		{
			keyframe.out[component] = isSamplerCubic ? valueFloats[baseIndex + offset++] : 0.0f;
		}
	}
}

cgltf_data* GLTFLoader::Load(const std::string& path)
{
	cgltf_result result = cgltf_result_success;
	cgltf_options options = {};
	cgltf_data* data = nullptr;

	result = cgltf_parse_file(&options, path.c_str(), &data);
	ASSERT(result == cgltf_result_success, "failed to parse %s file", path.c_str());

	result = cgltf_load_buffers(&options, data, path.c_str());
	ASSERT(result == cgltf_result_success, "failed to load %s file", path.c_str());

	result = cgltf_validate(data);
	ASSERT(result == cgltf_result_success, "invalid %s gltf file", path.c_str());

	return data;
}

void GLTFLoader::Free(cgltf_data* data)
{
	if (data)
	{
		cgltf_free(data);
		data = nullptr;
	}
}

Pose GLTFLoader::LoadRestPose(cgltf_data* data)
{
	uint32_t numJoints = static_cast<uint32_t>(data->nodes_count);
	Pose restPose(numJoints);

	for (uint32_t index = 0; index < numJoints; ++index)
	{
		cgltf_node* node = &(data->nodes[index]);

		Transform transform = GetLocalTransform(node);
		restPose.SetLocalTransform(index, transform);

		int32_t parent = GetNodeIndex(node->parent, data->nodes, numJoints);
		restPose.SetParent(index, parent);
	}

	return restPose;
}

Pose GLTFLoader::LoadBindPose(cgltf_data* data)
{
	Pose restPose = LoadRestPose(data);
	uint32_t numJoints = restPose.Size();
	std::vector<Transform> worldBindPose(numJoints);

	for (uint32_t index = 0; index < numJoints; ++index)
	{
		worldBindPose[index] = restPose.GetGlobalTransform(index);
	}

	uint32_t numSkins = static_cast<uint32_t>(data->skins_count);
	for (uint32_t index = 0; index < numSkins; ++index)
	{
		cgltf_skin* skin = &(data->skins[index]);
		std::vector<float> invBindAccessor;
		GetScalarValues(invBindAccessor, 16, skin->inverse_bind_matrices);

		uint32_t countJoints = static_cast<uint32_t>(skin->joints_count);
		for (uint32_t joint = 0; joint < countJoints; ++joint)
		{
			float* matrix = &(invBindAccessor[joint * 16]);
			Mat4x4 invBindMatrix(
				matrix[0],  matrix[1],  matrix[2],  matrix[3],
				matrix[4],  matrix[5],  matrix[6],  matrix[7],
				matrix[8],  matrix[9],  matrix[10], matrix[11],
				matrix[12], matrix[13], matrix[14], matrix[15]
			);

			Mat4x4 bindMatrix = Mat4x4::Inverse(invBindMatrix);
			Transform bindTransform = Transform::ToTransform(bindMatrix);

			cgltf_node* jointNode = skin->joints[joint];
			int32_t jointIndex = GetNodeIndex(jointNode, data->nodes, numJoints);
			worldBindPose[jointIndex] = bindTransform;
		}
	}

	Pose bindPose = restPose;
	for (uint32_t index = 0; index < numJoints; ++index)
	{
		Transform current = worldBindPose[index];
		int32_t p = bindPose.GetParent(index);
		if (p >= 0)
		{
			Transform parent = worldBindPose[p];
			current = Transform::Combine(Transform::Inverse(parent), current);
		}

		bindPose.SetLocalTransform(index, current);
	}

	return bindPose;
}

std::vector<std::string> GLTFLoader::LoadJointNames(cgltf_data* data)
{
	uint32_t numJoints = static_cast<uint32_t>(data->nodes_count);
	std::vector<std::string> jointNames(numJoints, "EMPTY NODE");

	for (uint32_t index = 0; index < numJoints; ++index)
	{
		cgltf_node* node = &(data->nodes[index]);

		if (node->name)
		{
			jointNames[index] = node->name;
		}
	}

	return jointNames;
}

Skeleton GLTFLoader::LoadSkeleton(cgltf_data* data)
{
	return Skeleton(LoadRestPose(data), LoadBindPose(data), LoadJointNames(data));
}

std::vector<Clip> GLTFLoader::LoadAnimationClips(cgltf_data* data)
{
	uint32_t numClips = static_cast<uint32_t>(data->animations_count);
	uint32_t numNodes = static_cast<uint32_t>(data->nodes_count);

	std::vector<Clip> clips(numClips);

	for (uint32_t index = 0; index < numClips; ++index)
	{
		clips[index].SetName(data->animations[index].name);

		uint32_t numChannels = static_cast<uint32_t>(data->animations[index].channels_count);
		for(uint32_t ci = 0; ci < numChannels; ++ci)
		{
			cgltf_animation_channel* channel = &(data->animations[index].channels[ci]);
			cgltf_node* target = channel->target_node;
			int32_t nodeID = GetNodeIndex(target, data->nodes, numNodes);

			if (channel->target_path == cgltf_animation_path_type_translation)
			{
				VectorTrack& track = clips[index][nodeID].GetPositionTrack();
				TrackFromChannel<Vec3f, 3>(track, channel);
			}
			else if (channel->target_path == cgltf_animation_path_type_scale)
			{
				VectorTrack& track = clips[index][nodeID].GetScaleTrack();
				TrackFromChannel<Vec3f, 3>(track, channel);
			}
			else if (channel->target_path == cgltf_animation_path_type_rotation)
			{
				QuaternionTrack& track = clips[index][nodeID].GetRotateTrack();
				TrackFromChannel<Quat, 4>(track, channel);
			}
		}

		clips[index].RecalculateDuration();
	}

	return clips;
}

std::vector<GLTFLoader::MeshData> GLTFLoader::LoadMeshData(cgltf_data* data)
{
	std::vector<MeshData> meshes;
	cgltf_node* nodes = data->nodes;
	uint32_t numNodes = static_cast<uint32_t>(data->nodes_count);

	for (uint32_t index = 0; index < numNodes; ++index)
	{
		cgltf_node* node = &(nodes[index]);
		if (!node->mesh || !node->skin)
		{
			continue;
		}

		uint32_t numPrims = static_cast<uint32_t>(node->mesh->primitives_count);
		for (uint32_t pi = 0; pi < numPrims; ++pi)
		{
			meshes.push_back(MeshData());
			MeshData& mesh = meshes.back();

			cgltf_primitive* primitive = &(node->mesh->primitives[pi]);

			uint32_t numAttrib = static_cast<uint32_t>(primitive->attributes_count);
			for (uint32_t ai = 0; ai < numAttrib; ++ai)
			{
				cgltf_attribute* attrib = &(primitive->attributes[ai]);
				GetMeshFromAttribute(mesh, attrib, node->skin, nodes, numNodes);
			}

			if (primitive->indices)
			{
				uint32_t numIndices = static_cast<uint32_t>(primitive->indices->count);
				mesh.indices.resize(numIndices);

				for (uint32_t i = 0; i < numIndices; ++i)
				{
					mesh.indices[i] = static_cast<uint32_t>(cgltf_accessor_read_index(primitive->indices, i));
				}
			}
		}
	}

	return meshes;
}

Transform GLTFLoader::GetLocalTransform(cgltf_node* node)
{
	Transform transform;

	if (node->has_matrix)
	{
		Mat4x4 m(
			 node->matrix[0],  node->matrix[1],  node->matrix[2],  node->matrix[3],
			 node->matrix[4],  node->matrix[5],  node->matrix[6],  node->matrix[7],
			 node->matrix[8],  node->matrix[9], node->matrix[10], node->matrix[11],
			node->matrix[12], node->matrix[13], node->matrix[14], node->matrix[15]
		);

		transform = Transform::ToTransform(m);
	}

	if (node->has_translation)
	{
		transform.position = Vec3f(node->translation[0], node->translation[1], node->translation[2]);
	}

	if (node->has_rotation)
	{
		transform.rotate = Quat(node->rotation[0], node->rotation[1], node->rotation[2], node->rotation[3]);
	}

	if (node->has_scale)
	{
		transform.scale = Vec3f(node->scale[0], node->scale[1], node->scale[2]);
	}

	return transform;
}

int32_t GLTFLoader::GetNodeIndex(cgltf_node* target, cgltf_node* nodes, uint32_t numModes)
{
	if (target == nullptr)
	{
		return -1;
	}

	for (uint32_t index = 0; index < numModes; ++index)
	{
		if (target == &nodes[index])
		{
			return static_cast<int32_t>(index);
		}
	}

	return -1;
}

void GLTFLoader::GetScalarValues(std::vector<float>& outScalars, uint32_t componentCount, const cgltf_accessor* accessor)
{
	outScalars.resize(accessor->count * componentCount);

	for (cgltf_size index = 0; index < accessor->count; ++index)
	{
		cgltf_accessor_read_float(accessor, index, &outScalars[index * componentCount], componentCount);
	}
}

void GLTFLoader::GetMeshFromAttribute(MeshData& outMesh, cgltf_attribute* attribute, cgltf_skin* skin, cgltf_node* nodes, uint32_t numNodes)
{
	cgltf_attribute_type type = attribute->type;
	cgltf_accessor* accessor = attribute->data;

	uint32_t component = 0;
	if (accessor->type == cgltf_type_vec2)
	{
		component = 2;
	}
	else if (accessor->type == cgltf_type_vec3)
	{
		component = 3;
	}
	else if (accessor->type == cgltf_type_vec4)
	{
		component = 4;
	}

	std::vector<float> values;
	GetScalarValues(values, component, accessor);

	uint32_t accessorCount = static_cast<uint32_t>(accessor->count);
	for (int32_t index = 0; index < accessorCount; ++index)
	{
		int32_t offset = index * component;

		switch (type)
		{
		case cgltf_attribute_type_position:
			outMesh.positions.push_back(Vec3f(values[offset + 0], values[offset + 1], values[offset + 2]));
			break;

		case cgltf_attribute_type_normal:
		{
			Vec3f normal = Vec3f(values[offset + 0], values[offset + 1], values[offset + 2]);
			if (Vec3f::LengthSq(normal) < Epsilon)
			{
				normal = Vec3f(0.0f, 1.0f, 0.0f);
			}
			outMesh.normals.push_back(Vec3f::Normalize(normal));
		}
		break;

		case cgltf_attribute_type_texcoord:
			outMesh.texcoords.push_back(Vec2f(values[offset + 0], values[offset + 1]));
			break;

		case cgltf_attribute_type_weights:
			outMesh.weights.push_back(Vec4f(values[offset + 0], values[offset + 1], values[offset + 2], values[offset + 3]));
			break;

		case cgltf_attribute_type_joints:
		{
			Vec4i joints(
				static_cast<int32_t>(values[offset + 0] + 0.5f),
				static_cast<int32_t>(values[offset + 1] + 0.5f),
				static_cast<int32_t>(values[offset + 2] + 0.5f),
				static_cast<int32_t>(values[offset + 3] + 0.5f)
			);

			joints.x = MathModule::Max(0, GetNodeIndex(skin->joints[joints.x], nodes, numNodes));
			joints.y = MathModule::Max(0, GetNodeIndex(skin->joints[joints.y], nodes, numNodes));
			joints.z = MathModule::Max(0, GetNodeIndex(skin->joints[joints.z], nodes, numNodes));
			joints.w = MathModule::Max(0, GetNodeIndex(skin->joints[joints.w], nodes, numNodes));

			outMesh.joints.push_back(joints);
		}
		break;
		}
	}
}

#pragma warning(pop)