#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include"common\common.h"
#include"movements\movement.h"
#include"utils\point.h"

/**
*	\brief �ṩλ�����ԵĻ������Ƶ�Ԫ
*/
class Drawable
{
public:
	virtual ~Drawable();

	virtual void Update();
	virtual void Draw() = 0;

	Point2 GetPos()const;
	void SetPos(const Point2& pos);
	float GetRotated()const;
	void SetRotated(float angle);
	float GetGlobalOrder()const;
	void SetGlobalOrder(float order);

protected:
	Drawable();

	Point2 mPos;
	float mRotate;
	float mGlobalZOrder;
	bool mSuspended;

	std::vector<MovementPtr> mMovements;

};



#endif