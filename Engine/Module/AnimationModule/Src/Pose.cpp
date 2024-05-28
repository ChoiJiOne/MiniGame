#include "Pose.h"

Pose& Pose::operator=(Pose&& instance) noexcept
{
	if (this == &instance) return *this;

	if (parents_.size() != instance.parents_.size())
	{
		parents_.resize(instance.parents_.size());
	}

	if (joints_.size() != instance.joints_.size())
	{
		joints_.resize(instance.joints_.size());
	}

	if (parents_.size() != 0)
	{
		std::memcpy(parents_.data(), instance.parents_.data(), static_cast<uint32_t>(sizeof(int32_t)) * parents_.size());
	}

	if (joints_.size() != 0)
	{
		std::memcpy(joints_.data(), instance.joints_.data(), static_cast<uint32_t>(sizeof(Transform)) * joints_.size());
	}

	return *this;
}

Pose& Pose::operator=(const Pose& instance) noexcept
{
	if (this == &instance) return *this;

	if (parents_.size() != instance.parents_.size())
	{
		parents_.resize(instance.parents_.size());
	}

	if (joints_.size() != instance.joints_.size())
	{
		joints_.resize(instance.joints_.size());
	}

	if (parents_.size() != 0)
	{
		std::memcpy(parents_.data(), instance.parents_.data(), static_cast<uint32_t>(sizeof(int32_t)) * parents_.size());
	}

	if (joints_.size() != 0)
	{
		std::memcpy(joints_.data(), instance.joints_.data(), static_cast<uint32_t>(sizeof(Transform)) * joints_.size());
	}

	return *this;
}

bool Pose::operator==(Pose&& instance) noexcept
{
	if (joints_.size() != instance.joints_.size())
	{
		return false;
	}

	if (parents_.size() != instance.parents_.size())
	{
		return false;
	}

	uint32_t size = static_cast<uint32_t>(joints_.size());
	for (uint32_t index = 0; index < size; ++index)
	{
		int32_t thisParent = parents_[index];
		int32_t otherParent = instance.parents_[index];
		if (thisParent != otherParent)
		{
			return false;
		}

		Transform thisLocal = joints_[index];
		Transform otherLocal = instance.joints_[index];
		if (thisLocal != otherLocal)
		{
			return false;
		}
	}

	return true;
}

bool Pose::operator==(const Pose& instance)
{
	if (joints_.size() != instance.joints_.size())
	{
		return false;
	}

	if (parents_.size() != instance.parents_.size())
	{
		return false;
	}

	uint32_t size = static_cast<uint32_t>(joints_.size());
	for (uint32_t index = 0; index < size; ++index)
	{
		int32_t thisParent = parents_[index];
		int32_t otherParent = instance.parents_[index];
		if (thisParent != otherParent)
		{
			return false;
		}

		Transform thisLocal = joints_[index];
		Transform otherLocal = instance.joints_[index];
		if (thisLocal != otherLocal)
		{
			return false;
		}
	}

	return true;
}

bool Pose::operator!=(Pose&& instance) noexcept
{
	return !(*this == instance);
}

bool Pose::operator!=(const Pose& instance)
{
	return !(*this == instance);
}

void Pose::Resize(uint32_t size)
{
	parents_.resize(size);
	joints_.resize(size);
}

Transform Pose::GetLocalTransform(uint32_t index)
{
	return joints_[index];
}

void Pose::SetLocalTransform(uint32_t index, const Transform& transform)
{
	joints_[index] = transform;
}

Transform Pose::GetGlobalTransform(uint32_t index)
{
	Transform result = joints_[index];

	for (int32_t parent = parents_[index]; parent >= 0; parent = parents_[parent])
	{
		result = Transform::Combine(joints_[parent], result);
	}

	return result;
}

Transform Pose::operator[](uint32_t index)
{
	return GetGlobalTransform(index);
}

void Pose::GetMatrixPalette(std::vector<Mat4x4>& outMatrixPalette)
{
	uint32_t size = static_cast<uint32_t>(joints_.size());
	if (outMatrixPalette.size() != size)
	{
		outMatrixPalette.resize(size);
	}

	for (uint32_t index = 0; index < size; ++index)
	{
		Transform transform = GetGlobalTransform(index);
		outMatrixPalette[index] = Transform::ToMat(transform);
	}
}

bool Pose::IsInHierarchy(Pose& pose, uint32_t parent, uint32_t search)
{
	if (search == parent)
	{
		return true;
	}

	int32_t p = pose.GetParent(search);
	while (p >= 0)
	{
		if (p == static_cast<int32_t>(parent))
		{
			return true;
		}
		p = pose.GetParent(p);
	}

	return false;
}

void Pose::Blend(Pose& output, Pose& start, Pose& end, float t, int32_t blendRoot)
{
	uint32_t numJoints = output.Size();
	for (uint32_t index = 0; index < numJoints; ++index)
	{
		if (blendRoot >= 0)
		{
			if (!IsInHierarchy(output, static_cast<uint32_t>(blendRoot), index))
			{
				continue;
			}
		}

		output.SetLocalTransform(index, Transform::Mix(start.GetLocalTransform(index), end.GetLocalTransform(index), t));
	}
}
