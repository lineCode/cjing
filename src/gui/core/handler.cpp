#include "handler.h"
#include"core\debug.h"
#include"gui\widget\widget.h"
#include"gui\core\dispatcher.h"

namespace gui
{

static std::unique_ptr<class EventHandler> mHandler = nullptr;

/**
*	\brief �¼�������
*
*	event handerά�����ӵ�Dispatcher��ͬʱ����inputevent�������¼���ͬʱ
*	���¼��ַ��������˵�dispatcher
*/
class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	void Connect(Dispatcher* dispatcher);
	void Disconnect(Dispatcher* Dispatcher);

	void Active();

private:
	std::vector<Dispatcher*> mDispatcher;

};

EventHandler::EventHandler():
	mDispatcher()
{
}

EventHandler::~EventHandler()
{
	mDispatcher.clear();
}

/**
*	\brief ����dispatcher,Dispatcher���뱣֤Ψһ��
*/
void EventHandler::Connect(Dispatcher * dispatcher)
{
	Debug::CheckAssertion(std::find(mDispatcher.begin(), mDispatcher.end(), dispatcher) == mDispatcher.end(),
		"dispatcher had already connected. ");

	mDispatcher.push_back(dispatcher);
}

void EventHandler::Disconnect(Dispatcher * Dispatcher)
{
	auto itr = std::find(mDispatcher.begin(), mDispatcher.end(), Dispatcher);
	Debug::CheckAssertion(itr != mDispatcher.end());

	mDispatcher.erase(itr);

	/** ��ɾ��һ��widget��������widget�ַ�active�¼� */
	Active();
}

void EventHandler::Active()
{
	for (auto dispatcher: mDispatcher)
	{
		//dispatcher->Fire(UI_EVENT_UNKNOW, std::<Widget&>(*dispatcher));
	}
}

/***** ȫ�ֺ��� *****/
void ConnectDispatcher(Dispatcher * dispatcher)
{
	Debug::CheckAssertion(mHandler != nullptr, 
		"event handler have not exist.");
	Debug::CheckAssertion(dispatcher != nullptr, 
		"the Dispatcher connect to event handler is null.");
	mHandler->Connect(dispatcher);
}

void DisconnectDispatcher(Dispatcher * dispatcher)
{
	Debug::CheckAssertion(mHandler != nullptr, 
		"event handler have not exist.");
	Debug::CheckAssertion(dispatcher != nullptr, 
		"the Dispatcher disconnect to event handler is null.");
	mHandler->Disconnect(dispatcher);
}


}