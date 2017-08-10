#include "widget.h"
#include "window.h"

namespace gui
{

Widget::Widget() :
	mID(""),
	mParent(nullptr),
	mIsDirty(true),
	mVisible(Visiblility::Visible),
	mReDrawAction(ReDrawAction::Full),
	mLinkedGroup(""),
	mPosition(),
	mSize()
{
}

Widget::~Widget()
{
	// ֪ͨ����widget,��widget���
	Widget* parent = GetParent();
	while (parent)
	{
		parent->Fire(gui::EVENT_NOTIFY_REMOVE, *parent);
		parent = parent->GetParent();
	}

	// link group ???
	if (!mLinkedGroup.empty())
	{

	}
}

/**
*	\brief ����widget ID
*	\param id Ψһidֵ
*/
void Widget::SetID(const string & id)
{
	mID = id;
}

const string & Widget::GetID() const
{
	return mID;
}

Widget * Widget::GetParent()
{
	return mParent;
}

/**
*	\brief ��ȡ�㼶����
*/
Window * Widget::GetWindow()
{
	Widget* result = this;
	while (result->GetParent() != nullptr)
	{
		result = result->GetParent();
	}
	return dynamic_cast<Window*>(result);
}

const Window * Widget::GetWindow() const
{
	const Widget* result = this;
	while (result->mParent != nullptr)
	{
		result = result->mParent;
	}
	return dynamic_cast<const Window*>(result);
}

/***** ****** ****** Layout and size  ****** ****** *******/

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

void Widget::Place(const Point2 & pos, const Size & size)
{
	mPosition = pos;
	mSize = size;
}

/**
*	\brief λ�Ƶ�ǰwidget
*	\param offset ƫ����
*/
void Widget::Move(const Point2 & offset)
{
	mPosition += offset;
}

void Widget::Move(const int xoffset, const int yoffset)
{
	mPosition += Point2(xoffset, yoffset);
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
*	\brief ���Ʊ���ͼ
*/
void Widget::DrawBackground(const Point2& offset)
{
}

/**
*	\brief ����ǰ��ͼ
*/
void Widget::DrawForeground(const Point2& offset)
{
}

/**
*	\bref ����children
*/
void Widget::DrawChildren(const Point2& offset)
{
}

/**
*	\brief ��debugģʽ�»��������������
*/
void Widget::DrawDebugGround()
{
}

void Widget::ImplDrawBackground()
{
}

void Widget::ImplDrawForeground()
{
}

void Widget::ImplDrawChildren()
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

void Widget::SetParent(Widget * parent)
{
}

void Widget::SetLinkGrounp(const std::string & linkedGroup)
{
}

Widget* Widget::Find(string& id, const bool activited)
{
	return nullptr;
}

const Widget* Widget::Find(string& id, const bool activied)const
{
	return nullptr;
}

bool Widget::HasWidget(const Widget& widget)const
{
	return false;
}

bool Widget::IsAt(const Point2& pos)const
{
	return false;
}

Size Widget::CalculateBestSize()const
{
	return Size();
}

}