#ifndef _ANIMATION_SPRITE_H_
#define _ANIMATION_SPRITE_H_

#include"common\common.h"
#include"game\sprite.h"
#include"game\animationSet.h"
#include"lua\luaRef.h"
#include<map>

/**
*	\brief �������飬���Լ��ض����ļ������ž���֡����
*/
class AnimationSprite : public Sprite
{
public:
	//AnimationSprite();
	AnimationSprite(const string& id);
	~AnimationSprite();

	// system
	virtual void Update();
	virtual void Draw();
	virtual bool IsAnimationed()const;
	
	// frame status
	bool IsFrameChanged()const;
	void SetFrameChanged(bool changed);
	void SetCurrFrame(int currFrame);
	int  GetCurrFrame()const;
	bool IsFrameFinished()const;
	bool IsFrameStarted()const ;
	void SetFrameDelay(uint32_t delay);
	uint32_t GetFrameDelay()const;
	int GetNextFrame()const;
	int GetNumFrames()const;

	// direction
	int GetCurrDirection()const;
	int GetNumDirections()const;

	// animation status
	void SetCurrAnimation(const string& name);
	void SetCurrAnimationSetId(const string& id);
	string GetCurrAnimationSetId()const;

	// notify
	void NotifyFinished();

private:
	void SetDataFromAnimation(const Animation& animation);

	AnimationSet& GetAnimationSet(const string& id);
	void UpdateFramedChanged();

	std::map<string, AnimationSet*> mAllAnimationSets;
	AnimationSet& mCurrAnimationSet;

	string mCurrAnimationName;
	Animation* mCurrAnimation;	// �������mCurrAnimationSet�е�animationָ�룬�����������

	uint32_t mFrameDelay;
	uint32_t mNextFrameDate;
	int      mFrameLoop;
	int      mCurrFrame;
	int      mFrameNum;
	int      mCurrDirection;
	bool     mFrameChanged;
	bool     mFrameFinished;

};

using AnimationSpritePtr = std::shared_ptr<AnimationSprite>;

#endif