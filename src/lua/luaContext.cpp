#include"luaContext.h"
#include"core\fileData.h"
#include"core\debug.h"
#include"core\logger.h"
#include"game\app.h"
#include"game\enumInfo.h"
#include<sstream>

std::map<lua_State*, LuaContext*> LuaContext::mLuaContexts;
const string LuaContext::module_name = "cjing";

LuaContext::LuaContext(App* app):
	mApp(app)
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

	lua_newtable(l);
	lua_setglobal(l, module_name.c_str());
	// ע�ắ��
	RegisterModules();

	// �Զ���lua require �����������ڽ����������Դ�ļ�


	// ����main�ű�,����start����ʼִ��
	Debug::CheckAssertion(lua_gettop(l) == 0, "The lua stack is not empty.");
	DoFileIfExists(l,"main");
	Debug::CheckAssertion(lua_gettop(l) == 0, "The lua stack is not empty.");

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
	UpdateMenus();

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
		DestoryMenus();

		lua_close(l);
		mLuaContexts.erase(l);
		l = nullptr;
	}
}

App * LuaContext::GetApp() const
{
	return mApp;
}

/**
*	\brief ��Ӧ�����¼�
*
*   ����OnMainInput��������mainȥ�����¼�
*/
bool LuaContext::NotifyInput(const InputEvent & event)
{
	Debug::CheckAssertion(lua_gettop(l) == 0, "Stack must empty before NotifyInput.");
	bool handle = OnMainInput(event);
	Debug::CheckAssertion(lua_gettop(l) == 0, "Stack must empty after NotifyInput.");
	return handle;
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
	lua_pushvalue(l, positiveIndex);// objectҪ��Ϊ����
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
	if (!moduleName.empty())
	{
		lua_getglobal(l, module_name.c_str());
		// cjing
		lua_getfield(l, -1, moduleName.c_str());
		// cjing module/nil
		if (lua_isnil(l, -1))
		{
			lua_pop(l, 1);
			lua_newtable(l);
			// cjing module
		}
		if (functions != nullptr)
			luaL_setfuncs(l, functions, 0);
		lua_setfield(l, -2, moduleName.c_str());
		// cjing
		lua_pop(l, 1);
		// --
	}
	else
	{
		luaL_setfuncs(l, functions, 0);
	}
}

/**
*	\brief ע��һ��C++��������
*/
void LuaContext::RegisterType(const string & moduleName, const luaL_Reg * functions, const luaL_Reg * methods, const luaL_Reg * metamethods)
{
	luaL_getmetatable(l, moduleName.c_str());
	Debug::CheckAssertion(lua_isnil(l, 1 - 1), string("Type:") + moduleName + " has already registered.");

	// ע�ắ��
	if (functions != nullptr )
	{
		RegisterFunction(moduleName, functions);
	}

	luaL_newmetatable(l, moduleName.c_str());
	lua_pushstring(l, moduleName.c_str());	
							// meta moduleName
	lua_setfield(l, -2, "__CJING_TYPE");
							// meta
	// ע�᷽��
	if (methods != nullptr )
	{
		RegisterFunction("", methods);
	}
	// ע��Ԫ����
	if (metamethods != nullptr)
	{
		RegisterFunction("", metamethods);
	}						// meta
	lua_getfield(l, -1, "__index");
							// meta index/nil
	lua_pushvalue(l, -2);
							// meta __index/nil 
	if ( lua_isnil(l, -2))
	{							
		lua_setfield(l, -3, "__index");
	}						// meta nil
	lua_settop(l, 0);
							// --
}

/**
*	\brief Ϊջ��Ԫ�ش���һ���µ�LuaRef����
*/
LuaRef LuaContext::CreateRef()
{
	return LuaTools::CreateRef(l);
}

/**
*	\brief ��LuaRef���ö���ѹջ
*
*	���LuaRefΪ�գ���ѹ��nil
*/
void LuaContext::PushRef(lua_State * l, const LuaRef & luaref)
{
	if (luaref.IsEmpty())
	{
		lua_pushnil(l);
		return;
	}
	// need to judge these lua_State?
	luaref.Push();
}

