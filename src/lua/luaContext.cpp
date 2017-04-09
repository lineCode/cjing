#include"luaContext.h"
#include"core\fileData.h"
#include"core\debug.h"

std::map<lua_State*, LuaContext*> LuaContext::mLuaContexts;

LuaContext::LuaContext()
{
}

LuaContext::~LuaContext()
{
	this->Exit();
}

/**
*	\brief ��ʼ��lua
*
*	������ʼ��lua_state��ע�����ģ�飬����userdata table
*/
void LuaContext::Initialize()
{
	l = luaL_newstate();
	lua_atpanic(l, api_panic);
	luaL_openlibs(l);
	mLuaContexts[l] = this;

	// ����userdata ��,����ģʽΪ����
	lua_newtable(l);
					// userdata
	lua_newtable(l);
					// userdata meta
	lua_pushstring(l, "v");
					// userdata meta v
	lua_setfield(l, -2, "__mode");
					// userdata meta
	lua_setmetatable(l, -2);
					// userdata
	lua_setfield(l, LUA_REGISTRYINDEX, "all_userdata");

	// ����userdata_table
	lua_newtable(l);
	lua_setfield(l, LUA_REGISTRYINDEX, "userdata_tables");

	// ע�ắ��
	RegisterModules();

	// ����main�ű�,����start����ʼִ��
	DoFileIfExists(l,"main");
	OnMainStart();

}

/**
*	\brief ˢ��Lua
*
*	����ˢ��timers,menu,drawable,movements
*/
void LuaContext::Update()
{
	UpdateTimers();

	OnMainUpdate();
	Debug::CheckAssertion(lua_gettop(l) == 0,
		"There are something in lua stack after update");
}


/**
*	\brief �ر�lua
*/
void LuaContext::Exit()
{
	if (l != nullptr)
	{
		DestoryTimers();

		lua_close(l);
		mLuaContexts.erase(l);
		l = nullptr;
	}
}

bool LuaContext::DoFileIfExists(lua_State* l, const string& name)
{
	if (LoadFile(l, name))
	{
		// if exists,call function
		LuaTools::CallFunction(l,0,0,name);
		return true;
	}
	return false;
}

/**
*	\brief ����һ��lua�ļ�������ļ�����������ջ��
*/
bool LuaContext::LoadFile(lua_State* l, const string& name)
{
	string fileName(name);
	// ��������ڣ���ĩβ����Lua���ٳ���
	if(!FileData::IsFileExists(fileName))
		fileName += ".lua";

	// ������ɲ������򷵻�
	if (!FileData::IsFileExists(fileName))
		return false;

	const string buffer = FileData::ReadFile(fileName);
	int result = luaL_loadbuffer(l, buffer.data(), buffer.size(), fileName.c_str());

	if (result != 0)
	{
		lua_pop(l, 1);
		// error
		return false;
	}
	return true;
}

bool LuaContext::FindMethod(const string& name)
{
	return FindMethod(name,-1);
}

/**
*	\brief ����ָ���������ķ�������������򽫷�������ջ��
*
*	\return������������ڣ�����false
*/
bool LuaContext::FindMethod(const string & name, int index)
{
	int positiveIndex = LuaTools::GetPositiveIndex(l,index);	// ��þ�������
	
	if (lua_isnil(l, positiveIndex))
		return false;
							// -- object ---
	lua_getfield(l, positiveIndex, name.c_str());
							// -- object -- function/nil
	if (!lua_isfunction(l, -1))
	{
		lua_pop(l, 1);
		return false;
	}
	lua_pushvalue(l, index);// objectҪ��Ϊ����
							// -- object -- function object
	return true;
}



/**
*	\brief ע��C++ API ����
*
*	��ȡmoduleName��global�����������ڱ���
*/
void LuaContext::RegisterFunction(const string & moduleName, const luaL_Reg * functions)
{
	lua_getglobal(l, moduleName.c_str());
	if (lua_isnil(l, -1))	
	{	
		lua_pop(l, 1);
		lua_newtable(l);
						// table
		lua_setglobal(l, moduleName.c_str());
						// --
		lua_getglobal(l, moduleName.c_str());
						// table
	}
	if (functions != nullptr)
		luaL_setfuncs(l, functions,0);

	lua_pop(l, 1);
}

void LuaContext::OnStart()
{
	if (FindMethod("onStarted"))
		LuaTools::CallFunction(l, 1, 0, "onStarted");
}

void LuaContext::OnUpdate()
{
	if (FindMethod("onUpdated"))
		LuaTools::CallFunction(l, 1, 0, "onUpdated");
}

void LuaContext::OnFinish()
{
	if (FindMethod("onFinished"))
		LuaTools::CallFunction(l, 1, 0, "onFinished");
}

void LuaContext::RegisterModules()
{
	RegisterMainModule();
	RegisterTimeModule();
}

LuaContext& LuaContext::GetLuaContext(lua_State* l)
{
	return *mLuaContexts[l];
}

/**
*	\brief unprotected lua error����ʱ����
*/
int LuaContext::api_panic(lua_State*l)
{
	string errmsg = LuaTools::CheckString(l, -1);
	Debug::Die(errmsg);
	return 0;
}