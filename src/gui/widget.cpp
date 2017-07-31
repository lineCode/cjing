#include "widget.h"

Widget::Widget():
	mID(0),
	mParent(),
	mIsDirty(false),
	mVisible(Visiblility::Visible),
	mPosition(),
	mSize()
{
}

Widget::~Widget()
{
}

const Point2 & Widget::GetPositon() const
{
	return mPosition;
}

void Widget::SetPosition(const Point2 & position)
{
	mPosition = position;
}

const Size & Widget::GetSize() const
{
	return mSize;
}

void Widget::SetSize(const Size & size)
{
	mSize = size;
}

int Widget::GetWidth() const
{
	return mSize.width;
}

int Widget::GetHeight() const
{
	return mSize.height;
}

/**
*	\brief λ�Ƶ�ǰwidget
*	\param offset ƫ����
*/
void Widget::Move(const Point2 & offset)
{
}

void Widget::Move(const int xoffset, const int yoffset)
{
}

/**
*	\brief ��ʼ������
*/
void Widget::InitLayout()
{

}

Size Widget::GetBestSize() const
{
	return Size();
}

/**
*	\brief ����ˮƽ����
*/
void Widget::SetHorizontalAlignment()
{
}

/**
*	\brief ���ô�ֱ����
*/
void Widget::SetVerticalAlignment()
{
}

/**
*	\brief ����widget ID
*	\param id Ψһidֵ
*/
void Widget::SetID(const string & id)
{
}

const string & Widget::GetID() const
{
	// TODO: �ڴ˴����� return ���
}

/**
*	\brief ���Ʊ���ͼ
*/
void Widget::DrawBackground()
{
}

/**
*	\brief ����ǰ��ͼ
*/
void Widget::DrawForeground()
{
}

/**
*	\bref ����children
*/
void Widget::DrawChildren()
{
}

void Widget::SetIsDirty(bool isDirty)
{
	mIsDirty = isDirty;
}

bool Widget::IsDirty() const
{
	return mIsDirty;
}

const Rect Widget::GetDirtyRect() const
{
	return Rect();
}

Widget::Visiblility Widget::GetVisibility() const
{
	return mVisible;
}

void Widget::SetVisibility(const Visiblility & visibility)
{
	mVisible = visibility;
}
