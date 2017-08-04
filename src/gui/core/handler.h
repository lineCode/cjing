#pragma once

#include"common\common.h"

namespace gui
{

class Widget;
class Dispatcher;

enum ui_event
{
	UI_EVENT_UNKNOW,
	UI_EVENT_MOUSE_MOTION,
	UI_EVENT_MOUSE_LEFT_BUTTON_DOWN,
	UI_EVENT_MOUSE_RIGHT_BUTTON_DOWN,
	UI_EVENT_MOUSE_MIDDLE_BUTTON_DOWN,
	UI_EVENT_MOUSE_LEFT_BUTTON_UP,
	UI_EVENT_MOUSE_RIGHT_BUTTON_UP,
	UI_EVENT_MOUSE_MIDDLE_BUTTON_UP,
	UI_EVENT_KEY_DOWN,
	UI_EVENT_KEY_UP,
};

/**
*	\brief ��һ��dispatcher���ӵ�event handlder
*/
void ConnectDispatcher(Dispatcher* Dispatcher);

/**
*	\brief ��һ��dispatcher��event handlder�н������
*/
void DisconnectDispatcher(Dispatcher* dispatcher);

}