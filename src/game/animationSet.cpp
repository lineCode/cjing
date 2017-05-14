#include "animationSet.h"
#include "core\debug.h"

AnimationSet::AnimationSet()
{
}

AnimationSet::AnimationSet(const string & name):
	mAnimationsSetName(name)
{
	InitAnimationSetWithFile(name);
}

AnimationSet::~AnimationSet()
{
	mAnimations.clear();
}

bool AnimationSet::HasAnimation(const string & name)const
{
	auto animation = mAnimations.find(name);
	return (animation != mAnimations.end());
}

string AnimationSet::GetAnimationSetName() const
{
	return mAnimationsSetName;
}

string AnimationSet::GetAnimationDefaultName() const
{
	return mAnimationDefaultName;
}

/**
*	\brief ����ָ��name��animation
*	\return const Animation&
*/
const Animation& AnimationSet::GetAnimation(const string & name)const
{
	Debug::CheckAssertion(HasAnimation(name),"Invalid animation name.");
	return mAnimations.find(name)->second;
}

/**
*	\brief ����ָ��name��animation
*	\return Animation&
*/
Animation & AnimationSet::GetAnimation(const string & name)
{
	Debug::CheckAssertion(HasAnimation(name), "Invalid animation name.");
	return mAnimations.find(name)->second;
}

/**
*	\brief ��Ӿ���
*
*	Ĭ�ϼ�ֵnameΨһ
*/
void AnimationSet::AddAnimation(const string & name, const Animation & animation)
{
	Debug::CheckAssertion(HasAnimation(name), "Invalid animation name.");
	mAnimations[name] = animation;
}

/**
*	\brief ��Ӿ���
*
*	Ĭ�ϼ�ֵnameΨһ
*/
void AnimationSet::AddAnimation(const string & name, const AnimationData & animationData)
{
	Debug::CheckAssertion(HasAnimation(name), "Invalid animation name.");
	mAnimations.emplace(name, Animation());
}

/**
*	\brief ���ļ��г�ʼ��animationSet
*/
void AnimationSet::InitAnimationSetWithFile(const string & name)
{
	Debug::CheckAssertion(mAnimations.empty(), "Animation set already loaded.");

	string fileName = string("sprites") + name + ".dat";
	AnimationLuaData data;
	bool success = data.ImportFromFile(name);
	if (success)
	{

	}

}
