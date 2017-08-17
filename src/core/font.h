#pragma once

#include"common\common.h"
#include"core\textureCache.h"
#include"game\sprite.h"
#include"utils\point.h"
#include"utils\size.h"
#include"utils\rectangle.h"

namespace font{

/**
*	\brief Font �������
*
*	����freetypeʵ��������Ⱦ������
*	�Լ����ṩ�ı����ֹ��ܣ�Font��ά��һ����������
*	�����������ŵ�ǰʹ������һϵ�����Σ�ÿ��update
*	ʱ����ɾ��ʹ�����ٵ��֣��Ա�Ϊ�����ӵ������ṩ�ռ�
*/
class Font
{
public:
	Font();
	~Font();

	Font(const Font& other) = delete;
	Font& operator=(const Font& other) = delete;	

	void RenderText();
	void ReaderText(float size, const Point2& pos, int cols, const Rect& clipRect, const string& renderText);
	
private:
	/*** *** ***  Glyph *** *** ***/
	struct Glyph
	{
	public:
		Glyph();

	};

	Glyph& FindGlyph();
	const Glyph& FindGlyph()const;

public:


private:
	


};

}