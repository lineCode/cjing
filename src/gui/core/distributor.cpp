#include "distributor.h"
#include "gui\widget\widget.h"
#include "core\debug.h"

namespace gui
{

/**
*	\brief ���ڶԱ�����bool����
*/
class ResourceLocker
{
public:
	ResourceLocker(bool& locked) :mLocked(locked)
	{
		mLocked = true;
	}
	~ResourceLocker()
	{
		mLocked = false;
	}
private:
	bool &mLocked;
};

MouseMotion::MouseMotion(Widget & widget, Dispatcher::queue_position position):
	mOwner(widget),
	mMouseFocus(nullptr),
	mMouseCaptured(false),
	mSignalHandelMouseMotionEntered(false)
{
	mOwner.ConnectSignal<ui_event::UI_EVENT_MOUSE_MOTION>(
		std::bind(&MouseMotion::SignalHandleMouseMotion, this, std::placeholders::_2, std::placeholders::_3, std::placeholders::_5),position);
}

MouseMotion::~MouseMotion()
{
}

void MouseMotion::MouseEnter(Widget * widget)
{
	Debug::CheckAssertion(widget);

	std::cout << ("[EVENT] Enter event.") << endl;

	mMouseFocus = widget;
	mOwner.Fire(ui_event::UI_EVENT_MOUSE_ENTER, *widget);
}

void MouseMotion::MouseLeave()
{
	std::cout << ("[EVENT] Leave event.") << endl;

	mOwner.Fire(ui_event::UI_EVENT_MOUSE_LEAVE, *mMouseFocus);
	mMouseFocus = nullptr;
}

void MouseMotion::MouseHover(Widget * widget, const Point2 & coords)
{
	mOwner.Fire(ui_event::UI_EVENT_MOUSE_MOTION, *widget, coords);
}

/**
*	\brief ����ƶ����źŴ���
*/
void MouseMotion::SignalHandleMouseMotion(const ui_event event, bool & handle, const Point2 & coords)
{
	if (mSignalHandelMouseMotionEntered)
	{
		return;
	}
	// �Ըú�������
	ResourceLocker locker(mSignalHandelMouseMotionEntered);

	if (mMouseCaptured)
	{

	}
	else
	{	// �ҵ����λ������widget��fire��Ӧ�¼�
		Widget* widget = mOwner.FindAt(coords);
		while (widget && !widget->CanMouseFocus() && widget->GetParent())
		{
			widget = widget->GetParent();
		}
		if (widget)
		{
			if (mOwner.Fire(event, *widget, coords))	// ����źŴ����ض���ֱ�ӽ���
				return;
		}

		// �����ǰû�о۽���widget
		if (mMouseFocus == nullptr && widget != nullptr)
		{
			MouseEnter(widget);
		}
		else if (mMouseFocus != nullptr && widget == nullptr)
		{
			MouseLeave();
		}
		else if (mMouseFocus && widget == mMouseFocus)
		{
			MouseHover(widget, coords);
		}
		else if (mMouseFocus && widget)
		{
			MouseLeave();
			MouseEnter(widget);
		}
	}
}



Distributor::Distributor(Widget& widget, Dispatcher::queue_position position):
	MouseMotion(widget, position)
{
}

Distributor::~Distributor()
{
}


}
