#include <glad/glad.h>

#include "Assertion.h"
#include "FileUtils.h"
#include "Shader.h"

using namespace GameMaker;

Shader::Shader(const std::string& csPath)
{
	uniformLocationCache_ = std::map<std::string, uint32_t>();

	uint32_t csID = CreateShader(EType::Compute, csPath);
	ASSERT(csID != 0, "failed to create %s", csPath.c_str());

	std::vector<uint32_t> shaderIDs = { csID };
	programID_ = CreateProgram(shaderIDs);

	GL_FAILED(glDeleteShader(csID));

	bIsInitialized_ = true;
}

Shader::Shader(const std::string& vsPath, const std::string& fsPath)
{
	uniformLocationCache_ = std::map<std::string, uint32_t>();

	uint32_t vsID = CreateShader(EType::Vertex, vsPath);
	ASSERT(vsID != 0, "failed to create %s", vsPath.c_str());

	uint32_t fsID = CreateShader(EType::Fragment, fsPath);
	ASSERT(fsID != 0, "failed to create %s", fsPath.c_str());

	std::vector<uint32_t> shaderIDs = { vsID, fsID };
	programID_ = CreateProgram(shaderIDs);

	GL_FAILED(glDeleteShader(vsID));
	GL_FAILED(glDeleteShader(fsID));

	bIsInitialized_ = true;
}

Shader::Shader(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath)
{
	uniformLocationCache_ = std::map<std::string, uint32_t>();

	uint32_t vsID = CreateShader(EType::Vertex, vsPath);
	ASSERT(vsID != 0, "failed to create %s", vsPath.c_str());

	uint32_t gsID = CreateShader(EType::Geometry, gsPath);
	ASSERT(gsID != 0, "failed to create %s", gsPath.c_str());

	uint32_t fsID = CreateShader(EType::Fragment, fsPath);
	ASSERT(fsID != 0, "failed to create %s", fsPath.c_str());

	std::vector<uint32_t> shaderIDs = { vsID, gsID, fsID };
	programID_ = CreateProgram(shaderIDs);

	GL_FAILED(glDeleteShader(vsID));
	GL_FAILED(glDeleteShader(gsID));
	GL_FAILED(glDeleteShader(fsID));

	bIsInitialized_ = true;
}

Shader::~Shader()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Shader::Release()
{
	CHECK(bIsInitialized_);

	if (programID_)
	{
		GL_FAILED(glDeleteProgram(programID_));
	}

	bIsInitialized_ = false;
}

void Shader::Bind()
{
	GL_FAILED(glUseProgram(programID_));
}

void Shader::Unbind()
{
	GL_FAILED(glUseProgram(0));
}

void Shader::SetUniform(const std::string& name, bool value)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform1i(location, static_cast<int32_t>(value)));
}

void Shader::SetUniform(const std::string& name, int32_t value)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform1i(location, value));
}

void Shader::SetUniform(const std::string& name, const int32_t* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform1iv(location, count, bufferPtr));
}

void Shader::SetUniform(const std::string& name, float value)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform1f(location, value));
}

void Shader::SetUniform(const std::string& name, const float* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform1fv(location, count, bufferPtr));
}

void Shader::SetUniform(const std::string& name, const Vec2f& value)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = value.GetPtr();
	GL_FAILED(glUniform2fv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const Vec2f* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_FAILED(glUniform2fv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, float x, float y)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform2f(location, x, y));
}

void Shader::SetUniform(const std::string& name, const Vec2i& value)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = value.GetPtr();
	GL_FAILED(glUniform2iv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const Vec2i* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = reinterpret_cast<const int32_t*>(bufferPtr);
	GL_FAILED(glUniform2iv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, int32_t x, int32_t y)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform2i(location, x, y));
}

void Shader::SetUniform(const std::string& name, const Vec3f& value)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = value.GetPtr();
	GL_FAILED(glUniform3fv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const Vec3f* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_FAILED(glUniform3fv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, float x, float y, float z)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform3f(location, x, y, z));
}

