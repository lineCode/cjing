#ifndef _ANIMATION_SPRITE_H_
#define _ANIMATION_SPRITE_H_

#include"common\common.h"
#include"game\sprite.h"
#include"game\animationSet.h"

/**
*	\brief �������飬���Լ��ض����ļ������ž���֡����
*/
class AnimationSprite : public Sprite
{
public:
	AnimationSprite(const string& id);
	~AnimationSprite();

	// system
	virtual void Update();
	virtual void Draw();
	virtual bool IsAnimationed()const;

	bool IsFrameChanged()const;

	// status


private:
	void UpdateFramedChanged();

	uint32_t mFrameDelay;
	uint32_t mNextFrameDate;
	int      mFrameLoop;
	int      mCurrFrame;
	int      mFrameNum;
	bool     mFrameChanged;
};


#endif