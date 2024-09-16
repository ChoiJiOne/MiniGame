#include <cstdint>
#include <Windows.h>
#include <tuple>

#include <glad/glad.h>
#include <imgui.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "IApp.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "VertexBuffer.h"

struct Vertex
{
	GameMath::Vec3f position;
	GameMath::Vec4f color;
};

class DemoApp : public IApp
{
public:
	DemoApp() : IApp("15.GammaCorrection", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		shader_ = ResourceManager::Get().Create<Shader>(
			"GameMaker\\Sample\\15.GammaCorrection\\Res\\Shader.vert", 
			"GameMaker\\Sample\\15.GammaCorrection\\Res\\Shader.frag"
		);

		{ // Object 1.
			std::array<Vertex, 4> vertices =
			{
				Vertex{GameMath::Vec3f(-0.9f, +0.2f, 0.0f), GameMath::Vec4f(0.0f, 0.0f, 0.0f, 1.0f) },
				Vertex{GameMath::Vec3f(+0.9f, +0.2f, 0.0f), GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f) },
				Vertex{GameMath::Vec3f(+0.9f, +0.4f, 0.0f), GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f) },
				Vertex{GameMath::Vec3f(-0.9f, +0.4f, 0.0f), GameMath::Vec4f(0.0f, 0.0f, 0.0f, 1.0f) },
			};

			uint32_t stride = static_cast<uint32_t>(sizeof(Vertex));
			uint32_t byteSize = static_cast<uint32_t>(vertices.size()) * stride;
			VertexBuffer* vertexBuffer = ResourceManager::Get().Create<VertexBuffer>(vertices.data(), byteSize, VertexBuffer::Usage::STATIC);

			uint32_t vao;
			GL_CHECK(glGenVertexArrays(1, &vao));
			GL_CHECK(glBindVertexArray(vao));
			{
				vertexBuffer->Bind();

				GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, position))));
				GL_CHECK(glEnableVertexAttribArray(0));

				GL_CHECK(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, color))));
				GL_CHECK(glEnableVertexAttribArray(1));

				vertexBuffer->Unbind();
			}
			GL_CHECK(glBindVertexArray(0));

			objects_.push_back(std::pair{ vao, vertexBuffer });
		}

		{ // Object 2.
			std::array<Vertex, 4> vertices =
			{
				Vertex{GameMath::Vec3f(-0.9f, -0.4f, 0.0f), GameMath::Vec4f(0.0f, 0.0f, 0.0f, 1.0f) },
				Vertex{GameMath::Vec3f(+0.9f, -0.4f, 0.0f), GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f) },
				Vertex{GameMath::Vec3f(+0.9f, -0.2f, 0.0f), GameMath::Vec4f(1.0f, 1.0f, 1.0f, 1.0f) },
				Vertex{GameMath::Vec3f(-0.9f, -0.2f, 0.0f), GameMath::Vec4f(0.0f, 0.0f, 0.0f, 1.0f) },
			};

			uint32_t stride = static_cast<uint32_t>(sizeof(Vertex));
			uint32_t byteSize = static_cast<uint32_t>(vertices.size()) * stride;
			VertexBuffer* vertexBuffer = ResourceManager::Get().Create<VertexBuffer>(vertices.data(), byteSize, VertexBuffer::Usage::STATIC);

			uint32_t vao;
			GL_CHECK(glGenVertexArrays(1, &vao));
			GL_CHECK(glBindVertexArray(vao));
			{
				vertexBuffer->Bind();

				GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, position))));
				GL_CHECK(glEnableVertexAttribArray(0));

				GL_CHECK(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, color))));
				GL_CHECK(glEnableVertexAttribArray(1));

				vertexBuffer->Unbind();
			}
			GL_CHECK(glBindVertexArray(0));

			objects_.push_back(std::pair{ vao, vertexBuffer });
		}
	}

	virtual void Shutdown() override
	{
		for (auto& object : objects_)
		{
			GL_CHECK(glDeleteVertexArrays(1, &object.first));
			ResourceManager::Get().Destroy(object.second);
		}

		ResourceManager::Get().Destroy(shader_);
		shader_ = nullptr;
	}

	virtual void Run() override
	{
		RunLoop(
			[&](float deltaSeconds)
			{
				ImGui::Begin("GAMMA", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
				ImGui::SetWindowSize(ImVec2(400.0f, 55.0f));
				ImGui::SliderFloat("Gamma", &gamma_, 1.0f, 3.0f);
				ImGui::End();

				BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
				shader_->Bind();
				{
					shader_->SetUniform("gamma", 1.0f / gamma_);
					for (auto& object : objects_)
					{
						shader_->SetUniform("bEnableGammaCorrection", bbEnableGammaCorrection_);
						bbEnableGammaCorrection_ = !bbEnableGammaCorrection_;

						GL_CHECK(glBindVertexArray(object.first));
						GL_CHECK(glDrawArrays(static_cast<GLenum>(DrawMode::TRIANGLE_FAN), 0, 4));
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
	float gamma_ = 2.2f;
	bool bbEnableGammaCorrection_ = false;

	std::vector<std::pair<uint32_t, VertexBuffer*>> objects_;
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