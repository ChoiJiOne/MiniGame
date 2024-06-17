#include <glad/glad.h>

#include "GLAssertion.h"
#include "RenderModule.h"
#include "Shader.h"
#include "Skybox.h"
#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer()
{
	shader_ = RenderModule::CreateResource<Shader>("Resource/Shader/Skybox.vert", "Resource/Shader/Skybox.frag");
	
	std::vector<float> vertices =
	{
		-1.0f, +1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,

		+1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		+1.0f,  1.0f,  1.0f,
		+1.0f,  1.0f,  1.0f,
		+1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		+1.0f,  1.0f, -1.0f,
		+1.0f,  1.0f,  1.0f,
		+1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		+1.0f, -1.0f,  1.0f
	};
	vertexCount_ = static_cast<uint32_t>(vertices.size());

	uint32_t byteSize = static_cast<uint32_t>(sizeof(float) * vertices.size());
	VertexBuffer::EUsage usage = VertexBuffer::EUsage::STATIC;
	vertexBuffer_ = RenderModule::CreateResource<VertexBuffer>(vertices.data(), byteSize, usage);

	GL_FAILED(glGenVertexArrays(1, &vertexArrayObject_));
	GL_FAILED(glBindVertexArray(vertexArrayObject_));
	{
		vertexBuffer_->Bind();

		GL_FAILED(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0)));
		GL_FAILED(glEnableVertexAttribArray(0));

		vertexBuffer_->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));

	bIsInitialized_ = true;
}

SkyboxRenderer::~SkyboxRenderer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void SkyboxRenderer::Release()
{
	CHECK(bIsInitialized_);

	if (shader_)
	{
		RenderModule::DestroyResource(shader_);
		shader_ = nullptr;
	}

	if (vertexBuffer_)
	{
		RenderModule::DestroyResource(vertexBuffer_);
		vertexBuffer_ = nullptr;
	}

	GL_FAILED(glDeleteVertexArrays(1, &vertexArrayObject_));

	bIsInitialized_ = false;
}

void SkyboxRenderer::Draw(const Mat4x4& view, const Mat4x4& projection, const Skybox* skybox)
{
	shader_->Bind();
	{
		GL_FAILED(glDepthFunc(GL_LEQUAL));

		shader_->SetUniform("view", view);
		shader_->SetUniform("projection", projection);

		skybox->Active(SKYBOX_BIND_SLOT);

		GL_FAILED(glBindVertexArray(vertexArrayObject_));
		RenderModule::ExecuteDrawVertex(vertexCount_, EDrawMode::TRIANGLES);
		GL_FAILED(glBindVertexArray(0));

		GL_FAILED(glDepthFunc(GL_LESS));
	}
	shader_->Unbind();
}