void Shader::SetUniform(const std::string& name, const Vec3i& value)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = value.GetPtr();
	GL_FAILED(glUniform3iv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const Vec3i* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = reinterpret_cast<const int32_t*>(bufferPtr);
	GL_FAILED(glUniform3iv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, int32_t x, int32_t y, int32_t z)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform3i(location, x, y, z));
}

void Shader::SetUniform(const std::string& name, const Vec4f& value)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = value.GetPtr();
	GL_FAILED(glUniform4fv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const Vec4f* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_FAILED(glUniform4fv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, float x, float y, float z, float w)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform4f(location, x, y, z, w));
}

void Shader::SetUniform(const std::string& name, const Vec4i& value)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = value.GetPtr();
	GL_FAILED(glUniform4iv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const Vec4i* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = reinterpret_cast<const int32_t*>(bufferPtr);
	GL_FAILED(glUniform4iv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, int32_t x, int32_t y, int32_t z, int32_t w)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniform4i(location, x, y, z, w));
}

void Shader::SetUniform(const std::string& name, const Mat2x2& value)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniformMatrix2fv(location, 1, GL_FALSE, value.GetPtr()));
}

void Shader::SetUniform(const std::string& name, const Mat2x2* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);
	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_FAILED(glUniformMatrix2fv(location, count, GL_FALSE, valuePtr));
}

void Shader::SetUniform(const std::string& name, const Mat3x3& value)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniformMatrix3fv(location, 1, GL_FALSE, value.GetPtr()));
}

void Shader::SetUniform(const std::string& name, const Mat3x3* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);
	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_FAILED(glUniformMatrix3fv(location, count, GL_FALSE, valuePtr));
}

void Shader::SetUniform(const std::string& name, const Mat4x4& value)
{
	int32_t location = GetUniformLocation(name);
	GL_FAILED(glUniformMatrix4fv(location, 1, GL_FALSE, value.GetPtr()));
}

void Shader::SetUniform(const std::string& name, const Mat4x4* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);
	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_FAILED(glUniformMatrix4fv(location, count, GL_FALSE, valuePtr));
}

int32_t Shader::GetUniformLocation(const std::string& name)
{
	std::map<std::string, uint32_t>::iterator uniformLocation = uniformLocationCache_.find(name);

	if (uniformLocation == uniformLocationCache_.end())
	{
		int32_t location = glGetUniformLocation(programID_, name.c_str());
		CHECK(location != -1);

		uniformLocationCache_.insert({ name, location });
		return location;
	}
	else
	{
		return uniformLocation->second;
	}
}

uint32_t Shader::CreateShader(const EType& type, const std::string& path)
{
	uint32_t shaderID = glCreateShader(static_cast<GLenum>(type));

	std::vector<uint8_t> buffer = FileUtils::ReadFile(path);

	std::string source = std::string(buffer.begin(), buffer.end());
	const char* sourcePtr = source.c_str();

	GL_FAILED(glShaderSource(shaderID, 1, &sourcePtr, nullptr));
	GL_FAILED(glCompileShader(shaderID));

	int32_t status;
	GL_FAILED(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status));
	if (!status)
	{
		char buffer[1024];

		glGetShaderInfoLog(shaderID, 1024, nullptr, buffer);
		ASSERT(false, "failed to compile shader\n%s", buffer);
	}

	return shaderID;
}

uint32_t Shader::CreateProgram(const std::vector<uint32_t>& shaderIDs)
{
	CHECK(shaderIDs.size() > 0);

	uint32_t programID = glCreateProgram();
	CHECK(programID != 0);

	for (const uint32_t shaderID : shaderIDs)
	{
		GL_FAILED(glAttachShader(programID, shaderID));
	}
	GL_FAILED(glLinkProgram(programID));

	int32_t status;
	GL_FAILED(glGetProgramiv(programID, GL_LINK_STATUS, &status));
	if (!status)
	{
		char buffer[1024];

		glGetProgramInfoLog(programID_, 1024, nullptr, buffer);
		ASSERT(false, "failed to link shader program\n%s", buffer);
	}

	return programID;
}