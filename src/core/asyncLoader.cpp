#include "asyncLoader.h"
#include "core\debug.h"
#include "core\logger.h"
#include "lua\luaContext.h"

#include <asyncinfo.h>

AsyncLoader::AsyncLoader():
	mFinishCallBack(),
	mTaskQueue(),
	mMaxTimeLimt(0),
	mUpdateStep(0),
	mUsedTime(0),
	mStartd(false),
	mFinished(false),
	mHRCShareing(nullptr),
	mHDC(nullptr)
{
	HWND hwnd = GetActiveWindow();
	if (hwnd == NULL)
		Debug::Die("Can not get the window's hwnd in AsyncLoader.");
	mHDC = GetDC(hwnd);
	if (mHDC == NULL)
		Debug::Die("Can not get the window's dc in AsyncLoader.");
	mHRCShareing = wglCreateContext(mHDC);
	if (mHRCShareing == NULL)
		Debug::Die("Can not get the window's hrc.");
	wglShareLists(wglGetCurrentContext(), mHRCShareing);
}

AsyncLoader::~AsyncLoader()
{
	if (mHRCShareing != nullptr)
	{
		wglDeleteContext(mHRCShareing);
		mHRCShareing = nullptr;
	}
}

/**
*	\brief ֹͣ��ǰ����
*
*	��ǰ����ǿ�˱�������
*/
void AsyncLoader::Stop()
{
	if (mStartd && !mFinished)
	{
		std::future_status status = mFuture.wait_for(std::chrono::milliseconds(mUpdateStep / 2));
		if (status != std::future_status::ready)
		{
			Debug::Warning("The future is be killed.");
			mFuture.~future();
		}
		//Debug::Warning("The future is be killed.");
		//mFuture.~future();
	}
}

void AsyncLoader::Update()
{
	if (!mStartd || mFinished)
	{
		return;
	}
	std::future_status status = mFuture.wait_for(std::chrono::milliseconds(mUpdateStep / 2));
	if (status == std::future_status::ready)
	{
		//Logger::Info("Async load finished.");
		mFinished = true;
		if (mFinishCallBack)
		{
			mFinishCallBack();
		}
	}
	mUsedTime += mUpdateStep;
	if (mMaxTimeLimt > 0 && mUsedTime >= mMaxTimeLimt)
	{
		Debug::Die("Time out to async load.");
	}
}

/**
*	 \brief ִ���첽����
*
*	 ��ΪOpengl����Ⱦ�豸Ĭ�϶��̻߳��ȡ����,������Ҫ����
*	 һ����Ⱦ������(hRC)������֮���ڶ��߳���ʹ��
*/
void AsyncLoader::Run(uint32_t maxTimeLimt)
{
	if (mFinished || mHRCShareing == nullptr)
	{
		return;
	}
	Logger::Info("Async loading start.");
	mStartd = true;
	mMaxTimeLimt = maxTimeLimt;
	mFuture = std::async(std::launch::async, [&]() {
		wglMakeCurrent(this->GetHDC(), this->GetHGLRC());

		auto& taskQueue = this->GetTaksQueue();
		while (!taskQueue.empty())
		{
			auto task = taskQueue.front();
			if(task != nullptr)
				task();
			taskQueue.pop();
		}

		wglMakeCurrent(NULL, NULL);
	});
}

/**
*	\brife ���������
*/
void AsyncLoader::AddTask(const TaskFunction & task)
{
	mTaskQueue.push(task);
}

/**
*	\brife ���������
*/
void AsyncLoader::AddTask(const LuaRef & callback)
{
	auto taskFunction = [=]() {
		LuaContext* luaContext = GetLuaContext();
		if (luaContext != nullptr)
		{
			callback.Call("movment finished callback");
		}
	};
	mTaskQueue.push(taskFunction);
}

/**
*	\brief ���ý����ص�����
*/
void AsyncLoader::SetFinishCallBack(const std::function<void()>& finisCallBack)
{
	if (IsStarted())
	{
		Debug::Warning("Can not set finished callback when loader had start.");
		return;
	}
	mFinishCallBack = finisCallBack;
}

/**
*	\brief ���ý����ص�����
*/
void AsyncLoader::SetFinishCallBack(const LuaRef & callback)
{
	if (IsStarted())
	{
		Debug::Warning("Can not set finished callback when loader had start.");
		return;
	}
	auto finisCallBack = [=]() {
		LuaContext* luaContext = GetLuaContext();
		if (luaContext != nullptr)
		{
			callback.Call("movment finished callback");
		}
	};
	mFinishCallBack = finisCallBack;
}

void AsyncLoader::ClearTaskQueue()
{
	while (!mTaskQueue.empty())
	{
		mTaskQueue.pop();
	}
}

const string AsyncLoader::GetLuaObjectName() const
{
	return LuaContext::module_async_loader_name;
}
