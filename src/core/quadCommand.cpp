#include "quadCommand.h"

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
void QuadCommand::Init(float globalOrder, GLuint textureID, Quad * quads, int quadCounts, const BlendFunc & blendFunc)
{
	if(mTextureID != textureID || mBlendFunc != blendFunc)
		GenerateShadeState();
}

/**
*	\brief ʹ�õ�ǰ��ɫ״̬������Ⱦ
*/
void QuadCommand::UseShade()
{
	// ʹ����ɫ��

	// ������

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

/**
*	\brief ���ɵ�ǰ��ɫ״̬
*
*    ������Id�����ģʽ����ɫ��ͨ��hash��������32λ�޷�������
*/
void QuadCommand::GenerateShadeState()
{
	mShadeState = 0;
}
