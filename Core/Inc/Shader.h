#pragma once

#include <map>
#include <string>
#include <vector>

#include "GameMath.h"
#include "IResource.h"


namespace GameMaker
{
class Shader : public IResource
{
public:
	explicit Shader(const std::string& csPath);
	explicit Shader(const std::string& vsPath, const std::string& fsPath);
	explicit Shader(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath);
	virtual ~Shader();

	DISALLOW_COPY_AND_ASSIGN(Shader);

	virtual void Release() override;

	void Bind();
	void Unbind();

	void SetUniform(const std::string& name, bool value);
	void SetUniform(const std::string& name, int32_t value);
	void SetUniform(const std::string& name, const int32_t* bufferPtr, uint32_t count);
	void SetUniform(const std::string& name, float value);
	void SetUniform(const std::string& name, const float* bufferPtr, uint32_t count);
	void SetUniform(const std::string& name, const Vec2f& value);
	void SetUniform(const std::string& name, const Vec2f* bufferPtr, uint32_t count);
	void SetUniform(const std::string& name, float x, float y);
	void SetUniform(const std::string& name, const Vec2i& value);
	void SetUniform(const std::string& name, const Vec2i* bufferPtr, uint32_t count);
	void SetUniform(const std::string& name, int32_t x, int32_t y);
	void SetUniform(const std::string& name, const Vec3f& value);
	void SetUniform(const std::string& name, const Vec3f* bufferPtr, uint32_t count);
	void SetUniform(const std::string& name, float x, float y, float z);
	void SetUniform(const std::string& name, const Vec3i& value);
	void SetUniform(const std::string& name, const Vec3i* bufferPtr, uint32_t count);
	void SetUniform(const std::string& name, int32_t x, int32_t y, int32_t z);
	void SetUniform(const std::string& name, const Vec4f& value);
	void SetUniform(const std::string& name, const Vec4f* bufferPtr, uint32_t count);
	void SetUniform(const std::string& name, float x, float y, float z, float w);
	void SetUniform(const std::string& name, const Vec4i& value);
	void SetUniform(const std::string& name, const Vec4i* bufferPtr, uint32_t count);
	void SetUniform(const std::string& name, int32_t x, int32_t y, int32_t z, int32_t w);
	void SetUniform(const std::string& name, const Mat2x2& value);
	void SetUniform(const std::string& name, const Mat2x2* bufferPtr, uint32_t count);
	void SetUniform(const std::string& name, const Mat3x3& value);
	void SetUniform(const std::string& name, const Mat3x3* bufferPtr, uint32_t count);
	void SetUniform(const std::string& name, const Mat4x4& value);
	void SetUniform(const std::string& name, const Mat4x4* bufferPtr, uint32_t count);

protected:
	enum class EType : int32_t
	{
		Vertex     = 0x8B31,
		Control    = 0x8E88,
		Evaluation = 0x8E87,
		Geometry   = 0x8DD9,
		Fragment   = 0x8B30,
		Compute    = 0x91B9,
	};

	int32_t GetUniformLocation(const std::string& name);
	uint32_t CreateShader(const EType& type, const std::string& path);
	uint32_t CreateProgram(const std::vector<uint32_t>& shaderIDs);

protected:
	uint32_t programID_ = 0;
	std::map<std::string, uint32_t> uniformLocationCache_;
};

}