#pragma once

#include <map>
#include <string>
#include <vector>

#include "Mat.h"
#include "Vec.h"

#include "IResource.h"


/**
 * @brief GLSL 셰이더를 컴파일하고 셰이더 프로그램을 관리합니다.
 */
class Shader : public IResource
{
public:
	/**
	 * @brief 셰이더 파일을 읽어 셰이더 프로그램을 생성합니다.
	 * 
	 * @param csPath 컴퓨트 셰이더(Compute Shader)의 경로입니다.
	 */
	explicit Shader(const std::string& csPath);


	/**
	 * @brief 셰이더 파일을 읽어 셰이더 프로그램을 생성합니다.
	 *
	 * @param vsPath 버텍스 셰이더(Vertex Shader)의 경로입니다.
	 * @param fsPath 프레그먼트 셰이더(Fragment Shader)의 경로입니다.
	 */
	explicit Shader(const std::string& vsPath, const std::string& fsPath);


	/**
	 * @brief 셰이더 파일을 읽어 셰이더 프로그램을 생성합니다.
	 *
	 * @param vsPath 버텍스 셰이더(Vertex Shader)의 경로입니다.
	 * @param gsFile 지오메트리 셰이더의 확장자(.geom)를 포함한 경로입니다.
	 * @param fsPath 프레그먼트 셰이더(Fragment Shader)의 경로입니다.
	 */
	explicit Shader(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath);


	/**
	 * @brief 셰이더의 가상 소멸자입니다.
	 */
	virtual ~Shader();


	/**
	 * @brief 셰이더의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Shader);


	/**
	 * @brief 셰이더 내부에서 할당된 리소스의 할당을 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 셰이더를 파이프라인에 바인딩합니다.
	 */
	void Bind();


	/**
	 * @brief 셰이더를 바인딩 해제합니다.
	 */
	void Unbind();


	/**
	 * @brief 셰이더 내에 boolean 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, bool value);


	/**
	 * @brief 셰이더 내에 int 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, int32_t value);


	/**
	 * @brief 셰이더 내에 int 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const int32_t* bufferPtr, uint32_t count);


	/**
	 * @brief 셰이더 내에 float 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, float value);


	/**
	 * @brief 셰이더 내에 float  타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const float* bufferPtr, uint32_t count);


	/**
	 * @brief 셰이더 내에 2차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, const Vec2f& value);


	/**
	 * @brief 셰이더 내에 2차원 벡터 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const Vec2f* bufferPtr, uint32_t count);


	/**
	 * @brief 셰이더 내에 2차원 벡턴 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param x 유니폼 변수의 x 값입니다.
	 * @param y 유니폼 변수의 y 값입니다.
	 */
	void SetUniform(const std::string& name, float x, float y);


	/**
	 * @brief 셰이더 내에 2차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, const Vec2i& value);


	/**
	 * @brief 셰이더 내에 2차원 벡터 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const Vec2i* bufferPtr, uint32_t count);


	/**
	 * @brief 셰이더 내에 2차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param x 유니폼 변수의 x 값입니다.
	 * @param y 유니폼 변수의 y 값입니다.
	 */
	void SetUniform(const std::string& name, int32_t x, int32_t y);


	/**
	 * @brief 셰이더 내에 3차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, const Vec3f& value);


	/**
	 * @brief 셰이더 내에 3차원 벡터 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const Vec3f* bufferPtr, uint32_t count);


	/**
	 * @brief 셰이더 내에 3차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param x 유니폼 변수의 x 값입니다.
	 * @param y 유니폼 변수의 y 값입니다.
	 * @param z 유니폼 변수의 z 값입니다.
	 */
	void SetUniform(const std::string& name, float x, float y, float z);


	/**
	 * @brief 셰이더 내에 3차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, const Vec3i& value);


	/**
	 * @brief 셰이더 내에 3차원 벡터 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const Vec3i* bufferPtr, uint32_t count);


	/**
	 * @brief 셰이더 내에 3차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param x 유니폼 변수의 x 값입니다.
	 * @param y 유니폼 변수의 y 값입니다.
	 * @param z 유니폼 변수의 z 값입니다.
	 */
	void SetUniform(const std::string& name, int32_t x, int32_t y, int32_t z);


