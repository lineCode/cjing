#include "shadeResource.h"

ShadeResourceCache::ShadeResourceCache():
	mPrograms()
{
}

ShadeResourceCache::~ShadeResourceCache()
{
	mPrograms.clear();
}

/**
*	\brief ����ShadeResourceCacheΨһʵ��
*/
ShadeResourceCache & ShadeResourceCache::GetInstance()
{
	static ShadeResourceCache instance;
	return instance;
}

/**
*	\brief ����Ĭ�ϵ���ɫ������
*/
void ShadeResourceCache::LoadDefaultProgram()
{
	// ����Ĭ�ϵ�sprite program
	auto program = std::make_shared<GLProgram>();
	program->InitWithFileNames(GLProgram::DEFAULT_SPRITE_NORMAL_PROGRAM_NAME + ".vert",
		GLProgram::DEFAULT_SPRITE_NORMAL_PROGRAM_NAME + ".frag");
	mPrograms[GLProgram::DEFAULT_SPRITE_NORMAL_PROGRAM_NAME] = program;

	// ����Ĭ�ϵ�����program

}

/**
*	\brief ����ָ��key����ɫ������
*
*  ���δ�ҵ����򷵻�nullptr
*/
GLProgramPtr ShadeResourceCache::GetGLProgram(const string & key)
{
	auto& it = mPrograms.find(key);
	if (it != mPrograms.end())
		return it->second;
	return nullptr;
}


