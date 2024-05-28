#include "Assertion.h"
#include "MathModule.h"

#include "Track.h"

template Track<float, 1>;
template Track<Vec3f, 3>;
template Track<Quat, 4>;

inline float Interpolation(float a, float b, float t)
{
	return a + (b - a) * t;
}

inline Vec3f Interpolation(const Vec3f& a, const Vec3f& b, float t)
{
	return Vec3f::Lerp(a, b, t);
}

inline Quat Interpolation(const Quat& a, const Quat& b, float t)
{
	Quat result = Quat::Lerp(a, b, t);

	if (Quat::Dot(a, b) < 0.0f)
	{
		result = Quat::Lerp(a, -b, t);
	}

	return Quat::Normalize(result);
}

inline float AdjustHermiteResult(float value)
{
	return value; // nothing...
}

inline Vec3f AdjustHermiteResult(const Vec3f& value)
{
	return value; // nothing...
}

inline Quat AdjustHermiteResult(const Quat& value)
{
	return Quat::Normalize(value);
}

inline void Neighborhood(const float& a, float& b)
{
	// nothing...
}

inline void Neighborhood(const Vec3f& a, Vec3f& b)
{
	// nothing...
}

inline void Neighborhood(const Quat& a, Quat& b)
{
	if (Quat::Dot(a, b) < 0.0f)
	{
		b = -b;
	}
}

template<typename T, uint32_t N>
Keyframe<N>& Track<T, N>::operator[](uint32_t index)
{
	return keyframes_[index];
}

template<typename T, uint32_t N>
T Track<T, N>::Sample(float time, bool bIsLooping)
{
	T sample;

	switch (interpolation_)
	{
	case EInterpolation::CONSTANT:
		sample = SampleConstant(time, bIsLooping);
		break;

	case EInterpolation::LINEAR:
		sample = SampleLinear(time, bIsLooping);
		break;

	case EInterpolation::CUBIC:
		sample = SampleCubic(time, bIsLooping);
		break;
	}

	return sample;
}

template<typename T, uint32_t N>
T Track<T, N>::SampleConstant(float time, bool bIsLooping)
{
	int32_t index = FrameIndex(time, bIsLooping);

	if (index < 0 || index >= static_cast<int32_t>(keyframes_.size()))
	{
		return T();
	}

	return Cast(&keyframes_[index].value[0]);
}

template<typename T, uint32_t N>
T Track<T, N>::SampleLinear(float time, bool bIsLooping)
{
	int32_t currFrame = FrameIndex(time, bIsLooping);
	if (currFrame < 0 || currFrame >= static_cast<int32_t>(keyframes_.size() - 1))
	{
		return T();
	}

	int32_t nextFrame = currFrame + 1;
	float trackTime = AdjustTimeToFitTrack(time, bIsLooping);
	float frameDelta = keyframes_[nextFrame].time - keyframes_[currFrame].time;
	if (frameDelta <= 0.0f)
	{
		return T();
	}

	float t = (trackTime - keyframes_[currFrame].time) / frameDelta;

	T start = Cast(&keyframes_[currFrame].value[0]);
	T end = Cast(&keyframes_[nextFrame].value[0]);

	return Interpolation(start, end, t);
}

template<typename T, uint32_t N>
T Track<T, N>::SampleCubic(float time, bool bIsLooping)
{
	int32_t currFrame = FrameIndex(time, bIsLooping);
	if (currFrame < 0 || currFrame >= static_cast<int32_t>(keyframes_.size() - 1))
	{
		return T();
	}

	int32_t nextFrame = currFrame + 1;
	float trackTime = AdjustTimeToFitTrack(time, bIsLooping);
	float frameDelta = keyframes_[nextFrame].time - keyframes_[currFrame].time;
	if (frameDelta <= 0.0f)
	{
		return T();
	}

	float t = (trackTime - keyframes_[currFrame].time) / frameDelta;

	T point1 = Cast(&keyframes_[currFrame].value[0]);
	T slope1;
	std::memcpy(&slope1, keyframes_[currFrame].out, N * sizeof(float));
	slope1 = slope1 * frameDelta;

	T point2 = Cast(&keyframes_[nextFrame].value[0]);
	T slope2;
	std::memcpy(&slope2, keyframes_[nextFrame].in, N * sizeof(float));
	slope2 = slope2 * frameDelta;

	return Hermite(t, point1, slope1, point2, slope2);
}

template<typename T, uint32_t N>
T Track<T, N>::Hermite(float time, const T& point1, const T& slope1, const T& point2, const T& slope2)
{
	float t = time;
	float tt = t * t;
	float ttt = tt * t;

	T p1 = point1;
	T s1 = slope1;
	T p2 = point2;
	T s2 = slope2;
	Neighborhood(p1, p2);

	float h1 = 2.0f * ttt - 3.0f * tt + 1.0f;
	float h2 = -2.0f * ttt + 3.0f * tt;
	float h3 = ttt - 2.0f * tt + t;
	float h4 = ttt - tt;

	T result = p1 * h1 + p2 * h2 + s1 * h3 + s2 * h4;
	return AdjustHermiteResult(result);
}

template<typename T, uint32_t N>
int32_t Track<T, N>::FrameIndex(float time, bool bIsLooping)
{
	uint32_t size = static_cast<uint32_t>(keyframes_.size());

	if (size <= 1)
	{
		return -1;
	}

	if (bIsLooping)
	{
		float startTime = keyframes_[0].time;
		float endTime = keyframes_[size - 1].time;
		float duration = endTime - startTime;

		time = MathModule::Fmod(time - startTime, endTime - startTime);
		if (time < 0.0f)
		{
			time += (endTime - startTime);
		}

		time += startTime;
	}
	else
	{
		if (time <= keyframes_[0].time)
		{
			return 0;
		}

		if (time >= keyframes_[size - 2].time)
		{
			return static_cast<int32_t>(size - 2);
		}
	}

	for (int32_t index = static_cast<int32_t>(size) - 1; index >= 0; --index)
	{
		if (time >= keyframes_[index].time)
		{
			return index;
		}
	}

	return -1;
}

template<typename T, uint32_t N>
float Track<T, N>::AdjustTimeToFitTrack(float time, bool bIsLooping)
{
	uint32_t size = static_cast<uint32_t>(keyframes_.size());
	if (size <= 1)
	{
		return 0.0f;
	}

	float startTime = keyframes_[0].time;
	float endTime = keyframes_[size - 1].time;
	float duration = endTime - startTime;
	if (duration <= 0.0f)
	{
		return 0.0f;
	}

	if (bIsLooping)
	{
		time = MathModule::Fmod(time - startTime, endTime - startTime);
		if (time < 0.0f)
		{
			time += (endTime - startTime);
		}

		time += startTime;
	}
	else
	{
		if (time <= keyframes_[0].time)
		{
			time = startTime;
		}

		if (time >= keyframes_[size - 1].time)
		{
			time = endTime;
		}
	}

	return time;
}

template<>
float Track<float, 1>::Cast(float* value)
{
	return value[0];
}

template<>
Vec3f Track<Vec3f, 3>::Cast(float* value)
{
	return Vec3f(value[0], value[1], value[2]);
}

template<>
Quat Track<Quat, 4>::Cast(float* value)
{
	Quat q = Quat(value[0], value[1], value[2], value[3]);
	return Quat::Normalize(q);
}