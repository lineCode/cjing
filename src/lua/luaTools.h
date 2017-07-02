#ifndef _LUATOOLS_H_
#define _LUATOOLS_H_

#include"common\common.h"
#include"lua\luaException.h"
#include"lua\luaRef.h"
#include<lua.hpp>

/**
*	\brief lua���ߺ���
*/
namespace LuaTools
{
	// public method
	LuaRef CreateRef(lua_State*l);
	LuaRef CreateRef(lua_State*l, int index);
	bool CallFunction(lua_State*l,int arguments,int results,const string& functionName);
	int  GetPositiveIndex(lua_State*l, int index);
	void CheckType(lua_State*l, int index, int exceptedType);
	int  CheckInt(lua_State*l, int index);
	int  CheckFieldInt(lua_State*l, int tableIndex, const string& name);
	string  CheckFieldString(lua_State*l, int tableIndex, const string& name);
	int   CheckFieldIntByDefault(lua_State*l, int tableIndex, const string&name, int defaultValue);
	string CheckString(lua_State*l,int index);
	LuaRef CheckFunction(lua_State*l, int index);
	void Error(lua_State*l, const string& message);
	void ArgError(lua_State*l, int index, const string&message);

	// template
	template<typename Callable>
	int  ExceptionBoundary(lua_State*l, Callable&& func);

	template<typename E>
	E CheckEnum(lua_State*l, int index);

	template<typename E>
	E CheckEnum(lua_State*l, int index, const std::map<E, string>& names);

}

#include"luaTools.inl"

#endif