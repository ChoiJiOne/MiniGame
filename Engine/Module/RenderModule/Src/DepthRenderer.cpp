#include <glad/glad.h>

#include "DepthRenderer.h"
#include "GLAssertion.h"
#include "RenderModule.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"

DepthRenderer::DepthRenderer()
{
	staticMeshShader_ = RenderModule::CreateResource<Shader>("Resource/Shader/StaticMeshShadow.vert", "Resource/Shader/Shadow.frag");
	skinnedMeshShader_ = RenderModule::CreateResource<Shader>("Resource/Shader/SkinnedMeshShadow.vert", "Resource/Shader/Shadow.frag");

	bIsInitialized_ = true;
}

DepthRenderer::~DepthRenderer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void DepthRenderer::Release()
{
	CHECK(bIsInitialized_);

	if (skinnedMeshShader_)
	{
		RenderModule::DestroyResource(skinnedMeshShader_);
		skinnedMeshShader_ = nullptr;
	}

	if (staticMeshShader_)
	{
		RenderModule::DestroyResource(staticMeshShader_);
		staticMeshShader_ = nullptr;
	}
}

void DepthRenderer::DrawStaticMesh(const Mat4x4& world, const StaticMesh* mesh)
{
	GLint originCullFace;
	GL_FAILED(glGetIntegerv(GL_CULL_FACE_MODE, &originCullFace));

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	staticMeshShader_->Bind();
	{
		RenderModule::SetDepthMode(true);

		staticMeshShader_->SetUniform("lightSpaceMatrix", lightSpaceMatrix_);
		staticMeshShader_->SetUniform("world", world);

		GL_FAILED(glCullFace(GL_FRONT));
		
		mesh->Bind();
		RenderModule::ExecuteDrawIndex(mesh->GetIndexCount(), EDrawMode::TRIANGLES);
		mesh->Unbind();

		GL_FAILED(glCullFace(originCullFace));
		RenderModule::SetDepthMode(originEnableDepth);
	}
	staticMeshShader_->Unbind();
}

void DepthRenderer::DrawSkinnedMesh(const Mat4x4& world, const std::vector<Mat4x4>& bindPose, const std::vector<Mat4x4>& invBindPose, const SkinnedMesh* mesh)
{
	GLint originCullFace;
	GL_FAILED(glGetIntegerv(GL_CULL_FACE_MODE, &originCullFace));

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	skinnedMeshShader_->Bind();
	{
		RenderModule::SetDepthMode(true);

		skinnedMeshShader_->SetUniform("lightSpaceMatrix", lightSpaceMatrix_);
		skinnedMeshShader_->SetUniform("world", world);
		skinnedMeshShader_->SetUniform("bindPose", bindPose.data(), bindPose.size());
		skinnedMeshShader_->SetUniform("invBindPose", invBindPose.data(), invBindPose.size());

		GL_FAILED(glCullFace(GL_FRONT));

		mesh->Bind();
		RenderModule::ExecuteDrawIndex(mesh->GetIndexCount(), EDrawMode::TRIANGLES);
		mesh->Unbind();

		GL_FAILED(glCullFace(originCullFace));
		RenderModule::SetDepthMode(originEnableDepth);
	}
	skinnedMeshShader_->Unbind();
}