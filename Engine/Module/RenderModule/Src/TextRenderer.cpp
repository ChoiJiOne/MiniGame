#pragma warning(push)
#pragma warning(disable: 26451)

#include <glad/glad.h>

#include "Assertion.h"

#include "GLAssertion.h"
#include "Shader.h"
#include "TextRenderer.h"
#include "TTFont.h"

TextRenderer::TextRenderer()
{
	shader_ = RenderModule::CreateResource<Shader>("Resource/Shader/Text.vert", "Resource/Shader/Text.frag");

	uint32_t byteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
	VertexBuffer::EUsage usage = VertexBuffer::EUsage::DYNAMIC;
	vertexBuffer_ = RenderModule::CreateResource<VertexBuffer>(byteSize, usage);

	GL_FAILED(glGenVertexArrays(1, &vertexArrayObject_));
	GL_FAILED(glBindVertexArray(vertexArrayObject_));
	{
		vertexBuffer_->Bind();

		GL_FAILED(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, position))));
		GL_FAILED(glEnableVertexAttribArray(0));

		GL_FAILED(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, uv))));
		GL_FAILED(glEnableVertexAttribArray(1));

		GL_FAILED(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, Vertex::GetStride(), (void*)(offsetof(Vertex, color))));
		GL_FAILED(glEnableVertexAttribArray(2));

		vertexBuffer_->Unbind();
	}
	GL_FAILED(glBindVertexArray(0));
}

TextRenderer::~TextRenderer()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void TextRenderer::Release()
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
}

void TextRenderer::DrawText2D(const TTFont* font, const std::wstring& text, const Vec2f& position, const Vec4f& color)
{
	CHECK(text.length() <= MAX_STRING_LEN && font != nullptr);

	float width = 0.0f;
	float height = 0.0f;
	font->MeasureText(text, width, height);

	Vec2f startPosition = position - Vec2f(width, -height) * 0.5f;
	int32_t vertexCount = UpdateGlyphVertexBuffer(font, text, startPosition, color);

	GLboolean originEnableDepth;
	GL_FAILED(glGetBooleanv(GL_DEPTH_TEST, &originEnableDepth));

	RenderModule::SetDepthMode(false);
	{
		shader_->Bind();

		const void* vertexPtr = reinterpret_cast<const void*>(vertices_.data());
		uint32_t bufferByteSize = static_cast<uint32_t>(Vertex::GetStride() * vertices_.size());
		vertexBuffer_->SetBufferData(vertexPtr, bufferByteSize);

		shader_->SetUniform("ortho", ortho_);

		GL_FAILED(glActiveTexture(GL_TEXTURE0));
		GL_FAILED(glBindTexture(GL_TEXTURE_2D, font->GetGlyphAtlasID()));

		GL_FAILED(glBindVertexArray(vertexArrayObject_));
		RenderModule::ExecuteDrawVertex(vertexCount, EDrawMode::TRIANGLES);
		GL_FAILED(glBindVertexArray(0));

		shader_->Unbind();
	}
	RenderModule::SetDepthMode(originEnableDepth);
}

uint32_t TextRenderer::UpdateGlyphVertexBuffer(const TTFont* font, const std::wstring& text, const Vec2f& startPosition, const Vec4f& color)
{
	float glyphAtlasSize = static_cast<float>(font->GetGlyphAtlasSize());

	Vec2f currentPosition = startPosition;
	int32_t vertexCount = 0;

	for (const auto& unicode : text)
	{
		const Glyph& glyph = font->GetGlyph(static_cast<int32_t>(unicode));

		float unicodeWidth = static_cast<float>(glyph.position1.x - glyph.position0.x);
		float unicodeHeight = static_cast<float>(glyph.position1.y - glyph.position0.y);

		vertices_[vertexCount + 0].position = Vec2f(currentPosition.x + glyph.xoffset, currentPosition.y + glyph.yoffset);
		vertices_[vertexCount + 0].uv = Vec2f(static_cast<float>(glyph.position0.x) / glyphAtlasSize, static_cast<float>(glyph.position0.y) / glyphAtlasSize);
		vertices_[vertexCount + 0].color = color;

		vertices_[vertexCount + 1].position = Vec2f(currentPosition.x + glyph.xoffset, currentPosition.y + unicodeHeight + glyph.yoffset);
		vertices_[vertexCount + 1].uv = Vec2f(static_cast<float>(glyph.position0.x) / glyphAtlasSize, static_cast<float>(glyph.position1.y) / glyphAtlasSize);
		vertices_[vertexCount + 1].color = color;

		vertices_[vertexCount + 2].position = Vec2f(currentPosition.x + glyph.xoffset + unicodeWidth, currentPosition.y + glyph.yoffset);
		vertices_[vertexCount + 2].uv = Vec2f(static_cast<float>(glyph.position1.x) / glyphAtlasSize, static_cast<float>(glyph.position0.y) / glyphAtlasSize);
		vertices_[vertexCount + 2].color = color;

		vertices_[vertexCount + 3].position = Vec2f(currentPosition.x + glyph.xoffset + unicodeWidth, currentPosition.y + glyph.yoffset);
		vertices_[vertexCount + 3].uv = Vec2f(static_cast<float>(glyph.position1.x) / glyphAtlasSize, static_cast<float>(glyph.position0.y) / glyphAtlasSize);
		vertices_[vertexCount + 3].color = color;

		vertices_[vertexCount + 4].position = Vec2f(currentPosition.x + glyph.xoffset, currentPosition.y + unicodeHeight + glyph.yoffset);
		vertices_[vertexCount + 4].uv = Vec2f(static_cast<float>(glyph.position0.x) / glyphAtlasSize, static_cast<float>(glyph.position1.y) / glyphAtlasSize);
		vertices_[vertexCount + 4].color = color;

		vertices_[vertexCount + 5].position = Vec2f(currentPosition.x + glyph.xoffset + unicodeWidth, currentPosition.y + unicodeHeight + glyph.yoffset);
		vertices_[vertexCount + 5].uv = Vec2f(static_cast<float>(glyph.position1.x) / glyphAtlasSize, static_cast<float>(glyph.position1.y) / glyphAtlasSize);
		vertices_[vertexCount + 5].color = color;

		currentPosition.x += glyph.xadvance;
		vertexCount += 6;
	}

	return vertexCount;
}

#pragma warning(pop)