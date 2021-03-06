#pragma once

#include"common\common.h"
#include"game\sprite.h"
#include"utils\point.h"
#include"gui\core\dispatcher.h"
#include"gui\core\builderWindow.h"

namespace gui
{

class Window;
class Grid;

/**
*	\brief widget基类
*/
class Widget : public Dispatcher, public std::enable_shared_from_this<Widget>
{
	/*** *** *** *** *** property. *** *** *** *** ***/
public:
	/** 可见性动作 */
	enum class Visiblility
	{
		Visible,
		Hidden,
		InVisible
	};

	/** 重绘动作 */
	enum class ReDrawAction
	{
		Full,		// 完整绘制
		Partly,		// 部分绘制,目前不支持
		None	
	};

public:
	Widget();
	Widget(const BuilderWidget& builder);
	virtual ~Widget();

	Widget(const Widget&) = delete;
	Widget& operator=(const Widget&) = delete;

	void SetID(const string& id);
	const string& GetID()const;

	void DrawBackground();
	void DrawForeground();

	void DrawBackground(const Point2& offset);
	void DrawForeground(const Point2& offset);
	void DrawChildren(const Point2& offset);
	void DrawDebugGround();

private:
	virtual void ImplDrawBackground(const Point2& offset);
	virtual void ImplDrawForeground(const Point2& offset);
	virtual void ImplDrawChildren(const Point2& offset);

public:
	/******  setter/ getter *******/

	void SetIsDirty(bool isDirty);
	bool IsDirty()const;
	const Rect GetDirtyRect()const;

	Visiblility GetVisibility()const;
	void SetVisibility(const Visiblility& visibility);

	ReDrawAction GetReDrawAction()const
	{
		return mReDrawAction;
	}
	void SetReDrawAction(const ReDrawAction& action)
	{
		mReDrawAction = action;
	}

	void SetParent(Widget* parent);
	Widget* GetParent();

	Window* GetWindow();
	const Window* GetWindow()const;

	Grid* GetParentGrid();

	virtual const string GetControlType()const;

	void SetLinkGrounp(const std::string& linkedGroup);
private:
	string mID;

	//std::weak_ptr<Widget> mParent;
	Widget* mParent;

	bool mIsDirty;			// 目前每帧重绘的情况下，dirty的存在暂定
	Visiblility mVisible;
	ReDrawAction mReDrawAction;

	std::string mLinkedGroup;

	/*** *** *** *** *** layout and size. *** *** *** *** ***/
public:
	const Point2& GetPositon()const;
	virtual void SetPosition(const Point2& position);
	const Size& GetSize()const;
	virtual void SetSize(const Size& size);

	int GetWidth()const;
	int GetHeight()const;

	Size GetLayoutSize()const
	{
		return mLayoutSize;
	}
	void SetLayoutSize(const Size& layoutSize)
	{
		mLayoutSize = layoutSize;
	}

	virtual void RequestReduceWidth(const int maxnumWidth);
	virtual void DemandReduceWidth(const int maxnumWidth);

	virtual void RequestReduceHeight(const int maxnumHeight);
	virtual void DemandReduceHeight(const int maxnumHeight);

	virtual void Place(const Point2& pos, const Size& size);

	virtual void Move(const Point2& offset);
	virtual void Move(const int xoffset, const int yoffset);

	virtual void InitLayout();
	virtual Size GetBestSize()const;
	virtual Size CalculateBestSize()const;
	virtual Size ReCalculateBestSize();

	virtual void SetHorizontalAlignment(const unsigned int align);
	virtual void SetVerticalAlignment(const unsigned int align);

	/** 
	*	能否换行，该属性影响requestReduceWidth操作是否可以进行
	*/
	virtual bool CanWrap() const;

	/**
	*	能否鼠标处理
	*/
	virtual bool CanMouseFocus()const 
	{
		return true;
	}

private:
	Point2 mPosition;
	Size mSize;
	Size mLayoutSize;

	// debug sprite
	SpritePtr mDebugSprite;

public:
	virtual Widget* Find(string& id, const bool activited);
	virtual const Widget* Find(string& id, const bool activied)const;
	virtual bool HasWidget(const Widget& widget)const;
	virtual bool IsAt(const Point2& pos)const;
	virtual Widget* FindAt(const Point2& pos);


};

using WidgetPtr = std::shared_ptr<Widget>;
using WidgetConstPtr = std::shared_ptr<const Widget>;

}