#include"grid.h"
#include"widget.h"

#include<algorithm>
#include<numeric>

namespace gui
{

Grid::Grid(int row, int col) :
	mRows(row),
	mCols(col),
	mRowsHeight(),
	mColsWidth(),
	mChilds(row * col),
	mNamedChilds()
{
	ConnectSignal<gui::EVENT_REQUEST_PLACEMENT>(
		std::bind(&Grid::RequesetPlacement, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
		gui::Dispatcher::back_pre_child);
}

Grid::~Grid()
{
	mNamedChilds.clear();
	mChilds.clear();
}

void Grid::AddRows(int rowCount )
{
	SetRowCols(mRows + rowCount, mCols);
}

void Grid::AddCols(int colCount )
{
	SetRowCols(mRows, mCols + colCount);
}

void Grid::SetCols(int cols)
{
	if (mCols == cols)
	{
		return;
	}
	SetRowCols(mRows, cols);
}

void Grid::SetRows(int rows)
{
	if (mRows == rows)
	{
		return;
	}
	SetRowCols(rows, mCols);
}

void Grid::SetRowCols(int row, int col)
{
	if (mRows == row && mCols == col)
	{
		return;
	}
	if (!mChilds.empty())
	{
		Debug::Warning("Resizing a non-empty grid");
	}
	mRows = row;
	mCols = col;
	mChilds.resize(row * col);
}

/**** **** **** **** *** Child *** **** **** **** ****/

/**
*	\brief ����ָ��������ӽڵ�
*	\param widget  ��ӵ�wiget
*	\param row,col ָ��������
*	\param flag    �ӽڵ�Ķ���ģʽ
*/
void Grid::SetChildren(const WidgetPtr& widget, int row, int col, const unsigned int flag)
{
	Debug::CheckAssertion(row < mRows && col < mCols);
	Debug::CheckAssertion(flag & ALIGN_VERTICAL_MASK);
	Debug::CheckAssertion(flag & ALIGN_HORIZONTAL_MASK);

	Children& children = GetChildren(row, col);
	if (children.GetWidget() != nullptr)
	{
		Debug::Warning(string("The childe '" + children.GetID() + "'will be replaced."));
		children.SetWidget(nullptr);	// use shard_ptr
	}

	children.SetFlag(flag);
	children.SetWidget(widget);
	if (children.GetWidget() != nullptr)
	{
		children.GetWidget()->SetParent(this);
	}
}

/**
*	\brief �Ƴ�ָ��������ӽڵ�
*	\param row,col ָ��������
*/
void Grid::RemoveChildren(int row, int col)
{
	Debug::CheckAssertion(row < mRows && col < mCols);

	Children& children = GetChildren(row, col);
	if (children.GetWidget() != nullptr)
	{
		children.SetWidget(nullptr);
	}
}

/**
*	\brief �Ƴ�ָ��������ӽڵ�
*	\param row,col ָ��������
*/
void Grid::RemoveChildren(const string& id)
{
	auto& findedChildren = mNamedChilds.find(id);
	if (findedChildren != mNamedChilds.end())
	{
		findedChildren->second.SetWidget(nullptr);
	}
	else
	{
		for (auto& child : mChilds)
		{
			if (child.GetID() == id)
			{
				child.SetWidget(nullptr);
			}
		}
	}
}

Grid::Children* Grid::GetChildren(const WidgetPtr& widget)
{
	for (auto& child : mChilds)
	{
		if (child.GetWidget() == widget)
			return &child;
	}
	return nullptr;
}

/**
*	\brief ����ָ�������ӽڵ�Ķ��뷽ʽ
*	\param flag      ���뷽ʽö��ֵ
*	\param modeMaks  ���뷽ʽ������
*/
void Grid::SetChildrenAlignment(WidgetPtr widget, const unsigned int setflag, const unsigned int modeMask)
{
	Children* children = GetChildren(widget);
	if (children == nullptr)
	{
		return;
	}
	unsigned int flag = children->GetFlag();
	flag &= ~modeMask;
	flag |= setflag;
	children->SetFlag(flag);
}

Grid::Children::Children():
	mFlag(0),
	mWidget(nullptr)
{
}

const string Grid::Children::GetID() const
{
	string result;
	if (mWidget != nullptr)
	{
		result = mWidget->GetID();
	}
	return result;
}

void Grid::Children::InitLayout()
{
	Debug::CheckAssertion(mWidget != nullptr);

	if (mWidget->GetVisibility() != Widget::Visiblility::InVisible)
	{
		mWidget->InitLayout();
	}
}

void Grid::Children::Place(const Point2& pos, const Size& size)
{
	if (mWidget == nullptr || mWidget->GetVisibility() == Widget::Visiblility::InVisible)
	{
		return;
	}
	Point2 adjustedPos(pos);
	Size   adjustedSize(size);

	// �����߽����
	if (mBorderSize > 0)
	{
		if (mFlag & BORDER_TOP)
		{
			adjustedPos.y += mBorderSize;
			adjustedSize.height -= mBorderSize;
		}
		if (mFlag & BORDER_LEFT)
		{
			adjustedPos.x += mBorderSize;
			adjustedSize.width -= mBorderSize;
		}
		if (mFlag & BORDER_BOTTOM)
		{
			adjustedSize.height -= mBorderSize;
		}
		if (mFlag & BORDER_RIGHT)
		{
			adjustedSize.width -= mBorderSize;
		}
	}

	const Size bestSize = mWidget->GetBestSize();
	if (size <= bestSize)
	{
		mWidget->Place(adjustedPos, adjustedSize);
	}

	// �����ô�С����bestSizeʱ������Ҫ���ݶ��뷽ʽ����λ��
	Point2 widgetPos = adjustedPos;
	Size widgetSize = Size(min(size.width, bestSize.width), min(size.height, bestSize.height));

	int vertialFlag = mFlag & ALIGN_VERTICAL_MASK;
	if (vertialFlag & ALIGN_VERTICAL_TOP)
	{
		// Ĭ���Ѿ���������,do nothing
		Debug::DebugString("The widget id:" + GetID() + " aligned to vertical top.");
	}
	else if (vertialFlag & ALIGN_VERTICAL_CENTER)
	{
		widgetPos.y += (size.height - widgetSize.height) / 2;
		Debug::DebugString("The widget id:" + GetID() + " aligned to vertical center.");
	}
	else if (vertialFlag & ALIGN_VERTICAL_BOTTOM)
	{
		widgetPos.y += (size.height - widgetSize.height);
		Debug::DebugString("The widget id:" + GetID() + " aligned to vertical bottom.");
	}
	else
	{
		Debug::Die("Invaild vertical align parameter.");
	}

	int horizontalFlag = mFlag & ALIGN_HORIZONTAL_MASK;
	if (horizontalFlag & ALIGN_HORIZONTAL_TOP)
	{
		// Ĭ���Ѿ���������,do nothing
		Debug::DebugString("The widget id:" + GetID() + " aligned to horizontal top.");
	}
	else if (horizontalFlag & ALIGN_HORIZONTAL_CENTER)
	{
		widgetPos.x += (size.width - widgetSize.width) / 2;
		Debug::DebugString("The widget id:" + GetID() + " aligned to horizontal center.");
	}
	else if (horizontalFlag & ALIGN_HORIZONTAL_TOP)
	{
		widgetPos.x += (size.width - widgetSize.width) / 2;
		Debug::DebugString("The widget id:" + GetID() + " aligned to horizontal bottom.");
	}
	else
	{
		Debug::Die("Invaild horizontal align parameter.");
	}

	mWidget->Place(widgetPos, widgetSize);
}

Size Grid::Children::GetBorderSpace()const
{
	Size size(0, 0);
	if (mBorderSize > 0)
	{
		if (mFlag & BORDER_TOP)
		{
			size.height += mBorderSize;
		}
		if (mFlag & BORDER_LEFT)
		{
			size.width += mBorderSize;
		}
		if (mFlag & BORDER_BOTTOM)
		{
			size.height += mBorderSize;
		}
		if (mFlag & BORDER_RIGHT)
		{
			size.width += mBorderSize;
		}
	}
	return size;
}

/**
*	\brief ��ȡ���ӽڵ����Ѵ�С
*
*	���������widget��ֱ�ӷ���
*/
Size Grid::Children::GetBestSize()const
{
	if (mWidget == nullptr)
	{
		return GetBorderSpace();
	}

	if (mWidget->GetVisibility() == Widget::Visiblility::InVisible)
	{
		return Size(0, 0);
	}

	return  mWidget->GetBestSize() + GetBorderSpace();
}

/**** **** **** **** *** Layout *** **** **** **** ****/

void Grid::InitLayout()
{
	Widget::InitLayout();
	for (auto& child : mChilds)
	{
		child.InitLayout();
	}
}

/**
*	\brief ���������ӽڵ�
*/
void Grid::LayoutChildren(const Point2& origin)
{
	Point2 pos = origin;
	for (int row = 0; row < mRows; row++)
	{
		for (int col = 0; col < mCols; col++)
		{
			const Size size(mColsWidth[col], mRowsHeight[row]);
			WidgetPtr widget = GetChildren(row, col).GetWidget();
			if (widget != nullptr)
			{
				GetChildren(row, col).Place(pos, size);
			}
			pos.x += mColsWidth[col];
		}
		pos.x = origin.x;
		pos.y += mRowsHeight[row];
	}
}

/**
*	\brief ���������ӽڵ�
*/
void Grid::Layout(const Point2& origin)
{
	Point2 pos = origin;
	for (int row = 0; row < mRows; row++)
	{
		for (int col = 0; col < mCols; col++)
		{
			const Size size(mColsWidth[col], mRowsHeight[row]);
			WidgetPtr widget = GetChildren(row, col).GetWidget();
			if (widget != nullptr)
			{
				GetChildren(row, col).Place(pos, size);
			}
			pos.x += mColsWidth[col];
		}
		pos.x = origin.x;
		pos.y += mRowsHeight[row];
	}
}

/**
*	\brief ���ö�Ӧλ��,�����ö�Ӧ��С
*
*    ���ô�С��λ�û����²��ָ������µ������ӽڵ㣬ͬʱ
*	 ���õĴ�С������ڵ���best_size
*/
void Grid::Place(const Point2& pos, const Size& size)
{
	Widget::Place(pos, size);

	Debug::CheckAssertion(mRowsHeight.size() == mRows);
	Debug::CheckAssertion(mColsWidth.size() == mCols);

	Size bestSize = GetBestSize();
	if (bestSize == size)
	{
		LayoutChildren(pos);
		return;
	}
	if (bestSize.width > size.width || bestSize.height > size.height)
	{
		Debug::Error("Faild to place grid,we have not enough space.");
		return ;
	}

	// �����õĴ�С��������Ĵ�С�������grid,����Ĭ�����е�����
	// ����������ͬ
	auto ResizeGridCell = [](int diff, std::vector<int>& gridVecot){
		const int factor = 1;					// ÿ���������������
		int count = gridVecot.size() * factor;	// �������
		int growBycell = diff / count;			// ÿ��������������

		for (auto& val : gridVecot)
		{
			val += growBycell;
		}
	};
	if (size.width > bestSize.width)
	{
		ResizeGridCell(size.width - bestSize.width, mColsWidth);
	}
	if (size.height > bestSize.height)
	{
		ResizeGridCell(size.height - bestSize.height, mRowsHeight);
	}
	LayoutChildren(pos);
}

void Grid::SetPosition(const Point2& position)
{
	Widget::SetPosition(position);

	for (auto& child : mChilds)
	{
		if (child.GetWidget() != nullptr)
		{
			child.GetWidget()->SetPosition(position);
		}
	}
}

/**
*	\brief ������Ѵ�С
*
*	ͨ������ÿ���ӽڵ����Ѵ�С
*/
Size Grid::CalculateBestSize()const
{
	mColsWidth.clear();
	mColsWidth.resize(mCols, 0);
	mRowsHeight.clear();
	mRowsHeight.resize(mRows, 0);

	for (int row = 0; row < mRows; row++)
	{
		for (int col = 0; col < mCols; col++)
		{
			Size bestSize = GetChildren(row, col).GetBestSize();
			if (bestSize.width > mColsWidth[col])
			{
				mColsWidth[col] = bestSize.width;
			}
			if (bestSize.height > mRowsHeight[row])
			{
				mRowsHeight[row] = bestSize.height;
			}
		}
	}
	// ��ͼ����ܵĴ�С
	Size bestSize(std::accumulate(mColsWidth.begin(), mColsWidth.end(), 0),
				  std::accumulate(mRowsHeight.begin(), mRowsHeight.end(), 0));

	return bestSize;
}

/**
*	\brief ���ٵ�ǰ���
*
*	�����ٵ��Ŀ��С��bestSizeʱ�������requestReduceSize
*/
void Grid::ReduceWidth(const int maxnumWidth)
{
	const Size bestSize = GetBestSize();
	if (bestSize.width <=  maxnumWidth)
	{
		return;
	}
	// reduce width friendly
	RequestReduceWidth(maxnumWidth);
	
	const Size bestSizeAfterReduce = GetBestSize();
	if (bestSizeAfterReduce.width <=  maxnumWidth)
	{
		return;
	}
	// ������ٿռ�ʧ��
	Debug::Error("Failed to reduce width.");
}

void Grid::RequestReduceWidth(const int maxnumWidth)
{
}

void Grid::DemandReduceWidth(const int maxnumWidth)
{
}

/**
*	\brief ���ٵ�ǰ�߶�
*
*	�����ٵ��Ŀ��С��bestSizeʱ�������requestReduceSize
*/
void Grid::ReduceHeight(const int maxnumHeight)
{
	const Size bestSize = GetBestSize();
	if (bestSize.height <= maxnumHeight)
	{
		return;
	}
	// reduce width friendly
	RequestReduceHeight(maxnumHeight);

	const Size bestSizeAfterReduce = GetBestSize();
	if (bestSizeAfterReduce.height <= maxnumHeight)
	{
		return;
	}
	// ������ٿռ�ʧ��
	Debug::Error("Failed to reduce width.");
}

void Grid::RequestReduceHeight(const int maxnumHeight)
{
}

void Grid::DemandReduceHeight(const int maxnumHeight)
{
}


void Grid::ImplDrawBackground()
{
}

void Grid::ImplDrawForeground()
{
}

void Grid::ImplDrawChildren()
{
}

/**
*	\brief ������������
*
*	signal����,�ڹ���ʱ����signalConnect����
*/
void Grid::RequesetPlacement(Dispatcher&, const gui::ui_event, bool&handle, bool& halt)
{

}

Widget* Grid::Find(string& id, const bool activited)
{
	return nullptr;
}

const Widget* Grid::Find(string& id, const bool activied)const
{
	return nullptr;
}

bool Grid::HasWidget(const Widget& widget)const
{
	return false;
}

bool Grid::IsAt(const Point2& pos)const
{
	return false;
}

}