#ifndef _GL_PROGRAM_STATE_H_
#define _GL_PROGRAM_STATE_H_

#include"common\common.h"
#include"core\glProgram.h"
#include"utils\vec3.h"

#include<unordered_map>

/**
*	\brief uniform值
*/
class UniformValue
{
public:
	UniformValue();
	UniformValue(Uniform* uniform, GLProgram* program);
	~UniformValue();

	void Apply();

	// uniform
	void SetUniform(Uniform* uniform);
	void SetInteger(int value);
	void SetFloat(float value);
	void SetVec3i(const Vec3i& vec3i);
	void SetVec3f(const Vec3f& vec3f);
	void SetMat4(const Matrix4& mat);
	void SetTexture(GLuint textureId);

private:
	union  U
	{
		float flaotValue;
		int   intValue;
		int   intArray[3];
		float floatArray[3];
		float matrixValue[16];
		GLuint mTextureId;

		U() { memset(this, 0, sizeof(*this)); }
		~U() {}
		U&operator=(const U&other) {
			memcpy(this, &other, sizeof(*this));
			return *this;
		}
	}mValue;
	Uniform* mUniform;	
	GLProgram* mProgram;

};

/**
*	\brief 着色器状态
*/
class GLProgramState
{
public:
	GLProgramState();
	~GLProgramState();
	
	void Apply();
	void ApplyProgram();
	void ApplyAttibutes();
	void ApplyUniforms();
	
	UniformValue* GetUniform(const string& key);
	UniformValue* GetUniform(const GLint& key);

	void Set(GLProgramPtr program);
	GLuint GetProgram()const;

	// uniform
	void SetUniform1i(const string&name, int i1);
	void SetUniform3i(const string&name, const Vec3i& vec3i);
	void SetUniform1f(const string&name, float f1);
	void SetUniform3f(const string&name, const Vec3f& vec3f);
	void SetUniformMatrix4(const string& name, const Matrix4& mat);

	static const string DEFAULT_SPRITE_NORMAL_PROGRAMSTATE_NAME;
	static const string DEFAULT_FONT_NORMAL_PROGRAMSTATE_NAME;

private:
	void Init(GLProgramPtr program);
	void ResetProgram();

	bool mUniformValueDirty;
	
	GLProgramPtr mProgram;
	std::unordered_map<string, GLint> mUniformNames;	/* 以uniform name 保存 uniform location */
	std::unordered_map<GLint, UniformValue>mUniforms;	/* 以uniform location 保存uniform */

};

using GLProgramStatePtr = std::shared_ptr < GLProgramState >;

#endif