#include <glad/glad.h>

#include "MeshRenderer.h"
#include "GLAssertion.h"
#include "RenderModule.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "ShadowMap.h"

MeshRenderer::MeshRenderer()
{
	staticMeshShader_ = RenderModule::CreateResource<Shader>("Resource/Shader/StaticMesh.vert", "Resource/Shader/Mesh.frag");
	skinnedMeshShader_ = RenderModule::CreateResource<Shader>("Resource/Shader/SkinnedMesh.vert", "Resource/Shader/Mesh.frag");

	bIsInitialized_ = true;
}

MeshRenderer::~MeshRenderer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void MeshRenderer::Release()
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

	bIsInitialized_ = false;
}

void MeshRenderer::DrawStaticMesh(const Mat4x4& world, const StaticMesh* mesh, const ITexture* material)
{
	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	staticMeshShader_->Bind();
	{
		RenderModule::SetDepthMode(true);

		shadowMap_->Active(SHADOW_MAP_BIND_SLOT);
		material->Active(MATERIAL_MAP_BIND_SLOT);

		staticMeshShader_->SetUniform("world", world);
		staticMeshShader_->SetUniform("view", view_);
		staticMeshShader_->SetUniform("projection", projection_);
		staticMeshShader_->SetUniform("lightSpaceMatrix", lightSpaceMatrix_);
		staticMeshShader_->SetUniform("lightDirection", lightDirection_);
		staticMeshShader_->SetUniform("lightColor", lightColor_);
		staticMeshShader_->SetUniform("cameraPosition", cameraPosition_);

		mesh->Bind();
		RenderModule::ExecuteDrawIndex(mesh->GetIndexCount(), EDrawMode::TRIANGLES);
		mesh->Unbind();

		RenderModule::SetDepthMode(originEnableDepth);
	}
	staticMeshShader_->Unbind();
}

void MeshRenderer::DrawSkinnedMesh(const Mat4x4& world, const std::vector<Mat4x4>& bindPose, const std::vector<Mat4x4>& invBindPose, const SkinnedMesh* mesh, const ITexture* material)
{
	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	skinnedMeshShader_->Bind();
	{
		RenderModule::SetDepthMode(true);

		shadowMap_->Active(SHADOW_MAP_BIND_SLOT);
		material->Active(MATERIAL_MAP_BIND_SLOT);

		skinnedMeshShader_->SetUniform("world", world);
		skinnedMeshShader_->SetUniform("view", view_);
		skinnedMeshShader_->SetUniform("projection", projection_);
		skinnedMeshShader_->SetUniform("lightSpaceMatrix", lightSpaceMatrix_);
		skinnedMeshShader_->SetUniform("bindPose", bindPose.data(), bindPose.size());
		skinnedMeshShader_->SetUniform("invBindPose", invBindPose.data(), invBindPose.size());
		skinnedMeshShader_->SetUniform("lightDirection", lightDirection_);
		skinnedMeshShader_->SetUniform("lightColor", lightColor_);
		skinnedMeshShader_->SetUniform("cameraPosition", cameraPosition_);

		mesh->Bind();
		RenderModule::ExecuteDrawIndex(mesh->GetIndexCount(), EDrawMode::TRIANGLES);
		mesh->Unbind();

		RenderModule::SetDepthMode(originEnableDepth);
	}
	skinnedMeshShader_->Unbind();
}
