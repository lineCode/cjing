#include "movement.h"
#include "game\drawable.h"
#include "core\system.h"
#include "lua\luaContext.h"


Movement::Movement():
	mPos(Point2(0,0)),
	mFinished(false),
	mSuspended(false),
	mDrawable(nullptr),
	mWhenSuspendedTime(0),
	mIngoreObstacle(false)
{
}

Movement::Movement(bool ingoreObstacles):
	mPos(Point2(0, 0)),
	mFinished(false),
	mSuspended(false),
	mDrawable(nullptr),
	mWhenSuspendedTime(0),
	mIngoreObstacle(ingoreObstacles)
{
}

Movement::~Movement()
{
}

/**
*	\brief ÿ֡ˢ���¼�
*
*    movement�����update������������ã�������Ҫ���Ƿ�finished���д���
*/
void Movement::Update()
{
	if (!mFinished && IsFinished() )
	{
		mFinished = true;
		NotifyMovementFinished();
	}
	else if (mFinished && !IsFinished())
	{
		mFinished = false;
	}
}

bool Movement::IsSuspended() const
{
	return false;
}

void Movement::SetSuspended(bool suspended)
{
	if (suspended != mSuspended)
	{
		mSuspended = suspended;
		if (suspended)
		{
			mWhenSuspendedTime = System::Now();
		}
	}
}

/**
*	\brief �ƶ��Ƿ����
*
*	��������������Ƿ���ɵ������ж�
*/
bool Movement::IsFinished() const
{
	return mFinished;
}

/**
*	\brief ��Ӧ�ƶ��任
*/
void Movement::NotifyMovementChanged()
{
}

/**
*	\brief ��Ӧ�ƶ����
*/
void Movement::NotifyMovementFinished()
{
	LuaContext* luaContext = GetLuaContext();
	if (luaContext != nullptr )
	{
		mFinishedCallBack.Call("movment finished callback");
	}
}

void Movement::NotifyObstacleReached()
{

}

const string Movement::GetLuaObjectName() const
{
	return LuaContext::module_movement_name;
}

/**
*	\brief �����Ƿ�������ײ
*	\param dxy ��Ե�ǰλ�õ�ƫ��ֵ
*	\return �޷�����ײ����False,��������ײ����true
*/
bool Movement::TestCollisionWithObstacles(const Point2 & dxy) const
{
	if (mIngoreObstacle || mEntity == nullptr)
	{
		return false;
	}

	//Map map = GetMap();
	//Rect rect = GetBoundsRect();
	//rect.translate(dxy);

	//bool collision = map->TestCollisionWithObstacles(rect, *mEntity);

	bool collision = false;
	
	return collision;
}

bool Movement::IsIngoredObstacles() const
{
	return mIngoreObstacle;
}

void Movement::SetIngoredObstacles(bool ingoredObstacles)
{
	mIngoreObstacle = ingoredObstacles;
}

void Movement::Stop()
{
	mFinished = true;
}

bool Movement::IsStop() const
{
	return !IsStarted();
}

bool Movement::IsStarted() const
{
	return false;
}

Point2 Movement::GetPos() const
{
	return mPos;;
}

void Movement::SetX(const int x)
{
	mPos.x = x;
}

void Movement::SetY(const int y)
{
	mPos.y = y;
}

void Movement::SetPos(const Point2 & pos)
{
	mPos = pos;
}

void Movement::TranslateX(int x)
{
	mPos.x += x;
}

void Movement::TranslateY(int y)
{
	mPos.y += y;
}

void Movement::TranslatePos(const Point2 & dxy)
{
	mPos += dxy;
}

/**
*	\brief ��ȡ�ݶ�ʱ��ʱ��
*/
uint32_t Movement::GetWhenSuspeneded() const
{
	return mWhenSuspendedTime;
}

/**
*	\brief ���õ�ǰ�ƶ���drwable����
*/
void Movement::SetDrawable(Drawable * drawable)
{
	mDrawable = drawable;
	if (mDrawable == nullptr)
	{
		mPos = {0, 0};
	}
	else
	{
		mPos = mDrawable->GetPos();
		NotifyMovementChanged();
	}
}
