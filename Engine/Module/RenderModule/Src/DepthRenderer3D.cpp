#include <glad/glad.h>

#include "DepthRenderer3D.h"
#include "GLAssertion.h"
#include "IMesh.h"
#include "RenderModule.h"
#include "Shader.h"

DepthRenderer3D::DepthRenderer3D()
{
	staticMeshDepth_ = RenderModule::CreateResource<Shader>("Resource/Shader/StaticDepth.vert", "Resource/Shader/Depth.frag");
	skinnedMeshDepth_ = RenderModule::CreateResource<Shader>("Resource/Shader/SkinnedDepth.vert", "Resource/Shader/Depth.frag");
}

DepthRenderer3D::~DepthRenderer3D()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void DepthRenderer3D::Release()
{
	CHECK(bIsInitialized_);

	if (staticMeshDepth_)
	{
		RenderModule::DestroyResource(staticMeshDepth_);
		staticMeshDepth_ = nullptr;
	}

	if (skinnedMeshDepth_)
	{
		RenderModule::DestroyResource(skinnedMeshDepth_);
		skinnedMeshDepth_ = nullptr;
	}

	bIsInitialized_ = false;
}

void DepthRenderer3D::DrawStaticMesh3D(const Mat4x4& world, IMesh* mesh)
{
	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	RenderModule::SetDepthMode(true);
	{
		staticMeshDepth_->Bind();

		staticMeshDepth_->SetUniform("world", world);
		staticMeshDepth_->SetUniform("lightView", lightView_);
		staticMeshDepth_->SetUniform("lightProjection", lightProjection_);

		mesh->Bind();
		RenderModule::ExecuteDrawIndex(mesh->GetIndexCount(), EDrawMode::Triangles);
		mesh->Unbind();

		staticMeshDepth_->Unbind();

	}
	RenderModule::SetDepthMode(originEnableDepth);
}

void DepthRenderer3D::DrawSkinnedMesh3D(const Mat4x4& world, IMesh* mesh, const std::vector<Mat4x4>& bindPose, const std::vector<Mat4x4>& invBindPose)
{
	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	RenderModule::SetDepthMode(true);
	{
		skinnedMeshDepth_->Bind();

		skinnedMeshDepth_->SetUniform("world", world);
		skinnedMeshDepth_->SetUniform("lightView", lightView_);
		skinnedMeshDepth_->SetUniform("lightProjection", lightProjection_);
		skinnedMeshDepth_->SetUniform("bindPose", bindPose.data(), bindPose.size());
		skinnedMeshDepth_->SetUniform("invBindPose", invBindPose.data(), invBindPose.size());
		
		mesh->Bind();
		RenderModule::ExecuteDrawIndex(mesh->GetIndexCount(), EDrawMode::Triangles);
		mesh->Unbind();

		skinnedMeshDepth_->Unbind();
	}
	RenderModule::SetDepthMode(originEnableDepth);
}