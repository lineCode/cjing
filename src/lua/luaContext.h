#ifndef _LUACONTEXT_H_
#define _LUACONTEXT_H_

#include"common\common.h"
#include"lua\luaTools.h"
#include"lua\luaRef.h"
#include"game\timer.h"

#include<map>
#include<list>
#include<thirdparty\lua.hpp>

/**
 *	\brief C++��lua�Ľӿڣ��ṩ������luaʹ�õ�C++ API
 *
 *  ���е��ṩ��scripts����main,sprite,map,entity��
 *  �������е�script������ͬһ��luaContext
 */
class LuaContext
{
public:
	LuaContext();
	~LuaContext();

	// system
	void Initialize();
	void Update();
	void Exit();

	// script
	static LuaContext& GetLuaContext(lua_State* l);
	bool DoFileIfExists(lua_State*l,const string& name);
	bool LoadFile(lua_State*l, const string& name);
	bool FindMethod(const string& name);
	bool FindMethod(const string& name, int index);
	void RegisterFunction(const string& moduleName, const luaL_Reg* functions);

	// process
	void OnStart();
	void OnUpdate();
	void OnFinish();
	
	// modules
	void RegisterModules();
	void RegisterMainModule();
	void RegisterTimeModule();

	// binding function
	using FunctionExportToLua = int(lua_State* l);
	static FunctionExportToLua
		// error
		api_panic,
		// main
		main_api_hello,
		
		// time
		time_api_start;

	// main api	-- test 
	void PushMain(lua_State*l);
	void OnMainStart();
	void OnMainUpdate();
	void OnMainFinish();

	// time api
	struct TimerData
	{
		LuaRef callBackRef;
		const void* l;
	};
	void AddTimer(const TimerPtr& timer,int constext,const LuaRef& callback);
	void CallTimerRef(const TimerPtr& timer);
	void UpdateTimers();
	void DestoryTimers();

	// modules name
	static const string module_prefix_name;
	static const string module_main_name;
	static const string module_time_name;

private:
	lua_State* l;
	static std::map<lua_State*, LuaContext*>mLuaContexts;/* ������lua APi��ͨ��lua_state��ȡ
														    luaContext */

	std::map<TimerPtr, TimerData>mTimers;				 /* �洢�˶�ʱ����ÿ����ʱ��ӳ��Ķ�
														    Ӧ��callback*/
	std::list<TimerPtr>mTimersToRemove;

};

#endif