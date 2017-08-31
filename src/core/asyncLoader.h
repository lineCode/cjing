#pragma once

#include"common\common.h"
#include"core\font.h"
#include"core\fontAtlas.h"
#include"core\resourceCache.h"
#include"game\timer.h"
#include"game\currentQuest.h"
#include"lua\luaObject.h"
#include"lua\luaRef.h"

#include<thread>
#include<functional>
#include<queue>
#include<future>

/**
*	\brief ����һ���߳������첽������Ҫ����Դ
*
*	Ŀǰ���Ǵ���һ��������У�������������ʱ����ִ����ɻص�����
*	��ʱ����Ҫ���ǻ����������ΪԼ�������ⲿ�ȴ����ص�ʱ�򲻻�ʹ��
*	��ͬ����Դ
*
*	�첽������Դ,���첽����ԴΪ���塢��default֮���������Ƶ����
*	����Ŀǰ�첽���ض�����lua�д���,���е�lua������taskfunction��װ
*	ͳһ����
*
*	���ȴ�ʱ�����ʱ��Ӧִ�ж�Ӧ�Ļص�����
*/

class AsyncLoader : public LuaObject
{
	using TaskFunction = std::function<void()>;
public:
	AsyncLoader();
	~AsyncLoader();

	void Stop();
	void Update();
	void Run(uint32_t maxTimeLimt = 0);
	
	void AddTask(const TaskFunction& task);
	void AddTask(const LuaRef& callback);

	void SetFinishCallBack(const std::function<void()>& finisCallBack);
	void SetFinishCallBack(const LuaRef& callback);

	void ClearTaskQueue();

	virtual const string GetLuaObjectName()const;

	/**** **** setter/gettter **** ****/
	bool IsTaskListEmpty()const
	{
		return mTaskQueue.empty();
	}
	bool IsStarted()const
	{
		return mStartd;
	}

	bool IsFinished()const
	{
		return mFinished;
	}

	std::queue<TaskFunction>& GetTaksQueue()
	{
		return mTaskQueue;
	}

	void SetUpdateStep(uint32_t step)
	{
		mUpdateStep = step;
	}

	HGLRC GetHGLRC() const
	{
		return mHRCShareing;
	}

	HDC GetHDC() const
	{
		return mHDC;
	}

private:
	std::function<void()> mFinishCallBack;		/** �����ص����� */

	std::queue<TaskFunction> mTaskQueue;		/** ������У������������Ϊ��ʱִ�н����ص����� */

	uint32_t mMaxTimeLimt;		/** ���ִ��ʱ�� */
	uint32_t mUpdateStep;
	uint32_t mUsedTime;

	bool mStartd;				/** �Ƿ��Ѿ���ʼ��� */			
	bool mFinished;
	std::future<void> mFuture;

	HGLRC mHRCShareing;			/** opengl���߳��µĹ�����Ⱦ������ */
	HDC   mHDC;
};