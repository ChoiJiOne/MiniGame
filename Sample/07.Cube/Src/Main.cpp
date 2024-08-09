#include <array>
#include <cstdint>
#include <vector>
#include <Windows.h>

#include <glad/glad.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "EntityManager.h"
#include "FreeEulerCamera3D.h"
#include "GameEngine.h"
#include "GameMath.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Renderer3D.h"
#include "Shader.h"
#include "Texture2D.h"
#include "VertexBuffer.h"

struct Vertex
{
	GameMaker::Vec3f position;
	GameMaker::Vec2f uv;
};

void DrawGrid(GameMaker::Renderer3D* renderer)
{
	float minX = -50.0f;
	float maxX = +50.0f;
	float minY = -50.0f;
	float maxY = +50.0f;
	float minZ = -50.0f;
	float maxZ = +50.0f;
	float stride = 1.0f;

	GameMaker::Vec4f color;
	for (float x = minX; x <= maxX; x += stride)
	{
		color = (x == 0.0f) ? GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
		renderer->DrawLine(GameMaker::Vec3f(x, 0.0f, minZ), GameMaker::Vec3f(x, 0.0f, maxZ), color);
	}

	for (float z = minZ; z <= maxZ; z += stride)
	{
		color = (z == 0.0f) ? GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : GameMaker::Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
		renderer->DrawLine(GameMaker::Vec3f(minX, 0.0f, z), GameMaker::Vec3f(maxX, 0.0f, z), color);
	}

	renderer->DrawLine(GameMaker::Vec3f(0.0f, minY, 0.0f), GameMaker::Vec3f(0.0f, maxY, 0.0f), GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
}

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameMaker::GameEngine::Init({ "07.Cube", 100, 100, 800, 600, false, false });

	GameMaker::Renderer3D* renderer = GameMaker::RenderManager::Get().GetRenderer3D();
	GameMaker::FreeEulerCamera3D* camera = GameMaker::EntityManager::Get().Create<GameMaker::FreeEulerCamera3D>(GameMaker::Vec3f(3.0f, 3.0f, 3.0f), -GameMaker::ToRadian(135.0f), -GameMaker::PI / 6.0f, GameMaker::PI_DIV_4, 0.01f, 100.0f);
	
	std::array<Vertex, 36> vertices =
	{
		Vertex{ GameMaker::Vec3f(-0.5f, -0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f, -0.5f, -0.5f),  GameMaker::Vec2f(1.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f,  0.5f, -0.5f),  GameMaker::Vec2f(1.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f,  0.5f, -0.5f),  GameMaker::Vec2f(1.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f,  0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f, -0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 0.0f) },

		Vertex{ GameMaker::Vec3f(-0.5f, -0.5f,  0.5f),  GameMaker::Vec2f(0.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f, -0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f,  0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f,  0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f,  0.5f,  0.5f),  GameMaker::Vec2f(0.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f, -0.5f,  0.5f),  GameMaker::Vec2f(0.0f, 0.0f) },

		Vertex{ GameMaker::Vec3f(-0.5f,  0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f,  0.5f, -0.5f),  GameMaker::Vec2f(1.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f, -0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f, -0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f, -0.5f,  0.5f),  GameMaker::Vec2f(0.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f,  0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 0.0f) },

		Vertex{ GameMaker::Vec3f( 0.5f,  0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f,  0.5f, -0.5f),  GameMaker::Vec2f(1.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f, -0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f, -0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f, -0.5f,  0.5f),  GameMaker::Vec2f(0.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f,  0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 0.0f) },

		Vertex{ GameMaker::Vec3f(-0.5f, -0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f, -0.5f, -0.5f),  GameMaker::Vec2f(1.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f, -0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f, -0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f, -0.5f,  0.5f),  GameMaker::Vec2f(0.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f, -0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 1.0f) },

		Vertex{ GameMaker::Vec3f(-0.5f,  0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f,  0.5f, -0.5f),  GameMaker::Vec2f(1.0f, 1.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f,  0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f( 0.5f,  0.5f,  0.5f),  GameMaker::Vec2f(1.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f,  0.5f,  0.5f),  GameMaker::Vec2f(0.0f, 0.0f) },
		Vertex{ GameMaker::Vec3f(-0.5f,  0.5f, -0.5f),  GameMaker::Vec2f(0.0f, 1.0f) },
	};

	uint32_t stride = static_cast<uint32_t>(sizeof(Vertex));
	uint32_t byteSize = static_cast<uint32_t>(vertices.size()) * stride;
	GameMaker::VertexBuffer* vertexBuffer = GameMaker::ResourceManager::Get().Create<GameMaker::VertexBuffer>(vertices.data(), byteSize, GameMaker::VertexBuffer::EUsage::STATIC);

	uint32_t vao;
	GL_FAILED(glGenVertexArrays(1, &vao));
	GL_FAILED(glBindVertexArray(vao));
	{
		vertexBuffer->Bind();

		GL_FAILED(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, position))));
		GL_FAILED(glEnableVertexAttribArray(0));
		
		GL_FAILED(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, uv))));
		GL_FAILED(glEnableVertexAttribArray(1));

		vertexBuffer->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));

	GameMaker::Shader* shader = GameMaker::ResourceManager::Get().Create<GameMaker::Shader>(
		"GameMaker/Sample/07.Cube/Res/Shader.vert",
		"GameMaker/Sample/07.Cube/Res/Shader.frag"
	);

	GameMaker::Texture2D* texture = GameMaker::ResourceManager::Get().Create<GameMaker::Texture2D>(
		"GameMaker/Sample/07.Cube/Res/box.png", 
		GameMaker::Texture2D::EFilter::LINEAR,
		true
	);

	GameMaker::RenderManager::Get().SetCullFaceMode(false);

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds)
		{
			camera->Tick(deltaSeconds);

			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			renderer->Begin(camera);
			{
				DrawGrid(renderer);
			}
			renderer->End();

			shader->Bind();
			{
				texture->Active(0);

				shader->SetUniform("world", GameMaker::Mat4x4::Identity());
				shader->SetUniform("view", camera->GetView());
				shader->SetUniform("projection", camera->GetProjection());

				GL_FAILED(glBindVertexArray(vao));
				GameMaker::RenderManager::Get().ExecuteDrawVertex(static_cast<uint32_t>(vertices.size()), GameMaker::EDrawMode::TRIANGLES);
				GL_FAILED(glBindVertexArray(0));
			}
			shader->Unbind();

			GameMaker::RenderManager::Get().EndFrame();
		}
	);

	GL_FAILED(glDeleteVertexArrays(1, &vao));

	GameMaker::GameEngine::Shutdown();

	return 0;
}