/**
*	\brief ��ӡ��ǰLuaջ��Ϣ
*/
void LuaContext::PrintLuaStack(lua_State * l)
{
	int stackSize = lua_gettop(l);
	std::ostringstream oss;
	for (int i = 1; i <= stackSize; i++)
	{
		switch (lua_type(l,i))
		{
		case LUA_TSTRING:
			oss << "\"" << lua_tostring(l, i) << "\"";
			break;
		case LUA_TBOOLEAN:
			oss << (lua_toboolean(l,i) ? "True" : "False");
			break;
		case LUA_TNUMBER:
			oss << (lua_tonumber(l, i));
			break;
		default:
			oss << lua_typename(l, lua_type(l,i));
			break;
		}
		oss << endl;
	}
	Logger::Debug(oss.str());
}

/**
*	\brief ѹ���û���������
*/
void LuaContext::PushUserdata(lua_State * l, LuaObject & userData)
{
}

/**
*	\brief ���ָ��λ�õ�lua�û����󣬷��ظö���
*/
const LuaObjectPtr LuaContext::CheckUserData(lua_State * l, int index, const string & moduleName)
{
	return LuaObjectPtr();
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

/**
*	\brief �����¼���Ӧ
*
*	����event������(type��state)���ö�Ӧ��Lua����
*/
bool LuaContext::OnInput(const InputEvent & event)
{
	bool handle = false;
	if (event.IsKeyBoardEvent())
	{	// �����¼�
		if (event.IsKeyBoardPressed())
			handle = OnKeyPressed(event);
		if (event.IsKeyBoardReleased())
			handle = OnKeyReleased(event);
	}
	else if (event.IsMouseEvent())
	{	// ����¼�
	}		
	return handle;;
}

/**
*	\brief ��Ӧ���̰����¼�
*
*	onKeyPressed(key,modifier),����3������selfΪ������key���µļ�ֵ,
*   modifier�Ƿ�ͬʱ����ctrl/shift/alt lua�ĺ�������ֵ��ʾ�Ƿ���һ��
*   �ɴ��ݵ���Ч�¼�
*   \return ����ֵhandle���Ϊtrue�����ʾ�¼��Ѿ��������贫��
*/
bool LuaContext::OnKeyPressed(const InputEvent & event)
{
	bool handle = false;
	if (FindMethod("onKeyPressed"))	// ע��FindMethod�ὫOjb����һ�ݷ���stack��
	{								// obj method obj
		string key = EnumToString(event.GetKeyBoardKey());
		if (!key.empty())
		{
			lua_pushstring(l, key.c_str());
			lua_newtable(l);	// ����modifiers�����������Ƿ�ͬʱ�����������ΰ�������Ҫ����
								// ctrl��alt��shift)
			if (event.IsWithKeyShift())
			{
				lua_pushboolean(l, 1);
				lua_setfield(l, -2, "shift");
			}
			if (event.IsWithKeyAlt())
			{
				lua_pushboolean(l, 1);
				lua_setfield(l, -2, "alt");
			}
			if (event.IsWithKeyCtrl())
			{
				lua_pushboolean(l, 1);
				lua_setfield(l, -2, "ctrl");
			}
			bool success = LuaTools::CallFunction(l, 3, 1, "onKeyPressed");
			if (!success)
				handle = true;
			else
			{
				handle = lua_toboolean(l, -1);
				lua_pop(l, 1);
			}
		}
		else// obj mothod obj
			lua_pop(l, 2);
	}
	return handle;
}

/**
*	\brief ��Ӧ���̷ſ��¼�
*/
bool LuaContext::OnKeyReleased(const InputEvent & event)
{
	bool handle = false;
	if (FindMethod("onKeyReleased"))// ע��FindMethod�ὫOjb����һ�ݷ���stack��
	{								// obj method obj
		string key = EnumToString(event.GetKeyBoardKey());
		if (!key.empty())
		{
			lua_pushstring(l, key.c_str());
			lua_newtable(l);// modifiers

			if (event.IsWithKeyAlt())
			{
				lua_pushboolean(l, 1);
				lua_setfield(l, -2, "alt");
			}
			if (event.IsWithKeyShift())
			{
				lua_pushboolean(l, 1);
				lua_setfield(l, -2, "shift");
			}
			if (event.IsWithKeyCtrl())
			{
				lua_pushboolean(l, 1);
				lua_setfield(l, -2, "ctrl");
			}
			bool success = LuaTools::CallFunction(l, 3, 1, "onKeyReleased");
			if (!success)
				handle = true;
			else
			{
				handle = lua_toboolean(l, -1);
				lua_pop(l, 1);
			}
		}
		else // obj method obj
			lua_pop(l, 2);
	}
	return handle;
}

void LuaContext::RegisterModules()
{
	RegisterMainModule();
	RegisterVideoModule();
	RegisterMenuModule();
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