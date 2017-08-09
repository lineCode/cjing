#pragma once

#include"common\common.h"
#include<map>

/**
*	\brief ����window/widge������
*/
namespace gui
{
class Config;
class Widget;

/**
*	\brief ����widget������Ļ���
*
*	���崴������widget�����������ʵ��
*/
class BuilderWidget
{
	using replacementMap = std::map<string, std::shared_ptr<BuilderWidget> >;
public:
	BuilderWidget(const Config& config);
	virtual ~BuilderWidget();

	virtual Widget* Build()const = 0;

	string mID;
	string mLinkedGroup;
};

using BuilderWidgetPtr = std::shared_ptr<BuilderWidget>;

}