#include <chrono>
#include <random>

#include "MathModule.h"

bool MathModule::NearZero(float value, float epsilon)
{
	return (std::fabsf(value) <= epsilon);
}

float MathModule::ASin(float value)
{
	float x = fabsf(value);
	float omx = 1.0f - x;
	if (omx < 0.0f)
	{
		omx = 0.0f;
	}

	float root = sqrtf(omx);
	float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
	result *= root;

	if (value >= 0.0f)
	{
		return (PiDiv2 - result);
	}
	else
	{
		return (result - PiDiv2);
	}
}

float MathModule::ACos(float value)
{
	float x = fabsf(value);
	float omx = 1.0f - x;
	if (omx < 0.0f)
	{
		omx = 0.0f;
	}

	float root = sqrtf(omx);
	float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
	result *= root;

	if (value >= 0.0f)
	{
		return result;
	}
	else
	{
		return Pi - result;
	}
}

float MathModule::Sqrt(float x)
{
	return std::sqrtf(x);
}

float MathModule::Abs(float x)
{
	return std::fabsf(x);
}

float MathModule::Fmod(float x, float y)
{
	return std::fmodf(x, y);
}

int32_t MathModule::GenerateRandomInt(int32_t minValue, int32_t maxValue)
{
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_int_distribution<int32_t> distribution(Min<int32_t>(minValue, maxValue), Max<int32_t>(minValue, maxValue));

	return distribution(generator);
}

float MathModule::GenerateRandomFloat(float minValue, float maxValue)
{
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<float> distribution(Min<float>(minValue, maxValue), Max<float>(minValue, maxValue));

	return distribution(generator);
}