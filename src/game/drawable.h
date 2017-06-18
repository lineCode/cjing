#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include"common\common.h"
#include"movements\movement.h"
#include"utils\point.h"
#include"lua\luaObject.h"

/**
*	\brief �ṩλ�����ԵĻ������Ƶ�Ԫ
*/
class Drawable : public LuaObject
{
public:
	~Drawable();

	virtual void Update();
	virtual void Draw() = 0;

	Point2 GetPos()const;
	void SetPos(const Point2& pos);
	float GetRotated()const;
	void SetRotated(float angle);
	float GetGlobalOrder()const;
	void SetGlobalOrder(float order);

	virtual void SetSuspended(bool suspended);
	bool IsSuspended()const;

	// lua
	virtual const string GetLuaObjectName()const ;

protected:
	Drawable();

	Point2 mPos;
	float mRotate;
	float mGlobalZOrder;	// ȫ�ִ��򣬾�������Ⱦʱ�Ļ��ƴ���
	float mLocalZOrder;		// �ֲ���������Ϊ�ӽڵ�ʱ�Ĳ�������
	bool mSuspended;		

	std::vector<MovementPtr> mMovements;

};



#endif