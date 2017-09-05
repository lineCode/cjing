#pragma once

#include"gui\widget\styledwidget.h"
#include"gui\widget\selectableItem.h"

namespace gui
{
/**
*	\brief widget button
*/

class ToggleButton : public StyledWidget, public SelectableItem
{
public:
	ToggleButton();

	virtual void SetActivite(const bool activite)const;
	virtual bool GetActivite()const;
	virtual unsigned int GetState()const;

	// inherited interface
	virtual void SetSelected(bool selected);
	virtual bool IsSelected()const;

private:
	enum State
	{
		ENABLED,
		DISABLED,
		FOCUSED,
		SELECTED,
	};

	void SetState(const State state)
	{
		mState = state;
	}

	State mState;	// ��ǰ��ť״̬

	/** ���� */
	virtual void ImplDrawBackground(const Point2& offset);
	virtual void ImplDrawForeground(const Point2& offset);

	// test sprite
	SpritePtr mTestSprite;

	/** �źŻص����� */
	void SignalHandlerMouseEnter(const  ui_event event, bool&handle);
	void SignalHandlerMouseLeave(const  ui_event event, bool&handle);
	void SignalHandlerMouseLeftButtonDown(const  ui_event event, bool&handle);
	void SignalHandlerMouseLeftButtonUp(const  ui_event event, bool&handle);
	void SignalHandlerMouseLeftButtonClick(const  ui_event event, bool&handle);

	// test
	void SignalHandlerMouseLeftButtonDoubleClick(const  ui_event event, bool&handle);
};


}