#include "luaRef.h"

LuaRef::LuaRef():
	l(nullptr),
	mRef(LUA_REFNIL)
{
}

LuaRef::LuaRef(lua_State * l, int ref):
	l(l),
	mRef(ref)
{
}

LuaRef::LuaRef(const LuaRef& other):
	l(nullptr),
	mRef(LUA_REFNIL)
{
	*this = other;
}

LuaRef::LuaRef(LuaRef && other):
	l(other.l),
	mRef(other.mRef)
{
	other.l = nullptr;
	other.mRef = LUA_REFNIL;
}

/**
*	\brief ��ֵ���캯��
*
*	��Ϊluaref�������뱣��Ψһ�ԣ����ｫotherӦ�õ�Luaֵ����ѹջ������lua_ref�����µ�����
*/
LuaRef & LuaRef::operator=(const LuaRef& other)
{
	Clear();
	l = other.l;
	if (l != nullptr)
	{
		if (other.mRef == LUA_REFNIL || other.mRef == LUA_NOREF)
			mRef = other.mRef;
		else
		{
			lua_rawgeti(l, LUA_REGISTRYINDEX, other.mRef);
			mRef = luaL_ref(l, LUA_REGISTRYINDEX);
		}
	}
	return *this;
}

LuaRef & LuaRef::operator=(LuaRef & other)
{
	Clear();
	l = other.l;
	mRef = other.mRef;
	other.l = nullptr;
	other.mRef = LUA_REFNIL;
	return *this;
}

LuaRef::~LuaRef()
{
	Clear();
}


bool LuaRef::IsEmpty() const
{
	return l== nullptr || (mRef == LUA_REFNIL || mRef == LUA_NOREF);
}

int LuaRef::GetRef() const
{
	return mRef;
}

/**
*	\brief ɾ��ref����
*
*	����luaL_unref()
*/
void LuaRef::Clear()
{
	if (l != nullptr)
		luaL_unref(l, LUA_REGISTRYINDEX, mRef);
	l = nullptr;
	mRef = LUA_REFNIL;
}

void LuaRef::Push()const
{
	
}

void LuaRef::Call(const std::string & functionName)const
{
}
