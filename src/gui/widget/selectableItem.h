#pragma once

namespace gui
{
class Widget;

/**
*	\breif ��ѡ����widget�ĳ������,
*	�ṩ�����Ƿ�ѡ��״̬�ĳ���ӿ�
*/
class SelectableItem
{
public:
	virtual void SetSelected(bool selected) = 0;
	virtual bool IsSelected()const = 0;

};

}