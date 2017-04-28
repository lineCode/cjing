#include "quadCommand.h"
#include "core\debug.h"
#include "utils\xxhash.h"


QuadCommand::QuadCommand():
	RenderCommand(COMMAND_QUAD,0.0f),
	mTextureID(0),
	mQuadCounts(0),
	mQuads(nullptr),
	mShadeState(0)
{
}

QuadCommand::~QuadCommand()
{
}

/**
*	\brief ��ʼ��quadCommand
*/
void QuadCommand::Init(float globalOrder, GLProgramStatePtr programState, GLuint textureID, Quad * quads, int quadCounts, const BlendFunc & blendFunc)
{
	Debug::CheckAssertion(programState != nullptr, "Invalid programState in QuadCommand::Init().");

	mGlobalOrder = globalOrder;
	mProgramState = programState;
	mQuadCounts = quadCounts;
	mQuads = quads;

	if (mTextureID != textureID || mBlendFunc != blendFunc)
	{
		mTextureID = textureID;
		mBlendFunc = blendFunc;
		GenerateShadeState();
	}
}

/**
*	\brief ʹ�õ�ǰ��ɫ״̬������Ⱦ
*/
void QuadCommand::UseShade()
{
	Debug::CheckAssertion(mProgramState != nullptr, "Invalid program");
	// ʹ����ɫ��
	mProgramState->Apply();

	// ������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// �趨���ģʽ
	glBlendFunc(mBlendFunc.srcBlend, mBlendFunc.dstBlend);
}

int QuadCommand::GetQuadCounts() const
{
	return mQuadCounts;
}

const Quad* QuadCommand::GetQuads() const
{
	return mQuads;
}

uint32_t QuadCommand::GetShadeState() const
{
	return mShadeState;
}

BlendFunc QuadCommand::GetBlendFunc() const
{
	return mBlendFunc;
}

Matrix4 QuadCommand::GetTransfomr() const
{
	return mTransform;
}

/**
*	\brief ���ɵ�ǰ��ɫ״̬
*
*    ������Id�����ģʽ����ɫ��ͨ��hash��������32λ�޷�������
*/
void QuadCommand::GenerateShadeState()
{
	int program = mProgramState->GetProgram();
	int hashArray[4] = { program,(int)mTextureID,mBlendFunc.srcBlend ,mBlendFunc.dstBlend };

	mShadeState = XXH32((const void*)hashArray, sizeof(hashArray), 0);
}
