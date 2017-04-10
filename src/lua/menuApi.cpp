#include"luaContext.h"
#include"luaTools.h"

const string LuaContext::module_menu_name = "Menu";

void LuaContext::RegisterMenuModule()
{
	static const luaL_Reg functions[] = {
		{"Start",menu_api_start},
		{"Stop",menu_api_stop},
		{ nullptr,nullptr }
	};

	RegisterFunction(module_menu_name, functions);
}

/**
*	\brief ��Ӳ˵�
*/
void LuaContext::AddMenu(int contextIndex, const LuaRef& menuRef)
{
	const void*context = lua_topointer(l, 1);// ������ʱ����menu �ض�Ϊtable
	mMenus.emplace_back(menuRef, context);
	// ִ��onStarted
	OnMenuStart(menuRef);
}

/**
*	\brief ���²˵��ɾ��Ϊ�յĲ˵�
*
*	onUpdate���ɴκ�������,�Ҵ�ɾ��������menuRef�Ƿ�Ϊ��
*	����Ҫɾ����menuӦ������������ִ��menuRef.claer()
*/
void LuaContext::UpdateMenus()
{
}

void LuaContext::DestoryMenus()
{
	mMenus.clear();
}

/**
*	\brief �����˵�menu
*/
int LuaContext::menu_api_start(lua_State* l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		LuaContext& luaContext = GetLuaContext(l);
		// ����1������context table
		if (lua_type(l, 1) != LUA_TUSERDATA && lua_type(l, 1) != LUA_TTABLE)
			LuaTools::Error(l, "wrong type.Only userdata or table");

		// ����2������table
		LuaTools::CheckType(l, 2, LUA_TTABLE);
		LuaRef menuRef = luaContext.CreateRef();
		luaContext.AddMenu(1, menuRef);

		return 0;
	});
}

/**
*	\brief �رղ˵�
*
*	lua: Menu.Stop(self);
*/
int LuaContext::menu_api_stop(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
	
		return 0;
	});
}

void LuaContext::OnMenuStart(const LuaRef&menuRef)
{
	PushRef(l, menuRef);
	OnStart();
	lua_pop(l, 1);
}

/**
*	\brief ����menu
*
*	�ڸ����걾�����Ҫ�����Ӵ��壨����Ϊnil)
*/
void LuaContext::OnMenuUpdate(const LuaRef&menuRef)
{
	PushRef(l, menuRef);
	OnUpdate();
	// OnMenusUpdate(menuRef);
	lua_pop(l, 1);
}

/**
*	\brief ִ��menu��onFinished����
*
*	�ú���Ϊmenuz�ս�ʱ���ã���Ҫͬʱ�����Ӵ���
*/
void LuaContext::OnMenuFinish(const LuaRef&menuRef)
{
	PushRef(l, menuRef);
	// removeMenus(-1);
	OnFinish();
	lua_pop(l, 1);
}