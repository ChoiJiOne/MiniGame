#include <glad/glad.h>

#include "GameAssert.h"
#include "GameUtils.h"
#include "Shader.h"

static const uint32_t MAX_STRING_BUFFER = 1024;
static char glLogBuffer_[MAX_STRING_BUFFER] = { 0, };

Shader::Shader()
{
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
	GAME_CHECK(bIsInitialized_);

	if (programID_) /** 셰이더 프로그램이 할당에 성공했다면 0이 아닌 값이 할당됨. */
	{
		GL_CHECK(glDeleteProgram(programID_));
		programID_ = 0;
	}

	bIsInitialized_ = false;
}

GameError Shader::LoadFromFile(const std::string& csPath)
{
	std::vector<uint8_t> csBuffer;
	GameError error = GameUtils::ReadFile(csPath, csBuffer);
	if (error.GetCode() != ErrorCode::OK)
	{
		return error;
	}
	csBuffer.push_back('\0');

	uint32_t csID = CreateShader(Type::COMPUTE, reinterpret_cast<const char*>(csBuffer.data()));

	std::vector<uint32_t> shaderIDs = { csID };
	programID_ = CreateProgram(shaderIDs);

	GL_CHECK(glDeleteShader(csID));

	bIsInitialized_ = true;
	return GameError(ErrorCode::OK, "Succeed load shader from file.");
}

GameError Shader::LoadFromFile(const std::string& vsPath, const std::string& fsPath)
{
	std::vector<uint8_t> vsBuffer;
	GameError error = GameUtils::ReadFile(vsPath, vsBuffer);
	if (error.GetCode() != ErrorCode::OK)
	{
		return error;
	}
	vsBuffer.push_back('\0');

	std::vector<uint8_t> fsBuffer;
	error = GameUtils::ReadFile(fsPath, fsBuffer);
	if (error.GetCode() != ErrorCode::OK)
	{
		return error;
	}
	fsBuffer.push_back('\0');

	uint32_t vsID = CreateShader(Type::VERTEX, reinterpret_cast<const char*>(vsBuffer.data()));
	uint32_t fsID = CreateShader(Type::FRAGMENT, reinterpret_cast<const char*>(fsBuffer.data()));

	std::vector<uint32_t> shaderIDs = { vsID, fsID };
	programID_ = CreateProgram(shaderIDs);

	GL_CHECK(glDeleteShader(fsID));
	GL_CHECK(glDeleteShader(vsID));

	bIsInitialized_ = true;
	return GameError(ErrorCode::OK, "Succeed load shader from file.");
}

GameError Shader::LoadFromFile(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath)
{
	std::vector<uint8_t> vsBuffer;
	GameError error = GameUtils::ReadFile(vsPath, vsBuffer);
	if (error.GetCode() != ErrorCode::OK)
	{
		return error;
	}
	vsBuffer.push_back('\0');

	std::vector<uint8_t> gsBuffer;
	error = GameUtils::ReadFile(gsPath, gsBuffer);
	if (error.GetCode() != ErrorCode::OK)
	{
		return error;
	}
	gsBuffer.push_back('\0');

	std::vector<uint8_t> fsBuffer;
	error = GameUtils::ReadFile(fsPath, fsBuffer);
	if (error.GetCode() != ErrorCode::OK)
	{
		return error;
	}
	fsBuffer.push_back('\0');

	uint32_t vsID = CreateShader(Type::VERTEX, reinterpret_cast<const char*>(vsBuffer.data()));
	uint32_t gsID = CreateShader(Type::GEOMETRY, reinterpret_cast<const char*>(gsBuffer.data()));
	uint32_t fsID = CreateShader(Type::FRAGMENT, reinterpret_cast<const char*>(fsBuffer.data()));

	std::vector<uint32_t> shaderIDs = { vsID, gsID, fsID };
	programID_ = CreateProgram(shaderIDs);

	GL_CHECK(glDeleteShader(fsID));
	GL_CHECK(glDeleteShader(gsID));
	GL_CHECK(glDeleteShader(vsID));

	bIsInitialized_ = true;
	return GameError(ErrorCode::OK, "Succeed load shader from file.");
}

void Shader::Bind()
{
	GL_CHECK(glUseProgram(programID_));
}

void Shader::Unbind()
{
	GL_CHECK(glUseProgram(0));
}

void Shader::SetUniform(const std::string& name, bool value)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform1i(location, static_cast<int32_t>(value)));
}

void Shader::SetUniform(const std::string& name, int32_t value)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform1i(location, value));
}

void Shader::SetUniform(const std::string& name, const int32_t* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform1iv(location, count, bufferPtr));
}

void Shader::SetUniform(const std::string& name, float value)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform1f(location, value));
}

