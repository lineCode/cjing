#include "animationData.h"
#include "core\debug.h"
#include "lua\luaTools.h"

AnimationData::AnimationData()
{
}

AnimationData::AnimationData(const string & imageName, const std::deque<AnimationDirectionData>& directions, uint32_t frameDelay, int frameLoop):
	mImageName(imageName),
	mDirections(directions),
	mFrameDelay(frameDelay),
	mFrameLoop(frameLoop)
{
}

string AnimationData::GetImageName() const
{
	return mImageName;
}

uint32_t AnimationData::GetFrameDelay() const
{
	return mFrameDelay;
}

int AnimationData::GetFrameLoop() const
{
	return mFrameLoop;
}

const std::deque<AnimationDirectionData>& AnimationData::GetDirections()
{
	return mDirections;
}

AnimationLuaData::AnimationLuaData()
{
}

/**
*	\brief ��Lua�ļ��е�������
*
*	����ǰAnimationLuaData������LUA_REGISTRE,ͬʱ���ݸ�ʽ��
*	animationע���Ͻ����������ڽ��������洢����
*/
bool AnimationLuaData::ImportFromLua(lua_State * l)
{
	lua_pushlightuserdata(l, this);	// lightuserdata
	lua_setfield(l, LUA_REGISTRYINDEX, "Animation");
	lua_register(l, "animation", LuaAnimation);
	if (lua_pcall(l,0,0,0) != 0)
	{
		Debug::Error("Failed to load animation datafile.");
		lua_pop(l,1);	// ?????
		return false;
	}
	return true;
}

/**
*	\brief animation��ʽ����
*/
int AnimationLuaData::LuaAnimation(lua_State * l)
{
	return LuaTools::ExceptionBoundary(l, [&]{
		lua_getfield(l, LUA_REGISTRYINDEX, "Animation");
		AnimationLuaData* animationData = static_cast<AnimationLuaData*>(lua_touserdata(l, -1));
		lua_pop(l, 1);

		LuaTools::CheckType(l, 1, LUA_TTABLE);
		
		
	});

}

const std::map<string, AnimationData>& AnimationLuaData::GetAnimations() const
{
	return mAnimations;
}

string AnimationLuaData::GetDefaultName() const
{
	return mDefaultName;
}

