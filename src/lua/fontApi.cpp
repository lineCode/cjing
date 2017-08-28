#include"luaContext.h"
#include"game\textDrawable.h"

const string LuaContext::module_font_name = "Text";

const std::map<unsigned int, string> horizontalAlignNames = {
	{ font::TEXT_ALIGN_LEFT,   "left"  },
	{ font::TEXT_ALIGN_CENTER, "center" },
	{ font::TEXT_ALIGN_RIGHT,  "right" }
};

/**
*	\brief ע��videoģ��
*/
void LuaContext::RegisterFontModule()
{
	static const luaL_Reg functions[] = {
		{ "create",text_api_create },
		{ nullptr,nullptr }
	};

	static const luaL_Reg methods[] = {
		{ "setFont", text_api_set_font },
		{ "getFont", text_api_get_font },
		{ "setText", text_api_set_text },
		{ "getText", text_api_get_text },
		{ "setFontColor", text_api_set_color },
		{ "getFontColor", text_api_get_color },
		{ "setFontSize", text_api_set_size },
		{ "getFontSize", text_api_get_size },
		{ "setLineHeight", text_api_set_line_height },
		{ "setLetterSpacing", text_api_set_letter_spacing },
		{ "draw", text_api_draw },
		{ "getPos", drawable_api_get_pos },		// ����ķ���Ӧ����drawapiʵ�֣�������sprtie,��δʵ��
		{ "setPos", drawable_api_set_pos },		
		{ "runMovement", drawable_api_run_movement },
		{ "getMovment", drawable_api_get_movement },
		{ "stopMovement", drawable_api_stop_movment },
		{nullptr, nullptr}
	};
	
	static const luaL_Reg metamethods[] = {
		{"__gc", userdata_meta_gc},
		{nullptr, nullptr},
	};

	RegisterType(module_font_name, functions, methods, metamethods);
}

/**
*	\brief ��Textѹջ
*/
void LuaContext::PushText(lua_State*l, TextDrawable& textDrawable)
{
	PushUserdata(l, textDrawable);
}

/**
*	\brief ���ջ��index�������Ƿ�ΪtextDrawable userdata
*	\return the sprite
*/
std::shared_ptr<TextDrawable> LuaContext::CheckTextDrawable(lua_State*l, int index)
{
	return std::static_pointer_cast<TextDrawable>(CheckUserdata(l, index, module_font_name));
}

/**
*	\brief ����ջ��index�������Ƿ�textDrawable
*	\return true �����textDrawable
*/
bool LuaContext::IsTextDrawable(lua_State*l, int index)
{
	return IsUserdata(l, index, module_font_name);
}

/**
*	\brief ʵ��cjing.Text.create()
*
*	���ڴ���ĵ�һ������ӦΪһ�������������������������ֵ
*	font font����fonAltas�����������
*	horizontalAlign ˮƽ����ģʽ
*	fontSize ���Ƶ������С
*	fontColor ���Ƶ�������ɫ
*	lineHeight �и�
*	letterSpacing ˮƽ�ּ��
*/
int LuaContext::text_api_create(lua_State* l)
{
	return LuaTools::ExceptionBoundary(l, [&] {
		TextDrawablePtr textDrawable = std::make_shared<TextDrawable>();

		// property table
		const string& fontID = LuaTools::CheckFieldStringByDefault(l, 1, "fontName", font::DEFAULT_FONT_NAME);
		int fontSize = LuaTools::CheckFieldIntByDefault(l, 1, "fontSize", 0);
		int lineHeight = LuaTools::CheckFieldIntByDefault(l, 1, "lineHeight", 0);
		int letterSpacing = LuaTools::CheckFieldIntByDefault(l, 1, "letterSpacing", 0);
		const Color4B fontColor = LuaTools::CheckFieldColorByDefault(l, 1, "fontColor", Color4B::WHITE);
		// ....
		auto fontPtr = font::FontAtlas::GetInstance().GetFontByName(fontID);
		textDrawable->SetFont(fontPtr);
		textDrawable->SetTextColor(fontColor);

		if (lineHeight > 0)
			textDrawable->SetLineHeight(lineHeight);
		if (fontSize > 0)
			textDrawable->SetFontSize(fontSize);
		if (letterSpacing > 0)
			textDrawable->SetLetterSpacing(letterSpacing);

		GetLuaContext(l).AddDrawable(textDrawable);
		PushText(l, *textDrawable);

		return 1;
	});
}