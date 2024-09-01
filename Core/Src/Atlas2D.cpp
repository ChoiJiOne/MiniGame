#include <json/json.h>

#include "Assertion.h"
#include "Atlas2D.h"
#include "GameUtils.h"
#include "IApp.h"

Atlas2D::Atlas2D(const std::string& imagePath, const std::string& jsonPath, const Filter& filter)
	: Texture2D(imagePath, filter)
{
	std::vector<uint8_t> buffer = GameUtils::ReadFile(jsonPath);
	std::string jsonString(buffer.begin(), buffer.end());

	Json::Value root;
	Json::Reader reader;
	ASSERT(reader.parse(jsonString, root), "Failed to parse '%s' file.", jsonPath.c_str());

	for (const auto& key : root.getMemberNames())
	{
		const Json::Value& element = root[key];

		CHECK(element["x"].isInt() && !element["x"].isNull());
		CHECK(element["y"].isInt() && !element["y"].isNull());
		CHECK(element["w"].isInt() && !element["w"].isNull());
		CHECK(element["h"].isInt() && !element["h"].isNull());
		
		Block block;
		block.pos.x = element["x"].asInt();
		block.pos.y = element["y"].asInt();
		block.size.x = element["w"].asInt();
		block.size.y = element["h"].asInt();

		keys_.push_back(key.c_str());
		blocks_.insert({ key.c_str(), block });
	}
}

Atlas2D::~Atlas2D()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Atlas2D::Release()
{
	Texture2D::Release();
}

const Atlas2D::Block& Atlas2D::GetByName(const std::string& name) const
{
	auto it = blocks_.find(name);
	CHECK(it != blocks_.end());

	return it->second;
}