#ifndef _ANIMATION_SPRITE_H_
#define _ANIMATION_SPRITE_H_

#include"common\common.h"
#include"game\sprite.h"
#include"game\spriteAnimation.h"


/**
*	\brief �������飬���Լ��ض����ļ������ž���֡����
*/
class AnimationSprite : public Sprite
{
public:
	AnimationSprite(const string& id);
};


#endif