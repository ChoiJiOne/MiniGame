#include <glad/glad.h>

#include "GLAssertion.h"
#include "IMesh.h"
#include "MeshRenderer3D.h"
#include "RenderModule.h"
#include "Shader.h"

MeshRenderer3D::MeshRenderer3D()
{
	staticMeshShader_ = RenderModule::CreateResource<Shader>("Resource/Shader/StaticMesh.vert", "Resource/Shader/Mesh.frag");
	skinnedMeshShader_ = RenderModule::CreateResource<Shader>("Resource/Shader/SkinnedMesh.vert", "Resource/Shader/Mesh.frag");
}

MeshRenderer3D::~MeshRenderer3D()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void MeshRenderer3D::Release()
{
	CHECK(bIsInitialized_);

	if (staticMeshShader_)
	{
		RenderModule::DestroyResource(staticMeshShader_);
		staticMeshShader_ = nullptr;
	}

	if (skinnedMeshShader_)
	{
		RenderModule::DestroyResource(skinnedMeshShader_);
		skinnedMeshShader_ = nullptr;
	}

	bIsInitialized_ = false;
}

void MeshRenderer3D::DrawStaticMesh3D(const Mat4x4& world, IMesh* mesh, ITexture2D* material)
{
	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	RenderModule::SetDepthMode(true);
	{
		staticMeshShader_->Bind();

		staticMeshShader_->SetUniform("world", world);
		staticMeshShader_->SetUniform("view", view_);
		staticMeshShader_->SetUniform("projection", projection_);

		material->Active(0);

		mesh->Bind();
		RenderModule::ExecuteDrawIndex(mesh->GetIndexCount(), EDrawMode::Triangles);
		mesh->Unbind();

		staticMeshShader_->Unbind();
	}
	RenderModule::SetDepthMode(originEnableDepth);
}

void MeshRenderer3D::DrawSkinnedMesh3D(const Mat4x4& world, IMesh* mesh, const std::vector<Mat4x4>& bindPose, const std::vector<Mat4x4>& invBindPose, ITexture2D* material)
{
	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	RenderModule::SetDepthMode(true);
	{
		skinnedMeshShader_->Bind();

		skinnedMeshShader_->SetUniform("world", world);
		skinnedMeshShader_->SetUniform("view", view_);
		skinnedMeshShader_->SetUniform("projection", projection_);
		skinnedMeshShader_->SetUniform("bindPose", bindPose.data(), bindPose.size());
		skinnedMeshShader_->SetUniform("invBindPose", invBindPose.data(), invBindPose.size());

		material->Active(0);

		mesh->Bind();
		RenderModule::ExecuteDrawIndex(mesh->GetIndexCount(), EDrawMode::Triangles);
		mesh->Unbind();

		skinnedMeshShader_->Unbind();
	}
	RenderModule::SetDepthMode(originEnableDepth);
}
