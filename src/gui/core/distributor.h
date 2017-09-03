#pragma once

#include"gui\core\dispatcher.h"


/**
*	\brief distributor ���ڴ���������ض���widget�ĸ߲��¼�
*	�����������¼��������ͣ�¼���
*/
namespace gui
{	

class Widget;

class MouseMotion 
{
public:
	MouseMotion(Widget& widget, Dispatcher::queue_position position);
	~MouseMotion();


protected:
	void MouseEnter(Widget* widget);
	void MouseLeave();
	void MouseHover(Widget* widget, const Point2& coords);

private:
	void SignalHandleMouseMotion(const ui_event event, bool&handle, const Point2& coords);

	Widget& mOwner;
	Widget* mMouseFocus;

	bool mMouseCaptured;
	bool mSignalHandelMouseMotionEntered;

};

class Distributor : public MouseMotion
{
public:
	Distributor(Widget& widget, Dispatcher::queue_position position);
	~Distributor();



};

}