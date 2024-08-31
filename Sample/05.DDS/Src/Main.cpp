#include <cstdint>
#include <Windows.h>

#include <glad/glad.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "IApp.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Texture2D.h"
#include "VertexBuffer.h"

struct Vertex
{
	GameMath::Vec3f position;
	GameMath::Vec2f uv;
};

class DemoApp : public IApp
{
public:
	enum class Format
	{
		RGBA = 0x00,
		DXT1 = 0x01,
		DXT3 = 0x02,
		DXT5 = 0x03,
	};

	struct Chunk
	{
		uint32_t vao = 0;
		VertexBuffer* vertexBuffer = nullptr;
		Texture2D* texture = nullptr;
	};

public:
	DemoApp() : IApp("05.DDS", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		shader_ = ResourceManager::Get().Create<Shader>("GameMaker\\Sample\\05.DDS\\Res\\Shader.vert", "GameMaker\\Sample\\05.DDS\\Res\\Shader.frag");

		/** RGBA */
		Chunk chunk;

		std::array<Vertex, 4> vertices =
		{
			Vertex{ GameMath::Vec3f(-1.0f, +0.0f, 0.0f), GameMath::Vec2f(0.0f, 0.0f) },
			Vertex{ GameMath::Vec3f(+0.0f, +0.0f, 0.0f), GameMath::Vec2f(1.0f, 0.0f) },
			Vertex{ GameMath::Vec3f(+0.0f, +1.0f, 0.0f), GameMath::Vec2f(1.0f, 1.0f) },
			Vertex{ GameMath::Vec3f(-1.0f, +1.0f, 0.0f), GameMath::Vec2f(0.0f, 1.0f) },
		};

		uint32_t stride = static_cast<uint32_t>(sizeof(Vertex));
		uint32_t byteSize = static_cast<uint32_t>(vertices.size()) * stride;
		chunk.vertexBuffer = ResourceManager::Get().Create<VertexBuffer>(vertices.data(), byteSize, VertexBuffer::Usage::STATIC);

		GL_CHECK(glGenVertexArrays(1, &chunk.vao));
		GL_CHECK(glBindVertexArray(chunk.vao));
		{
			chunk.vertexBuffer->Bind();

			GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, position))));
			GL_CHECK(glEnableVertexAttribArray(0));

			GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, uv))));
			GL_CHECK(glEnableVertexAttribArray(1));

			chunk.vertexBuffer->Unbind();
		}
		GL_CHECK(glBindVertexArray(0));

		chunk.texture = ResourceManager::Get().Create<Texture2D>("GameMaker\\Sample\\05.DDS\\Res\\RGBA\\awesomeface_32.dds", Texture2D::Filter::LINEAR);
		chunks_.insert({ Format::RGBA, chunk });

		/** DXT1 */
		vertices =
		{
			Vertex{ GameMath::Vec3f(+0.0f, +0.0f, 0.0f), GameMath::Vec2f(0.0f, 0.0f) },
			Vertex{ GameMath::Vec3f(+1.0f, +0.0f, 0.0f), GameMath::Vec2f(1.0f, 0.0f) },
			Vertex{ GameMath::Vec3f(+1.0f, +1.0f, 0.0f), GameMath::Vec2f(1.0f, 1.0f) },
			Vertex{ GameMath::Vec3f(+0.0f, +1.0f, 0.0f), GameMath::Vec2f(0.0f, 1.0f) },
		};

		chunk.vertexBuffer = ResourceManager::Get().Create<VertexBuffer>(vertices.data(), byteSize, VertexBuffer::Usage::STATIC);

		GL_CHECK(glGenVertexArrays(1, &chunk.vao));
		GL_CHECK(glBindVertexArray(chunk.vao));
		{
			chunk.vertexBuffer->Bind();

			GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, position))));
			GL_CHECK(glEnableVertexAttribArray(0));

			GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, uv))));
			GL_CHECK(glEnableVertexAttribArray(1));

			chunk.vertexBuffer->Unbind();
		}
		GL_CHECK(glBindVertexArray(0));

		chunk.texture = ResourceManager::Get().Create<Texture2D>("GameMaker\\Sample\\05.DDS\\Res\\DXT1\\awesomeface_32.dds", Texture2D::Filter::LINEAR);
		chunks_.insert({ Format::DXT1, chunk });

		/** DXT3 */
		vertices =
		{
			Vertex{ GameMath::Vec3f(-1.0f, -1.0f, 0.0f), GameMath::Vec2f(0.0f, 0.0f) },
			Vertex{ GameMath::Vec3f(+0.0f, -1.0f, 0.0f), GameMath::Vec2f(1.0f, 0.0f) },
			Vertex{ GameMath::Vec3f(+0.0f, +0.0f, 0.0f), GameMath::Vec2f(1.0f, 1.0f) },
			Vertex{ GameMath::Vec3f(-1.0f, +0.0f, 0.0f), GameMath::Vec2f(0.0f, 1.0f) },
		};

		chunk.vertexBuffer = ResourceManager::Get().Create<VertexBuffer>(vertices.data(), byteSize, VertexBuffer::Usage::STATIC);

		GL_CHECK(glGenVertexArrays(1, &chunk.vao));
		GL_CHECK(glBindVertexArray(chunk.vao));
		{
			chunk.vertexBuffer->Bind();

			GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, position))));
			GL_CHECK(glEnableVertexAttribArray(0));

			GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, uv))));
			GL_CHECK(glEnableVertexAttribArray(1));

			chunk.vertexBuffer->Unbind();
		}
		GL_CHECK(glBindVertexArray(0));

		chunk.texture = ResourceManager::Get().Create<Texture2D>("GameMaker\\Sample\\05.DDS\\Res\\DXT3\\awesomeface_32.dds", Texture2D::Filter::LINEAR);
		chunks_.insert({ Format::DXT3, chunk });

		/** DXT5 */
		vertices =
		{
			Vertex{ GameMath::Vec3f(+0.0f, -1.0f, 0.0f), GameMath::Vec2f(0.0f, 0.0f) },
			Vertex{ GameMath::Vec3f(+1.0f, -1.0f, 0.0f), GameMath::Vec2f(1.0f, 0.0f) },
			Vertex{ GameMath::Vec3f(+1.0f, +0.0f, 0.0f), GameMath::Vec2f(1.0f, 1.0f) },
			Vertex{ GameMath::Vec3f(+0.0f, +0.0f, 0.0f), GameMath::Vec2f(0.0f, 1.0f) },
		};

		chunk.vertexBuffer = ResourceManager::Get().Create<VertexBuffer>(vertices.data(), byteSize, VertexBuffer::Usage::STATIC);

		GL_CHECK(glGenVertexArrays(1, &chunk.vao));
		GL_CHECK(glBindVertexArray(chunk.vao));
		{
			chunk.vertexBuffer->Bind();

			GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, position))));
			GL_CHECK(glEnableVertexAttribArray(0));

			GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, uv))));
			GL_CHECK(glEnableVertexAttribArray(1));

			chunk.vertexBuffer->Unbind();
		}
		GL_CHECK(glBindVertexArray(0));

		chunk.texture = ResourceManager::Get().Create<Texture2D>("GameMaker\\Sample\\05.DDS\\Res\\DXT5\\awesomeface_32.dds", Texture2D::Filter::LINEAR);
		chunks_.insert({ Format::DXT5, chunk });
	}

	virtual void Shutdown() override
	{
		for (auto& chunk : chunks_)
		{
			GL_CHECK(glDeleteVertexArrays(1, &chunk.second.vao));

			ResourceManager::Get().Destroy(chunk.second.vertexBuffer);
			ResourceManager::Get().Destroy(chunk.second.texture);
		}

		ResourceManager::Get().Destroy(shader_);
	}

	virtual void Run() override
	{
		/** 알파 블랜딩 활성화. */
		SetAlphaBlendMode(true);

		RunLoop(
			[&](float deltaSeconds)
			{
				BeginFrame(1.0f, 1.0f, 1.0f, 1.0f);

				shader_->Bind();
				{
					for (auto& chunk : chunks_)
					{
						chunk.second.texture->Active(0);
						GL_CHECK(glBindVertexArray(chunk.second.vao));
						GL_CHECK(glDrawArrays(static_cast<GLenum>(EDrawMode::TRIANGLE_FAN), 0, 4));
						GL_CHECK(glBindVertexArray(0));
					}
				}
				shader_->Unbind();

				EndFrame();
			}
		);
	}

private:
	Shader* shader_ = nullptr;
	std::map<Format, Chunk> chunks_;
};

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::unique_ptr<IApp> app = std::make_unique<DemoApp>();
	app->Startup();
	app->Run();
	app->Shutdown();
	return 0;
}