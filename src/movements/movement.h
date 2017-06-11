#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include"common\common.h"
#include"utils\point.h"
#include"utils\rectangle.h"
#include"lua\luaRef.h"
#include"lua\luaObject.h"

class Drawable;

/**
*	\brief �ƶ�����ĳ������,������Ϊ����ƶ�drawable��entity
*/
class Movement : public LuaObject
{
public:
	virtual ~Movement();

	virtual void Update();
	bool IsSuspended()const;
	virtual void SetSuspended(bool suspended);
	
	// status
	Point2 GetPos()const;
	void SetX(const int x);
	void SetY(const int y);
	void SetPos(const Point2& pos);
	void TranslateX(int x);
	void TranslateY(int y);
	void TranslatePos(const Point2& dxy);

	// move
	virtual bool IsFinished()const;
	virtual void Stop();

	// lua
	void NotifyMovementChanged();
	void NotifyMovementFinished();
	virtual const string GetLuaObjectName()const override;

	// test collision
	bool TestCollisionWithObstacles(const Point2& dxy)const;
	bool IsIngoredObstacles()const;
	void SetIngoredObstacles(bool ingoredObstacles);

protected:
	Movement();
	Movement(bool ingoreObstacles);

private:
	Point2 mPos;
	bool mFinished;
	bool mSuspended;
	uint32_t mWhenSuspendedTime;
	bool mIngoreObstacle;


	Drawable* mDrawable;

	LuaRef mFinishedCallBack;
	
};

using MovementPtr = std::shared_ptr<Movement>;

#endif