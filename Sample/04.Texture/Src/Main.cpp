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
	Vec3f position;
	Vec2f uv;
};

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("04.Texture", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		std::array<Vertex, 4> vertices =
		{
			Vertex{ Vec3f(-0.5f, -0.5f, 0.0f), Vec2f(0.0f, 0.0f) },
			Vertex{ Vec3f(+0.5f, -0.5f, 0.0f), Vec2f(1.0f, 0.0f) },
			Vertex{ Vec3f(+0.5f, +0.5f, 0.0f), Vec2f(1.0f, 1.0f) },
			Vertex{ Vec3f(-0.5f, +0.5f, 0.0f), Vec2f(0.0f, 1.0f) },
		};

		uint32_t stride = static_cast<uint32_t>(sizeof(Vertex));
		uint32_t byteSize = static_cast<uint32_t>(vertices.size()) * stride;
		vertexBuffer_ = ResourceManager::Get().Create<VertexBuffer>(vertices.data(), byteSize, VertexBuffer::Usage::STATIC);

		GL_CHECK(glGenVertexArrays(1, &vao_));
		GL_CHECK(glBindVertexArray(vao_));
		{
			vertexBuffer_->Bind();

			GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, position))));
			GL_CHECK(glEnableVertexAttribArray(0));

			GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, uv))));
			GL_CHECK(glEnableVertexAttribArray(1));

			vertexBuffer_->Unbind();
		}
		GL_CHECK(glBindVertexArray(0));

		shader_ = ResourceManager::Get().Create<Shader>("GameMaker\\Sample\\04.Texture\\Res\\Shader.vert", "GameMaker\\Sample\\04.Texture\\Res\\Shader.frag");
		texture_ = ResourceManager::Get().Create<Texture2D>("GameMaker\\Sample\\04.Texture\\Res\\awesomeface_32.png", Texture2D::Filter::LINEAR);
	}

	virtual void Shutdown() override
	{
		GL_CHECK(glDeleteVertexArrays(1, &vao_));

		ResourceManager::Get().Destroy(vertexBuffer_);
		ResourceManager::Get().Destroy(shader_);
		ResourceManager::Get().Destroy(texture_);
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
					texture_->Active(0);

					GL_CHECK(glBindVertexArray(vao_));
					GL_CHECK(glDrawArrays(static_cast<GLenum>(DrawMode::TRIANGLE_FAN), 0, 4));
					GL_CHECK(glBindVertexArray(0));
				}
				shader_->Unbind();

				EndFrame();
			}
		);
	}

private:
	VertexBuffer* vertexBuffer_ = nullptr;
	uint32_t vao_ = 0;
	Shader* shader_ = nullptr;
	Texture2D* texture_ = nullptr;
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