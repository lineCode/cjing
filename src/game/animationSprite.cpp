#include"animationSprite.h"

AnimationSprite::AnimationSprite(const string & id)
{
}

AnimationSprite::~AnimationSprite()
{
}

void AnimationSprite::Update()
{
}

/**
*	\brief ���ƶ�������
*
*   ����ʱ��Ҫ�����Ƿ�����֡�任
*/
void AnimationSprite::Draw()
{
	if (IsFrameChanged())
		UpdateFramedChanged();

	Sprite::Draw();
}

/**
*	\brief �Ƿ��Ƕ������飬AnimationSprite��Ȼ����false;
*/
bool AnimationSprite::IsAnimationed() const
{
	return true;
}

/**
*	\brief �Ƿ���֡�任
*/
bool AnimationSprite::IsFrameChanged() const
{
	return mFrameChanged;
}

/**
*	\brief ���±任��֡
*/
void AnimationSprite::UpdateFramedChanged()
{
}
