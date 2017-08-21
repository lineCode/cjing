#pragma once

#include"common\common.h"
#include"core\textureCache.h"
#include"game\sprite.h"
#include"utils\point.h"
#include"utils\size.h"
#include"utils\rectangle.h"
#include"core\renderer.h"

#include"ft2build.h"
#include"freetype.h"
#define FT_FREETYPE_H

namespace font{

struct FontConfig;

/**
*	\brief Font �������
*
*	����freetypeʵ��������Ⱦ�������Լ����ṩ�ı�����
*	���ܣ�Font��ά��һ���������������������ŵ�ǰʹ��
*	����һϵ�����Σ�ÿ��updateʱ����ɾ��ʹ�����ٵ���
*	�Ա�Ϊ�����ӵ������ṩ�ռ�.Font����Ⱦ����״̬ģʽ
*
*	δ����ʹ��Pangocario��Ϊ��������
*/
class Font
{
public:
	enum
	{
		FONT_CODE_LATIN_START = 0x0021,
		FONT_CODE_LATIN_END   = 0x00A6,
		FONT_CODE_CHINESE_START = 0x4E00,
		FONT_CODE_CHINESE_END  = 0x9FA5,
	};

	Font();
	Font(const string& font);
	~Font();

	Font(const Font& other) = delete;
	Font& operator=(const Font& other) = delete;	

	void UnLoad();
	void Flush();
	void LoadFont();
	void BuildLookupTable();

	void RenderText();
	void RenderText(float size, const Point2& pos, const Rect& clipRect, const string& renderText);
	void RenderText(float size, const Point2 & pos, float wrapWidth, const Rect & clipRect, const char* textBegin, const char* textEnd);

	/**** **** ****  setter/gettter **** **** ****/
	bool IsLoaded()const
	{
		return mIsLoaded;
	}
	bool IsDirty()const
	{
		return mIsDirty;
	}

private:
	/**** **** ****  Glyph **** **** ****/
	/*
	*	\brief ���νṹ�������������������е�λ��
	*	�Լ����Ƶ������Ϣ
	*/
	struct Glyph
	{		
		wchar charCode;
		GLenum chanel;
		float advance;		// �ֵ���㵽��һ���ֵ����
		float u0, v0, u1, v1;
	};

	float mFontSize;
	float mScale;
	float mAdvance;
	float mAscent;			// baseline֮�����ַ���ߴ��ľ���
	float mDescent;			// baseline֮�����ַ���ʹ��ľ���

	std::map<wchar, Glyph> mGlyphs;	// �洢���е�glyph

	std::map<Glyph, uint32_t> mTimesGlyphs;	// LRU�ķ�ʽά���ñ����д洢�е��ô���

	string mFontName;
	FontConfig* mConfig;
	FT_Library ft;
	FT_Face face;

public:
	Glyph* FindGlyph(wchar code);
	const Glyph* FindGlyph(wchar code)const;
	void InsertGlyph(const Glyph& glyph);

	void SetFontColor(const Color4B& color);
	const Color4B GetFontColor()const;

	void SetDynamicLoadTexture(bool isDynamicLoad , const Size& size);
	bool IsDynamicLoadTexture()const;
private:
	/**** **** ****  Render **** **** ****/
	void InitRender();
	void CalcTextureRect(int width, int height);
	void SetCharSize(const Size& size);
	void SetCharTexs(float u0, float v0, float u1, float v1);
	void SetCharScale(float scale);

	void Render(const Point2 & pos, const Size& size, float rotate);
	void Render(Renderer& renderer, const Matrix4& transform);

	Quad mQuad;					
	QuadCommand mQuadCommand;
	TexturePtr mTexture;	
	GLProgramStatePtr mProgramState;
	BlendFunc mBlendFunc;
	Matrix4 mModelView;

	bool mIsDynamicLoad;			// �Ƿ�ȫ������
	Size mFontTextureSize;
	bool mIsDirty;
	bool mIsLoaded;
};

using FontPtr = std::shared_ptr<Font>;
using FontConstPtr = std::shared_ptr<const Font>;

}