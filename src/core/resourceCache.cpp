#include"resourceCache.h"

ResourceCache::ResourceCache():
	mPrograms()
{
}

ResourceCache::~ResourceCache()
{
	Clear();
}

/**
*	\brief ����ResourceCacheΨһʵ��
*/
ResourceCache & ResourceCache::GetInstance()
{
	static ResourceCache instance;
	return instance;
}

void ResourceCache::Clear()
{
	mPrograms.clear();
	mProgramStates.clear();
	mTextures.clear();
}

/**
*	\brief ����Ĭ�ϵ���ɫ������
*/
void ResourceCache::LoadDefaultProgram()
{
	// ����Ĭ�ϵ�sprite program
	auto program = std::make_shared<GLProgram>();
	program->InitWithFileNames(GLProgram::DEFAULT_SPRITE_NORMAL_PROGRAM_NAME + ".vs",
		GLProgram::DEFAULT_SPRITE_NORMAL_PROGRAM_NAME + ".frag");
	program->Link();
	mPrograms[GLProgram::DEFAULT_SPRITE_NORMAL_PROGRAM_NAME] = program;

	// ����Ĭ�ϵ�����program

}

/**
*	\brief ����ָ��key����ɫ������
*
*  ���δ�ҵ����򷵻�nullptr
*/
void ResourceCache::AddGLProgram(GLProgramPtr ptr, const string & key)
{
	if (ptr == nullptr)
		return;
	mPrograms[key] = ptr;
}

/**
*	\brief ����ָ��key����ɫ������
*
*  ���δ�ҵ����򷵻�nullptr
*/
GLProgramPtr ResourceCache::GetGLProgram(const string & key)
{
	auto& it = mPrograms.find(key);
	if (it != mPrograms.end())
		return it->second;
	return nullptr;
}

/**
*	\brief ����Ĭ�ϵ�ProgramState
*
*	 ��֤DefaultProgram�Ѿ����غ�
*/
void ResourceCache::LoadDefaultProgramState()
{
	// ����Ĭ�ϵ�sprite program state
	auto newProgramState = std::make_shared<GLProgramState>();
	newProgramState->Set(GetGLProgram(GLProgram::DEFAULT_SPRITE_NORMAL_PROGRAM_NAME));
	mProgramStates[GLProgramState::DEFAULT_SPRITE_NORMAL_PROGRAMSTATE_NAME] = newProgramState;
}

/**
*	\brief ���һ���µ���ɫ��״̬
*
*   ����ͬ���������
*/
void ResourceCache::AddGLProgramState(GLProgramStatePtr ptr, const string & key)
{
	if (ptr == nullptr)
		return;
	mProgramStates[key] = ptr;
}

/**
*	\brief ����ָ��key����ɫ��״̬
*
*  ���δ�ҵ����򷵻�nullptr
*/
GLProgramStatePtr ResourceCache::GetGLProgramState(const string & key)
{
	auto& it = mProgramStates.find(key);
	if (it != mProgramStates.end())
		return it->second;
	return nullptr;
}

/**
*	\brief ����һ������
*
*   ��ͬ�����ǣ�������Ѿ�������ͬ����������ֱ�ӷ��ض�Ӧ����
*/
TexturePtr ResourceCache::LoadTexture2D(const string & texname)
{
	TexturePtr findTex = GetTexture2D(texname);
	if (findTex != nullptr)
		return findTex;

	auto newTex = std::make_shared<Texture2D>();
	newTex->InitWithFile(texname);
	mTextures[texname] = newTex;
	return newTex;
}

/**
*	\brief ����ָ��key������
*
*  ���δ�ҵ����򷵻�nullptr
*/
TexturePtr ResourceCache::GetTexture2D(const string & texnname)
{
	auto& it = mTextures.find(texnname);
	if (it != mTextures.end())
		return it->second;
	return nullptr;
}


