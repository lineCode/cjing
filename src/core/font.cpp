#include"core\font.h"

namespace font {

Font::Font()
{
}

Font::Font(const string & font)
{
}

Font::~Font()
{
}

void Font::RenderText()
{
}

/**
*	\brief ��Ⱦ����
*	\param size �Ӻ�
*	\param pos  λ��
*	\param cols ����
*	\param clipRect �ü�����
*	\param renderText ��Ⱦ����
*/
void Font::ReaderText(float size, const Point2 & pos, int cols, const Rect & clipRect, const string & renderText)
{
	// �ַ�����

	// �ַ�����
}

/***** ***** *****  Glyph ***** ***** *****/

Font::Glyph* Font::FindGlyph()
{
	return nullptr;
}

const Font::Glyph* Font::FindGlyph() const
{
	return nullptr;
}

Font::Glyph::Glyph()
{
}

}