#pragma once

#include"common\common.h"
#include"gui\widget\widget.h"

#include<memory>
#include<map>

namespace gui
{

/**** **** ��ֱ/ˮƽ����ģʽ **** ****/
	
	// ��ֱ���뷽ʽ
	static const unsigned int ALIGN_VERTICAL_SHIFT = 0;
	static const unsigned int ALIGN_VERTICAL_TOP = 1 << ALIGN_VERTICAL_SHIFT;
	static const unsigned int ALIGN_VERTICAL_CENTER = 2 << ALIGN_VERTICAL_SHIFT;
	static const unsigned int ALIGN_VERTICAL_BOTTOM = 4 << ALIGN_VERTICAL_SHIFT;
	static const unsigned int ALIGN_VERTICAL_MASK = 7 << ALIGN_VERTICAL_SHIFT;

	// ˮƽ���뷽ʽ
	static const unsigned int ALIGN_HORIZONTAL_SHIFT = 3;
	static const unsigned int ALIGN_HORIZONTAL_TOP = 1 << ALIGN_HORIZONTAL_SHIFT;
	static const unsigned int ALIGN_HORIZONTAL_CENTER = 2 << ALIGN_HORIZONTAL_SHIFT;
	static const unsigned int ALIGN_HORIZONTAL_BOTTOM = 4 << ALIGN_HORIZONTAL_SHIFT;
	static const unsigned int ALIGN_HORIZONTAL_MASK = 7 << ALIGN_HORIZONTAL_SHIFT;

	// �߽�
	static const unsigned int BORDER_TOP = 1 << 6;
	static const unsigned int BORDER_LEFT = 1 << 7;
	static const unsigned int BORDER_BOTTOM = 1 << 8;
	static const unsigned int BORDER_RIGHT = 1 << 9;
	static const unsigned int BORDER_ALL = BORDER_TOP | BORDER_LEFT | BORDER_BOTTOM | BORDER_RIGHT;

	
/**
*	\brief �������������������Ļ��࣬������������
*	����widget�Ĳ���
*/
class Grid : public Widget
{
	class Children;
public:
	Grid(int row, int col);
	~Grid();

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

	void SetRowCols(int row, int col);

	/**** **** *** Layout *** **** ****/
	
	virtual void InitLayout();

	virtual void Place(const Point2& pos, const Size& size);
	virtual void SetPosition(const Point2& position);

	void ReduceWidth(const int maxnumWidth);
	virtual void RequestReduceWidth(const int maxnumWidth);
	virtual void DemandReduceWidth(const int maxnumWidth);

	void ReduceHeight(const int maxnumHeight);
	virtual void RequestReduceHeight(const int maxnumHeight);
	virtual void DemandReduceHeight(const int maxnumHeight);

	virtual void Layout(const Point2& origin);
	virtual void LayoutChildren(const Point2& origin);

	virtual Widget* Find(string& id, const bool activited);
	virtual const Widget* Find(string& id, const bool activied)const;
	virtual bool HasWidget(const Widget& widget)const;
	virtual bool IsAt(const Point2& pos)const;
	virtual Size CalculateBestSize()const;
	virtual Size ReCalculateBestSize();

private:
	int RequestReduceColWidth(int col, const int maxnumWidth);
	void RequestReduceCellWidth(Children& child, const int maxnumWidth);
	int RequestReduceRowHeight(int row, const int maxnumHeight);
	void RequestReduceCellHeight(Children& child, const int maxnumHeight);

	virtual void ImplDrawChildren(const Point2& offset);

	void RequesetPlacement(Dispatcher&, const gui::ui_event, bool&handle, bool& halt);

	/**** **** **** **** Child **** **** **** ****/

	/**
	*	\brief ������grid�е��ӽڵ�
	*/
	class Children
	{
	public:
		Children();

		const string GetID()const;

		void InitLayout();
		void Place(const Point2& pos, const Size& size);
		Size GetBestSize()const;
		Size GetBorderSpace()const;

		void SetFlag(const unsigned int flag)
		{
			mFlag = flag;
		}
		unsigned int GetFlag()const
		{
			return mFlag;
		}
		void SetWidget(const WidgetPtr& widget)
		{
			mWidget = widget;
		}
		int GetBorderSize()const
		{
			return mBorderSize;
		}
		void SetBorderSize(int borderSize)
		{
			mBorderSize = borderSize;
		}
		WidgetPtr GetWidget()
		{
			return mWidget;
		}
		const WidgetPtr GetWidget()const
		{
			return mWidget;
		}
	private:
		unsigned int mFlag;
		int mBorderSize;
		WidgetPtr mWidget;
	};

public:
	void SetChildren(Widget* widget, int row, int col, const unsigned int flat);		// �Ƿ�ʹ������ָ��
	void SetChildren(const WidgetPtr& widget, int row, int col, const unsigned int flag, int borderSize);
	void RemoveChildren(int row, int col);
	void RemoveChildren(const string& id);
	void SetChildrenAlignment(WidgetPtr widget, const unsigned int setflag, const unsigned int modeMask);

	Children* GetChildren(const WidgetPtr& widget);
	Children& GetChildren(int row, int col)
	{
		return mChilds[col * mRows + row];
	}
	const Children& GetChildren(int row, int col)const
	{
		return mChilds[col * mRows + row];
	}
	WidgetPtr GetWidget(int row, int col)
	{
		return GetChildren(row, col).GetWidget();
	}
	const WidgetPtr GetWidget(int row, int col)const
	{
		return GetChildren(row, col).GetWidget();
	}

private:
	int mRows;							/** ��ǰ���� */
	int mCols;							/** ��ǰ���� */

	mutable std::vector<int> mRowsHeight;		/** ÿ������ĸ߶� */
	mutable std::vector<int> mColsWidth;		/** ÿ������Ŀ�� */

	std::vector<Children> mChilds;		/** ��������ʽ���ִ洢��ǰ����widget*/

	std::map<string, Children> mNamedChilds;	/** ��map��ʽ�洢children,����ID��Ϊ��ֵ,���ڲ��� */
												// Ŀǰ���Ǵ��ڱ�Ҫ�ԣ���ΪGUI������һ��Ƚ�С

};												


}