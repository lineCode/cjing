#pragma once

#include"common\common.h"
#include"lua\luaData.h"
#include"utils\size.h"

/**
*	\brief ��ԴĿ¼����
*
*	ʹ��physfs����ָ������ԴĿ¼
*/
namespace ConfigData
{
	/**
	*	\brief ���������ļ�
	*/
	class ConfigProperties : public LuaData
	{
	public:
		static ConfigProperties& GetInstance();

		Size GetNormalSize()const;
		const string GetTitle()const;
		const string GetWriteDir()const;

		void SetNormalSize(const Size& size);
		void SetTitle(const string& title);
		void SetWriteDir(const string& dir);

		virtual bool ImportFromLua(lua_State*l);
		static int LuaLoadConfig(lua_State* l);

	private:
		Size mNormalSize;
		string mTitle;
		string mWriteDir;

	};
	void LoadConfig(const string& config);

	ConfigProperties& GetConfigProperties();
}