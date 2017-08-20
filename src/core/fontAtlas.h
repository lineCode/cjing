#pragma once

#include"common\common.h"
#include"font.h"

namespace font {

/**
*	\brief ����������
*/
struct FontConfig
{
	FontConfig();

	string name;

	int   fontIndex;
	void* fontData;
	int   fontDataSize;
	Vec2i GlyphOffset;
	int   sizePixels;			

	bool  merageMode;
};

/**
*	\brief ���弯��
*/
class FontAtlas
{
public:
};

}
