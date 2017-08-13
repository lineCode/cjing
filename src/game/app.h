#ifndef _CJLING_APP_H_
#define _CJLING_APP_H_

#include"common\common.h"
#include"lua\luaContext.h"
#include"core\inputEvent.h"
#include"core\renderer.h"
#include"game\sprite.h"
#include"game\animationSprite.h"
#include"game\game.h"
#include"entity\player.h"

// test
#include"gui\core\handler.h"
#include"gui\widget\widget.h"
#include"gui\widget\grid.h"

/**
*	\brief �������ѭ��
*/
class App
{
public:
	App();
	~App();

	void Run();
	void Update();
	void SetExiting(bool isexit);

	void SetGame(Game* game);
	Game* GetGame();
	LuaContext& GetLuaContext();

private:
	bool IsExiting();
	void CheckInput();
	void Render();
	void NotifyInput(const InputEvent& ent);

	bool mExiting;

	std::unique_ptr<LuaContext> mLuaContext;		/** �ű������� */
	
	std::unique_ptr<Game> mCurrGame;				/** ��ǰ��Ϸ */

	Game* mNextGame;								/** ��һ����Ϸ�������л���Ϸ */
							
	std::unique_ptr<gui::GUIManager> mGUI;
	std::shared_ptr<gui::Widget> mWidget1;
	std::shared_ptr<gui::Widget> mWidget2;
	std::shared_ptr<gui::Widget> mWidget3;

	std::shared_ptr<gui::Grid> mGrid;
};

#endif
