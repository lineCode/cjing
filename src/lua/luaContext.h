#ifndef _LUACONTEXT_H_
#define _LUACONTEXT_H_

#include"common\common.h"
#include"lua\luaTools.h"
#include"lua\luaRef.h"
#include"lua\luaObject.h"
#include"game\timer.h"
#include"game\drawable.h"

#include<set>
#include<map>
#include<list>
#include<thirdparty\lua.hpp>

class App;
class InputEvent;

/**
 *	\brief C++��lua�Ľӿڣ��ṩ������luaʹ�õ�C++ API
 *
 *  ���е��ṩ��scripts����main,sprite,map,entity��
 *  �������е�script������ͬһ��luaContext
 */
class LuaContext
{
public:
	LuaContext(App* app);
	~LuaContext();

	// system
	void Initialize();
	void Update();
	void Exit();
	App* GetApp()const;
	bool NotifyInput(const InputEvent& event);

	// script
	static LuaContext& GetLuaContext(lua_State* l);
	bool DoFileIfExists(lua_State*l,const string& name);
	bool LoadFile(lua_State*l, const string& name);
	bool FindMethod(const string& name);
	bool FindMethod(const string& name, int index);
	void RegisterFunction(const string& moduleName, const luaL_Reg* functions);
	void RegisterType(const string& moduleName, const luaL_Reg* functions, const luaL_Reg* methods, const luaL_Reg* metamethods);
	LuaRef CreateRef();
	void PushRef(lua_State*l,const LuaRef& luaref);
	void PrintLuaStack(lua_State*l);
	
	// userdata
	void PushUserdata(lua_State*l, LuaObject& userData);
	const LuaObjectPtr CheckUserData(lua_State*l, int index, const string& moduleName);
	void NotifyUserdataDestoryed(LuaObject& obj);

	// process
	void OnStart();
	void OnUpdate();
	void OnFinish();
	bool OnInput(const InputEvent& event);
	bool OnKeyPressed(const InputEvent& event);
	bool OnKeyReleased(const InputEvent& event);

	// modules
	void RegisterModules();
	void RegisterMainModule();
	void RegisterTimeModule();
	void RegisterMenuModule();
	void RegisterVideoModule();
	void RegisterDrawableModule();
	void RegisterSpriteModule();
	void RegisterAnimationModule();

	// binding function
	using FunctionExportToLua = int(lua_State* l);
	static FunctionExportToLua
		// error
		api_panic,
		// main
		main_api_hello,
		main_api_exit,
		// time
		time_api_start,
		// menu
		menu_api_start,
		menu_api_stop,
		// video
		video_api_setFullScreen,
		video_api_isFullScreen,
		// sprite
		sprite_api_create,
		// userdata
		userdata_meta_gc,
		userdata_meta_newindex,
		userdata_meta_index
		;
		
	// main api	-- test 
	void PushMain(lua_State*l);
	void OnMainStart();
	void OnMainUpdate();
	void OnMainFinish();
	void OnMainDraw();
	bool OnMainInput(const InputEvent& event);

	// time api
	struct TimerData
	{
		LuaRef callBackRef;
		const void* l;
	};
	void AddTimer(const TimerPtr& timer,int contextIndex,const LuaRef& callback);
	void CallTimerRef(const TimerPtr& timer);
	void UpdateTimers();
	void DestoryTimers();

	// menu api
	struct MenuData	
	{	// �˵����ݽṹ������ָ��˵���table
		LuaRef menuRef;
		const void*l;
		bool recentlyAdd;
		MenuData(const LuaRef&menuref, const void*context) :
			menuRef(menuref), l(context), recentlyAdd(true) {}
	};
	void AddMenu(int contextIndex,const LuaRef& menuRef );
	void UpdateMenus();
	void DestoryMenus();
	void OnMenuStart(const LuaRef&menuRef);
	void OnMenuUpdate(const LuaRef&menuRef);
	void OnMenuFinish(const LuaRef&menuRef);
	bool OnMenuInput(int contextIndex,const InputEvent& event);
	bool OnMenuInput(const InputEvent& event, const LuaRef&menuRef);
	void OnMenuDraw(int contextIndex);
	void OnMenuDraw(const LuaRef& menuRef);
	void RemoveMenus(int contextIndex);
	void RemoveMenus();

	// sprite api
	void PushSprite(lua_State*l, Sprite& sprite);
	SpritePtr CheckSprite(lua_State*l, int index);

	// modules name
	static const string module_name;
	static const string module_main_name;
	static const string module_time_name;
	static const string module_menu_name;
	static const string module_video_name;

	// movement modules name
	static const string module_movement_name;
	static const string module_straight_movment_name;

	// drawable
	static const string module_drawable_name;
	static const string module_sprite_name;
	static const string module_animation_name;

private:
	App* mApp;
	lua_State* l;
	static std::map<lua_State*, LuaContext*>mLuaContexts;/* ������lua APi��ͨ��lua_state��ȡ
														    luaContext */

	std::map<TimerPtr, TimerData>mTimers;				 /* �洢�˶�ʱ����ÿ����ʱ��ӳ��Ķ�
														    Ӧ��callback*/
	std::list<TimerPtr>mTimersToRemove;					 

	std::list<MenuData>mMenus;							/* �洢�˲˵���ÿ���˵�����ӳ��menu 
															table��luaRef*/

	std::set<DrawablePtr>mDrawables;						/* �洢���ɽű�������Drawable����*/
	std::set<DrawablePtr>mDrawablesToRemove;

};

#endif