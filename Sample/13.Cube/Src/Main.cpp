#include <array>
#include <cstdint>
#include <Windows.h>

#include <glad/glad.h>
#include <imgui.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "Assertion.h"
#include "IApp.h"

#include "DebugDrawManager3D.h"
#include "EntityManager.h"
#include "FreeEulerCamera3D.h"
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
	DemoApp() : IApp("13.Cube", 100, 100, 800, 600, false, false) {}
	virtual ~DemoApp() {}

	DISALLOW_COPY_AND_ASSIGN(DemoApp);

	virtual void Startup() override
	{
		SetDepthMode(true);
		SetCullFaceMode(false);

		camera_ = EntityManager::Get().Create<FreeEulerCamera3D>(
			Vec3f(3.0f, 3.0f, 3.0f), 
			-GameMath::ToRadian(135.0f), 
			-PI / 6.0f, 
			PI_DIV_4, 
			0.01f, 
			100.0f
		);

		std::array<Vertex, 36> vertices =
		{
			Vertex{ Vec3f(-0.5f, -0.5f, -0.5f),  Vec2f(0.0f, 0.0f) },
			Vertex{ Vec3f( 0.5f, -0.5f, -0.5f),  Vec2f(1.0f, 0.0f) },
			Vertex{ Vec3f( 0.5f,  0.5f, -0.5f),  Vec2f(1.0f, 1.0f) },
			Vertex{ Vec3f( 0.5f,  0.5f, -0.5f),  Vec2f(1.0f, 1.0f) },
			Vertex{ Vec3f(-0.5f,  0.5f, -0.5f),  Vec2f(0.0f, 1.0f) },
			Vertex{ Vec3f(-0.5f, -0.5f, -0.5f),  Vec2f(0.0f, 0.0f) },

			Vertex{ Vec3f(-0.5f, -0.5f,  0.5f),  Vec2f(0.0f, 0.0f) },
			Vertex{ Vec3f( 0.5f, -0.5f,  0.5f),  Vec2f(1.0f, 0.0f) },
			Vertex{ Vec3f( 0.5f,  0.5f,  0.5f),  Vec2f(1.0f, 1.0f) },
			Vertex{ Vec3f( 0.5f,  0.5f,  0.5f),  Vec2f(1.0f, 1.0f) },
			Vertex{ Vec3f(-0.5f,  0.5f,  0.5f),  Vec2f(0.0f, 1.0f) },
			Vertex{ Vec3f(-0.5f, -0.5f,  0.5f),  Vec2f(0.0f, 0.0f) },

			Vertex{ Vec3f(-0.5f,  0.5f,  0.5f),  Vec2f(1.0f, 0.0f) },
			Vertex{ Vec3f(-0.5f,  0.5f, -0.5f),  Vec2f(1.0f, 1.0f) },
			Vertex{ Vec3f(-0.5f, -0.5f, -0.5f),  Vec2f(0.0f, 1.0f) },
			Vertex{ Vec3f(-0.5f, -0.5f, -0.5f),  Vec2f(0.0f, 1.0f) },
			Vertex{ Vec3f(-0.5f, -0.5f,  0.5f),  Vec2f(0.0f, 0.0f) },
			Vertex{ Vec3f(-0.5f,  0.5f,  0.5f),  Vec2f(1.0f, 0.0f) },

			Vertex{ Vec3f(0.5f,  0.5f,  0.5f),  Vec2f(1.0f, 0.0f) },
			Vertex{ Vec3f(0.5f,  0.5f, -0.5f),  Vec2f(1.0f, 1.0f) },
			Vertex{ Vec3f(0.5f, -0.5f, -0.5f),  Vec2f(0.0f, 1.0f) },
			Vertex{ Vec3f(0.5f, -0.5f, -0.5f),  Vec2f(0.0f, 1.0f) },
			Vertex{ Vec3f(0.5f, -0.5f,  0.5f),  Vec2f(0.0f, 0.0f) },
			Vertex{ Vec3f(0.5f,  0.5f,  0.5f),  Vec2f(1.0f, 0.0f) },

			Vertex{ Vec3f(-0.5f, -0.5f, -0.5f),  Vec2f(0.0f, 1.0f) },
			Vertex{ Vec3f( 0.5f, -0.5f, -0.5f),  Vec2f(1.0f, 1.0f) },
			Vertex{ Vec3f( 0.5f, -0.5f,  0.5f),  Vec2f(1.0f, 0.0f) },
			Vertex{ Vec3f( 0.5f, -0.5f,  0.5f),  Vec2f(1.0f, 0.0f) },
			Vertex{ Vec3f(-0.5f, -0.5f,  0.5f),  Vec2f(0.0f, 0.0f) },
			Vertex{ Vec3f(-0.5f, -0.5f, -0.5f),  Vec2f(0.0f, 1.0f) },

			Vertex{ Vec3f(-0.5f,  0.5f, -0.5f),  Vec2f(0.0f, 1.0f) },
			Vertex{ Vec3f( 0.5f,  0.5f, -0.5f),  Vec2f(1.0f, 1.0f) },
			Vertex{ Vec3f( 0.5f,  0.5f,  0.5f),  Vec2f(1.0f, 0.0f) },
			Vertex{ Vec3f( 0.5f,  0.5f,  0.5f),  Vec2f(1.0f, 0.0f) },
			Vertex{ Vec3f(-0.5f,  0.5f,  0.5f),  Vec2f(0.0f, 0.0f) },
			Vertex{ Vec3f(-0.5f,  0.5f, -0.5f),  Vec2f(0.0f, 1.0f) },
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

		texture_ = ResourceManager::Get().Create<Texture2D>("GameMaker/Sample/13.Cube/Res/Box.png", Texture2D::Filter::LINEAR);
		shader_ = ResourceManager::Get().Create<Shader>("GameMaker/Sample/13.Cube/Res/Shader.vert", "GameMaker/Sample/13.Cube/Res/Shader.frag");
	}

	virtual void Shutdown() override
	{
		ResourceManager::Get().Destroy(shader_);
		shader_ = nullptr;

		ResourceManager::Get().Destroy(texture_);
		texture_ = nullptr;

		ResourceManager::Get().Destroy(vertexBuffer_);
		vertexBuffer_ = nullptr;

		GL_CHECK(glDeleteVertexArrays(1, &vao_));

		EntityManager::Get().Destroy(camera_);
		camera_ = nullptr;
	}

	virtual void Run() override
	{
		RunLoop(
			[&](float deltaSeconds)
			{
				ImGui::Begin("Transform", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				{
					ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
					ImGui::SetWindowSize(ImVec2(400.0f, 130.0f));
					UpdateTransformUI(deltaSeconds);
				}
				ImGui::End();

				transform_.rotate = Quat::AxisAngle(axis_, angle_);
				
				BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

				DebugDrawManager3D::Get().Begin(camera_);
				{
					DrawGrid3D();
				}
				DebugDrawManager3D::Get().End();

				shader_->Bind();
				{
					texture_->Active(0);

					shader_->SetUniform("world", Transform::ToMat(transform_));
					shader_->SetUniform("view", camera_->GetView());
					shader_->SetUniform("projection", camera_->GetProjection());

					GL_CHECK(glBindVertexArray(vao_));
					GL_CHECK(glDrawArrays(static_cast<GLenum>(DrawMode::TRIANGLES), 0, 36));
					GL_CHECK(glBindVertexArray(0));
				}
				shader_->Unbind();
				
				EndFrame();
			}
		);
	}

private:
	void UpdateTransformUI(float deltaSeconds)
	{
		ImGui::SliderFloat3("POSITION", transform_.position.data, -50.0f, +50.0f);
		ImGui::SameLine();
		if (ImGui::Button("RESET##POSITION"))
		{
			transform_.position = Vec3f(0.0f, 0.0f, 0.0f);
		}
		ImGui::SliderFloat3("AXIS", axis_.data, -1.0f, 1.0f);
		ImGui::SliderFloat("ANGLE", &angle_, 0.0f, 360.0f);
		ImGui::SameLine();
		if (ImGui::Button("RESET##ROTATE"))
		{
			axis_ = Vec3f();
			angle_ = 0.0f;
		}
		ImGui::SliderFloat3("SCALE", transform_.scale.data, 0.0f, 100.0f);
		ImGui::SameLine();
		if (ImGui::Button("RESET##SCALE"))
		{
			transform_.scale = Vec3f(1.0f, 1.0f, 1.0f);
		}

		if (!ImGui::IsWindowFocused())
		{
			camera_->Tick(deltaSeconds);
		}
	}

	void DrawGrid3D()
	{
		Vec4f color;
		for (float x = minX_; x <= maxX_; x += stride_)
		{
			color = (x == 0.0f) ? Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
			DebugDrawManager3D::Get().DrawLine(Vec3f(x, 0.0f, minZ_), Vec3f(x, 0.0f, maxZ_), color);
		}

		for (float z = minZ_; z <= maxZ_; z += stride_)
		{
			color = (z == 0.0f) ? Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
			DebugDrawManager3D::Get().DrawLine(Vec3f(minX_, 0.0f, z), Vec3f(maxX_, 0.0f, z), color);
		}

		DebugDrawManager3D::Get().DrawLine(Vec3f(0.0f, minY_, 0.0f), Vec3f(0.0f, maxY_, 0.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
	}

private:
	float minX_ = -50.0f;
	float maxX_ = +50.0f;
	float minY_ = -50.0f;
	float maxY_ = +50.0f;
	float minZ_ = -50.0f;
	float maxZ_ = +50.0f;
	float stride_ = 1.0f;

	FreeEulerCamera3D* camera_ = nullptr;

	uint32_t vao_ = 0;
	VertexBuffer* vertexBuffer_ = nullptr;
	Shader* shader_ = nullptr;
	Texture2D* texture_ = nullptr;

	Vec3f axis_ = Vec3f(0.0f, 0.0f, 0.0f);
	float angle_ = 0.0f;

	Transform transform_;
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