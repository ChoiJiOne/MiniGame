#include <array>

#include "GeometryRenderer2D.h"

#include "Character.h"
#include "Coin.h"
#include "MiniMap.h"

MiniMap::MiniMap(std::list<Coin*>& coins, Character* character, GeometryRenderer2D* renderer)
	: coins_(coins)
	, character_(character)
	, renderer_(renderer)
{
	center_ = Vec2f(80.0f, 80.0f);
	size_ = 150.0f;
	background_ = Vec4f(0.0f, 0.0f, 0.0f, 0.5f);
	coinColor_ = Vec4f(0.0f, 1.0f, 0.0f, 1.0f);
	characterColor_ = Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
	maxPosition_ = 10.0f;

	bIsInitialized_ = true;
}

MiniMap::~MiniMap()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void MiniMap::Tick(float deltaSeconds)
{
}

void MiniMap::Render()
{
	static const uint32_t MAX_POSITION = 200;
	std::array<Vec2f, MAX_POSITION> positions;
	
	uint32_t count = 0;
	for (const auto& coin : coins_)
	{
		if (coin)
		{
			positions[count++] = Convert3DTo2D(coin->GetTransform().position);
		}
	}

	Vec2f characterPosition2D = Convert3DTo2D(character_->GetTransform().position);

	renderer_->DrawRectangle2D(center_, size_, size_, 0.0f, background_);
	renderer_->DrawPoints2D(positions.data(), count, coinColor_, 5.0f);
	renderer_->DrawPoints2D(&characterPosition2D, 1, characterColor_, 5.0f);
}

void MiniMap::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}

Vec2f MiniMap::Convert3DTo2D(const Vec3f& position3D)
{
	float x = position3D.x;
	float y = position3D.z; // 2D 상의 y좌표 의미.

	x /= maxPosition_; // -1 <= x <= 1
	y /= maxPosition_; // -1 <= y <= 1

	return center_ + Vec2f(x, y) * 0.5f * size_;
}