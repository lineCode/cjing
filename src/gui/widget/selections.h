#pragma once

#include"gui\widget\widget.h"
#include"gui\widget\grid.h"

namespace gui
{

class SelectionHelper;
class MaxmunSelection;
class Placement;

/**
*	\brief selections���ض�����ʽά�����񼯺�
*	Ŀǰ֧�ִ�ֱ�б���ˮƽ�б����Լ��������ʽ
*	��ά��
*	slections��һ�������������࣬������widget��
*	ʾ��Ӧ���ǰ���selections��widget
*
*	�������й��������ࣺ
*	placement ���ڹ���item��λ��
*	selecAction ���ڹ���ѡ�����
*	
*/

class Selections : public Widget
{
public:
	friend class OneItem;
	friend class HorizontalList;

	Selections();
	Selections(Placement* placement);

	~Selections();

	/** item manager */
	void AddItem(WidgetPtr widget);


	/** selection operator */
	void SelectItem(const unsigned int index, bool selected = true); 
	bool IsSelected(const unsigned int index)const;
	unsigned int GetItemCount()const;
	int GetSelectedItemCount()const;
	int GetSelectedItem();
	Grid& GetItemGrid(const unsigned int index);
	bool GetItemShow(const unsigned int index)const;
	
	/** widget */
	virtual Widget* Find(string& id, const bool activited);
	virtual const Widget* Find(string& id, const bool activied)const;
	virtual Widget* FindAt(const Point2& pos);

	/** testing */
	virtual void Place(const Point2& pos, const Size& size);
	virtual Size CalculateBestSize()const;

protected:
	void DoSelectedItem(unsigned int index);
	void DoDeSelectedItem(unsigned int index);
	void SetItemSelected(const unsigned int index, bool selected);

private:
	int mSelectedItemCount;

	/** children manager */
	struct Child 
	{
		Child():mSelected(false),mShow(true), mGrid(),mOrderedIndex(0){}

		bool mSelected;
		bool mShow;
		Grid mGrid;
		int  mOrderedIndex;
	};

	std::vector<std::unique_ptr<Child> > mItems;

private:

	virtual void ImplDrawChildren(const Point2& offset);

	/** signal handler testing */
	void SignalHanderKeyDown(const ui_event event,bool& handle,const InputEvent::KeyboardKey key);

	void HandlerKeyLeft(bool& handle);
	void HandlerKeyRight(bool& handle);
	void HandlerKeyUp(bool& handle);
	void HandlerKeyDown(bool& handle);

	/** ������� */
	std::unique_ptr<Placement> mPlacement;
	std::unique_ptr<SelectionHelper> mSelectionAction;
	std::unique_ptr<SelectionHelper> mMinmumSelection;
	std::unique_ptr<MaxmunSelection> mMaxmumSelection;

};

}