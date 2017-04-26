#pragma once

#include"common\common.h"
#include"game\enumInfo.h"

/**
*	\brief ���ͷ�ļ���������
*/

enum TestNumber
{
	T_ONE,
	T_TWO,
	T_THREE
};

template<>
struct EnumInfoTraits<TestNumber>
{
	static const string prettyName;
	static const EnumInfo<TestNumber>::nameType names;
};

void Test();