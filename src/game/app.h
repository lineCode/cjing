#ifndef _APP_H_
#define _APP_H_

#include"common\common.h"
#include"lua\luaContext.h"
#include"core\inputEvent.h"

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
	void SetExiting(bool t);

private:
	bool IsExiting();
	void CheckInput();
	void Render();
	void NotifyInput(const InputEvent& ent);

	std::unique_ptr<LuaContext> mLuaContext;
	
	bool mExiting;

};

#endif
