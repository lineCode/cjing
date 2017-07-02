#include"lua\luaContext.h"
#include"lua\luaTools.h"
#include"game\sprite.h"
#include"game\enumInfo.h"

const string LuaContext::module_sprite_name = "Sprite";

void LuaContext::RegisterSpriteModule()
{
	static const luaL_Reg function[] = {
		{"Create", sprite_api_create},
		{nullptr,nullptr}
	};

	static const luaL_Reg methods[] = {
		{"SetSize", sprite_api_set_size},
		{"GetSize", sprite_api_get_size},
		{"SetBlend", sprite_api_set_blend},
		{"GetBlend", sprite_api_get_blend},
		{"SetOpacity", sprite_api_set_opacity},
		{"GetOpacity", sprite_api_get_opacity},
		{"Draw", sprite_api_draw},
		{ "SetPos", sprite_api_set_pos },		// ����ķ���Ӧ����drawapiʵ�֣�������sprtie,��δʵ��
		{ "RunMovement", sprite_api_run_movement },
		{ "GetMovment", sprite_api_get_movement },
		{ "StopMovement", sprite_api_stop_movment },
		{nullptr, nullptr}
	};

	static const luaL_Reg metamethods[] = {
		{"__gc", userdata_meta_gc},
		{nullptr, nullptr}
	};
	RegisterType(module_sprite_name, function, methods, metamethods);
}

/**
*	\brief ��spriteѹջ
*/
void LuaContext::PushSprite(lua_State*l, Sprite& sprite)
{
	PushUserdata(l, sprite);
}

/**
*	\brief ���ջ��index�������Ƿ�Ϊsprite userdata
*	\return the sprite
*/
SpritePtr LuaContext::CheckSprite(lua_State*l, int index)
{
	return std::static_pointer_cast<Sprite>(CheckUserdata(l, index, module_sprite_name));
}

/**
*	\brief ����ջ��index�������Ƿ�sprite
*	\return true �����sprite
*/
bool LuaContext::IsSprite(lua_State*l, int index)
{
	return IsUserdata(l, index, module_sprite_name);
}

/**
*	\brief ʵ��cjing.Sprite.create()
*
*	�������2�д�������ķ���,Ŀǰ�����Ƿ�֧�ֵڶ��ִ�ɫ����Ĵ���
*/
int LuaContext::sprite_api_create(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		const string spriteName = LuaTools::CheckString(l, 1);
		
		SpritePtr sprite = std::make_shared<Sprite>(spriteName);
		GetLuaContext(l).AddDrawable(sprite);
		return 1;
	});
}

/**
*	\brief ʵ��sprite:SetSize(x,y)
*/
int LuaContext::sprite_api_set_size(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		Sprite& sprite = *CheckSprite(l, 1);
		int x = LuaTools::CheckInt(l, 2);
		int y = LuaTools::CheckInt(l, 3);

		sprite.SetSize(Size(x, y));

		return 0;
	});
}

/**
*	\brief ʵ��w,h = sprite:GetSize()
*/
int LuaContext::sprite_api_get_size(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		Sprite& sprite = *CheckSprite(l, 1);

		const Size size = sprite.GetSize();
		lua_pushinteger(l, size.width);
		lua_pushinteger(l, size.height);

		return 2;
	});
}

/**
*	\brief ʵ��sprite:SetBlend(srcBlend, dstBlend)
*/
int LuaContext::sprite_api_set_blend(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		Sprite& sprite = *CheckSprite(l, 1);
		GLenum srcBlend = LuaTools::CheckEnum<GLenum>(l, 2);
		GLenum dstBlend = LuaTools::CheckEnum<GLenum>(l, 3);

		sprite.SetBlendFunc(BlendFunc(srcBlend, dstBlend));

		return 0;
	});
}

/**
*	\brief ʵ��sprite:GetBlend,���ν�srcBLend��dstBlendѹ��ջ
*/
int LuaContext::sprite_api_get_blend(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		Sprite& sprite = *CheckSprite(l, 1);
		BlendFunc blend = sprite.GetBlendFunc();

		lua_pushstring(l, EnumToString(blend.srcBlend).c_str());
		lua_pushstring(l, EnumToString(blend.dstBlend).c_str());

		return 2;
	});
}

/**
*	\brief
*/
int LuaContext::sprite_api_set_pos(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		Sprite& sprite = *CheckSprite(l, 1);
		int x = LuaTools::CheckInt(l, 2);
		int y = LuaTools::CheckInt(l, 3);

		sprite.SetPos(Point2(x, y));

		return 0;
	});
}

int LuaContext::sprite_api_get_pos(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		Sprite& sprite = *CheckSprite(l, 1);

		const Point2 pos = sprite.GetPos();
		lua_pushinteger(l, pos.x);
		lua_pushinteger(l, pos.y);

		return 2;
	});
}

/**
*	\brief ʵ��sprite:SetOpacity(alpha)
*/
int LuaContext::sprite_api_set_opacity(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		Sprite& sprite = *CheckSprite(l, 1);
		int alpha = LuaTools::CheckInt(l, 2);
		sprite.SetOpacity(alpha);

		return 0;
	});
}

/**
*	\brief ����͸����sprite:GetOpacity()
*/
int LuaContext::sprite_api_get_opacity(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		Sprite& sprite = *CheckSprite(l, 1);
		int alpha = sprite.GetOpacity();
		lua_pushinteger(l, alpha);

		return 1;
	});
}

int LuaContext::sprite_api_draw(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		Sprite& sprite = *CheckSprite(l, 1);
		/*int x = LuaTools::CheckInt(l, 2);
		int y = LuaTools::CheckInt(l, 3);*/
		sprite.Draw();

		return 0;
	});
}

int LuaContext::sprite_api_run_movement(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {


		return 1;
	});
}

int LuaContext::sprite_api_get_movement(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {


		return 1;
	});
}

int LuaContext::sprite_api_stop_movment(lua_State*l)
{
	return LuaTools::ExceptionBoundary(l, [&] {


		return 1;
	});
}