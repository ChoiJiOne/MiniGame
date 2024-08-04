#include <array>
#include <cstdint>
#include <vector>
#include <Windows.h>

#include <glad/glad.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "GameEngine.h"
#include "GameMath.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "VertexBuffer.h"

struct Vertex
{
	GameMaker::Vec3f position;
	GameMaker::Vec4f color;
};

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameMaker::GameEngine::Init({ "02.Triangle", 100, 100, 800, 600, false, false });

	std::array<Vertex, 3> vertices =
	{
		Vertex{GameMaker::Vec3f(-0.5f, -0.5f, 0.0f), GameMaker::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) },
		Vertex{GameMaker::Vec3f(+0.5f, -0.5f, 0.0f), GameMaker::Vec4f(0.0f, 1.0f, 0.0f, 1.0f) },
		Vertex{GameMaker::Vec3f(+0.0f, +0.5f, 0.0f), GameMaker::Vec4f(0.0f, 0.0f, 1.0f, 1.0f) },
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

		GL_FAILED(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, color))));
		GL_FAILED(glEnableVertexAttribArray(1));

		vertexBuffer->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));

	GameMaker::Shader* shader = GameMaker::ResourceManager::Get().Create<GameMaker::Shader>(
		"GameMaker/Sample/02.Triangle/Res/Shader.vert",
		"GameMaker/Sample/02.Triangle/Res/Shader.frag"
	);

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds)
		{
			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			shader->Bind();
			{
				GL_FAILED(glBindVertexArray(vao));
				GameMaker::RenderManager::Get().ExecuteDrawVertex(3, GameMaker::EDrawMode::TRIANGLES);
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