#include "GameModule.h"
#include "MathModule.h"

#include "Coin.h"
#include "CoinSpawner.h"

CoinSpawner::CoinSpawner(std::list<Coin*>& coins)
	: coins_(coins)
{
	GenerateCoins();
	bIsInitialized_ = true;
}

CoinSpawner::~CoinSpawner()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void CoinSpawner::Tick(float deltaSeconds)
{
	if (coins_.empty())
	{
		maxCoin_++;
		GenerateCoins();
	}
}

void CoinSpawner::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}

void CoinSpawner::GenerateCoins()
{
	for (uint32_t count = 0; count < maxCoin_; ++count)
	{
		float x = MathModule::GenerateRandomFloat(-9.0f, 9.0f);
		float y = 0.5;
		float z = MathModule::GenerateRandomFloat(-9.0f, 9.0f);

		coins_.push_back(GameModule::CreateEntity<Coin>(Vec3f(x, y, z)));
	}
}