	/**
	 * @brief 셰이더 내에 4차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, const Vec4f& value);


	/**
	 * @brief 셰이더 내에 4차원 벡터 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const Vec4f* bufferPtr, uint32_t count);


	/**
	 * @brief 셰이더 내에 4차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param x 유니폼 변수의 x 값입니다.
	 * @param y 유니폼 변수의 y 값입니다.
	 * @param z 유니폼 변수의 z 값입니다.
	 * @param w 유니폼 변수의 w 값입니다.
	 */
	void SetUniform(const std::string& name, float x, float y, float z, float w);


	/**
	 * @brief 셰이더 내에 4차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, const Vec4i& value);


	/**
	 * @brief 셰이더 내에 4차원 벡터 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const Vec4i* bufferPtr, uint32_t count);


	/**
	 * @brief 셰이더 내에 4차원 벡터 타입의 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param x 유니폼 변수의 x 값입니다.
	 * @param y 유니폼 변수의 y 값입니다.
	 * @param z 유니폼 변수의 z 값입니다.
	 * @param w 유니폼 변수의 w 값입니다.
	 */
	void SetUniform(const std::string& name, int32_t x, int32_t y, int32_t z, int32_t w);


	/**
	 * @brief 셰이더 내에 2x2 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, const Mat2x2& value);


	/**
	 * @brief 셰이더 내에 2x2 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const Mat2x2* bufferPtr, uint32_t count);


	/**
	 * @brief 셰이더 내에 3x3 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, const Mat3x3& value);


	/**
	 * @brief 셰이더 내에 3x3 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const Mat3x3* bufferPtr, uint32_t count);


	/**
	 * @brief 셰이더 내에 4x4 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param value 유니폼 변수의 설정 값입니다.
	 */
	void SetUniform(const std::string& name, const Mat4x4& value);


	/**
	 * @brief 셰이더 내에 4x4 타입의 행렬 유니폼 변수를 설정합니다.
	 *
	 * @param name 유니폼 변수의 이름입니다.
	 * @param bufferPtr 유니폼 변수의 설정 값 목록의 포인터입니다.
	 * @param count 설정할 값의 수입니다.
	 */
	void SetUniform(const std::string& name, const Mat4x4* bufferPtr, uint32_t count);


protected:
	/**
	 * @brief 셰이더의 종류입니다.
	 */
	enum class EType : int32_t
	{
		Vertex     = 0x8B31,
		Control    = 0x8E88,
		Evaluation = 0x8E87,
		Geometry   = 0x8DD9,
		Fragment   = 0x8B30,
		Compute    = 0x91B9,
	};


	/**
	 * @brief 셰이더 내 유니폼 객체의 위치를 얻습니다.
	 *
	 * @param name 위치를 찾을 셰이더 내 유니폼 이름입니다.
	 *
	 * @return 셰이더 내 유니폼 객체의 위치를 반환합니다. 찾지 못하면 -1을 반환합니다.
	 *
	 * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetUniformLocation.xhtml
	 */
	int32_t GetUniformLocation(const std::string& name);

	
	/**
	 * @brief 셰이더 오브젝트를 생성합니다.
	 *
	 * @note 셰이더 객체 생성에 성공하면 0이 아닌 값을 반환합니다.
	 *
	 * @param type 셰이더 타입입니다.
	 * @param path 셰이더 파일의 경로입니다.
	 * @param entryPoint 셰이더의 시작 지점입니다. 기본 값은 main입니다.
	 *
	 * @return 생성된 셰이더 객체의 아이디를 반환합니다.
	 */
	uint32_t CreateShader(const EType& type, const std::string& path, const char* entryPoint = "main");


	/**
	 * @brief 셰이더 프로그램을 생성합니다.
	 * 
	 * @note 셰이더 프로그램 생성에 성공하면 0이 아닌 값을 반환합니다.
	 * 
	 * @param shaderIDs 셰이더 프로그램에 추가할 셰이더 ID 목록입니다.
	 * 
	 * @return 생성된 셰이더 프로그램 객체의 아이디를 반환합니다.
	 */
	uint32_t CreateProgram(const std::vector<uint32_t>& shaderIDs);

	
protected:
	/**
	 * @brief 셰이더 프로그램의 아이디입니다.
	 */
	uint32_t programID_ = 0;


	/**
	 * @brief 유니폼 버퍼의 위치값입니다.
	 */
	std::map<std::string, uint32_t> uniformLocationCache_;
};