void Shader::SetUniform(const std::string& name, const float* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform1fv(location, count, bufferPtr));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec2f& value)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = value.GetPtr();
	GL_CHECK(glUniform2fv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec2f* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_CHECK(glUniform2fv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, float x, float y)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform2f(location, x, y));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec2i& value)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = value.GetPtr();
	GL_CHECK(glUniform2iv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec2i* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = reinterpret_cast<const int32_t*>(bufferPtr);
	GL_CHECK(glUniform2iv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, int32_t x, int32_t y)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform2i(location, x, y));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec3f& value)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = value.GetPtr();
	GL_CHECK(glUniform3fv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec3f* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_CHECK(glUniform3fv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, float x, float y, float z)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform3f(location, x, y, z));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec3i& value)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = value.GetPtr();
	GL_CHECK(glUniform3iv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec3i* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = reinterpret_cast<const int32_t*>(bufferPtr);
	GL_CHECK(glUniform3iv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, int32_t x, int32_t y, int32_t z)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform3i(location, x, y, z));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec4f& value)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = value.GetPtr();
	GL_CHECK(glUniform4fv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec4f* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_CHECK(glUniform4fv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, float x, float y, float z, float w)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform4f(location, x, y, z, w));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec4i& value)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = value.GetPtr();
	GL_CHECK(glUniform4iv(location, 1, valuePtr));
}

void Shader::SetUniform(const std::string& name, const GameMath::Vec4i* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);

	const int32_t* valuePtr = reinterpret_cast<const int32_t*>(bufferPtr);
	GL_CHECK(glUniform4iv(location, count, valuePtr));
}

void Shader::SetUniform(const std::string& name, int32_t x, int32_t y, int32_t z, int32_t w)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniform4i(location, x, y, z, w));
}

void Shader::SetUniform(const std::string& name, const GameMath::Mat2x2& value)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniformMatrix2fv(location, 1, GL_FALSE, value.GetPtr()));
}

void Shader::SetUniform(const std::string& name, const GameMath::Mat2x2* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);
	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_CHECK(glUniformMatrix2fv(location, count, GL_FALSE, valuePtr));
}

void Shader::SetUniform(const std::string& name, const GameMath::Mat3x3& value)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniformMatrix3fv(location, 1, GL_FALSE, value.GetPtr()));
}

void Shader::SetUniform(const std::string& name, const GameMath::Mat3x3* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);
	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_CHECK(glUniformMatrix3fv(location, count, GL_FALSE, valuePtr));
}

void Shader::SetUniform(const std::string& name, const GameMath::Mat4x4& value)
{
	int32_t location = GetUniformLocation(name);
	GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, value.GetPtr()));
}

void Shader::SetUniform(const std::string& name, const GameMath::Mat4x4* bufferPtr, uint32_t count)
{
	int32_t location = GetUniformLocation(name);
	const float* valuePtr = reinterpret_cast<const float*>(bufferPtr);
	GL_CHECK(glUniformMatrix4fv(location, count, GL_FALSE, valuePtr));
}

int32_t Shader::GetUniformLocation(const std::string& name)
{
	std::map<std::string, uint32_t>::iterator uniformLocation = uniformLocationCache_.find(name);

	if (uniformLocation == uniformLocationCache_.end())
	{
		int32_t location = glGetUniformLocation(programID_, name.c_str());
		GL_CHECK(location != -1);

		uniformLocationCache_.insert({ name, location });
		return location;
	}
	else
	{
		return uniformLocation->second;
	}
}

uint32_t Shader::CreateShader(const Type& type, const char* sourcePtr)
{
	uint32_t shaderID = glCreateShader(static_cast<GLenum>(type));
	GL_CHECK(shaderID != 0);

	GL_CHECK(glShaderSource(shaderID, 1, &sourcePtr, nullptr));
	GL_CHECK(glCompileShader(shaderID));

	int32_t status;
	GL_CHECK(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status));
	if (!status)
	{
		GL_CHECK(glGetShaderInfoLog(shaderID, MAX_STRING_BUFFER, nullptr, glLogBuffer_));
		GAME_ASSERT(false, "\nfailed to compile shader\n%s", glLogBuffer_);
	}

	return shaderID;
}

uint32_t Shader::CreateProgram(const std::vector<uint32_t>& shaderIDs)
{
	GAME_CHECK(shaderIDs.size() > 0);

	uint32_t programID = glCreateProgram();
	GL_CHECK(programID != 0);

	for (const uint32_t shaderID : shaderIDs)
	{
		GL_CHECK(glAttachShader(programID, shaderID));
	}
	GL_CHECK(glLinkProgram(programID));

	int32_t status;
	GL_CHECK(glGetProgramiv(programID, GL_LINK_STATUS, &status));
	if (!status)
	{
		GL_CHECK(glGetProgramInfoLog(programID_, MAX_STRING_BUFFER, nullptr, glLogBuffer_));
		GAME_ASSERT(false, "\nfailed to link shader program\n%s", glLogBuffer_);
	}

	return programID;
}