#include "RenderModule.h"
#include "Skybox.h"

#include "Background.h"

Background::Background()
{
	static Skybox* skybox = nullptr;
	if (!skybox)
	{
		skybox = RenderModule::CreateResource<Skybox>(
			"Resource/Texture/Skybox/Space_Right.png",
			"Resource/Texture/Skybox/Space_Left.png",
			"Resource/Texture/Skybox/Space_Top.png",
			"Resource/Texture/Skybox/Space_Bottom.png",
			"Resource/Texture/Skybox/Space_Front.png",
			"Resource/Texture/Skybox/Space_Back.png"
		);
	}

	skybox_ = skybox;
	bIsInitialized_ = true;
}

Background::~Background()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Background::Tick(float deltaSeconds)
{
}

void Background::Release()
{
	if (bIsInitialized_)
	{
		bIsInitialized_ = false;
	}
}
