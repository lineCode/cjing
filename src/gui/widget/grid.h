#pragma once

#include"common\common.h"
#include"gui\widget\widget.h"

#include<memory>

namespace gui
{

/**
*	\brief �������������������Ļ��࣬������������
*	����widget�Ĳ���
*/
class Grid
{
public:
	Grid(int row, int col);
	~Grid();

	/**** **** ��ֱ/ˮƽ����ģʽ **** ****/

	static const unsigned int VERTICAL_SHIFT = 0;			// ʹ�ó�����ֵ or ö��ֵ

	static const unsigned int HORIZONTAL_SHIFT = 3;

	static const unsigned int BORDER_SHIFT = 6;

	/**** **** memeber data **** ****/

	void AddRows(int rowCount = 1);
	void AddCols(int colCount = 1);

	void SetCols(int cols);
	int GetCols()const
	{
		return mCols;
	}

	void SetRows(int rows);
	int GetRows()const
	{
		return mRows;
	}

	/**
	*	\brief ����ָ��grid��children widget
	*/	
	void SetChild(Widget* widget, int row, int col, int flat);		// �Ƿ�ʹ������ָ��
	void SetChild(const WidgetPtr& widget, int row, int col, int flag);

	void SwapChild(const string& id, const WidgetPtr& widget);

	void RemoveChild(int row, int col);
	void RemoveChild(const string& id);

	void SetChildAlignment(Widget* widget, int flag);

	Widget* GetWidget(int row, int col);
	const Widget* GetWidget(int row, int col)const;

	/**** **** *** Layout *** **** ****/
	
	virtual void InitLayout();

	virtual void Place(const Point2& pos, const Size& size);
	virtual void Move(const Point2& offset);
	virtual void Move(const int xoffset, const int yoffset);

	virtual void LayoutChildren();

	virtual Widget* Find(string& id, const bool activited);
	virtual const Widget* Find(string& id, const bool activied)const;
	virtual bool HasWidget(const Widget& widget)const;
	virtual bool IsAt(const Point2& pos)const;

private:
	/**
	*	\brief ������grid�е���widg
	*/
	class Child
	{
	public:
		Child();
		~Child();

		void SetFlag(int flag);
		int GetFlag()const;

		void SetWidget(const WidgetPtr& widget);
		WidgetPtr GetWidget();
		WidgetConstPtr GetWidget()const;

	private:
		int mFlag;

		WidgetPtr mWidget;
	};

	using ChildPtr = std::shared_ptr<Child>;

private:
	int mRows;							/** ��ǰ���� */
	int mCols;							/** ��ǰ���� */

	std::vector<int> mRowsHeight;		/** ÿ������ĸ߶� */
	std::vector<int> mColsWidth;		/** ÿ������Ŀ�� */

	std::vector<ChildPtr> mChilds;		/** ��������ʽ���ִ洢��ǰ����widget*/

};


}