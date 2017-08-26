#include"core\font.h"
#include"core\resourceCache.h"
#include"core\fileData.h"
#include"core\debug.h"
#include"thirdparty\SOIL.h"
#include"core\logger.h"

namespace font {

namespace {
	const int MAX_FONT_TEXTURE_WIDTH = 2048;
	const int MAX_FONT_TEXTURE_HEIGHT = 1536;
	const int BEARING_STANDARD_HEIGHT_CODE = 0x4E09;	// ��"��"����׼��
}

Font::Font():
	mFontSize(0),
	mScale(1.0f),
	mAscent(0),
	mDescent(0),
	mLetterSpacing(0),
	mConfig(nullptr),
	mTexture(nullptr),
	mProgramState(nullptr),
	mIsDirty(false),
	mIsLoaded(false),
	mIsDynamicLoad(false)
{
	InitRender();
}

Font::Font(const string & font):
	mFontName(font),
	mFontSize(0),
	mScale(1.0f),
	mAscent(0),
	mDescent(0),
	mLetterSpacing(0),
	mConfig(nullptr),
	mTexture(nullptr),
	mProgramState(nullptr),
	mIsDirty(false),
	mIsLoaded(false),
	mIsDynamicLoad(false)
{
	InitRender();
}

Font::~Font()
{
	UnLoad();
}

void Font::UnLoad()
{
	if (IsLoaded())
	{
		if (IsDynamicLoadTexture())
		{
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
		}
	}
	mGlyphs.clear();
}

/**
*	\brief ˢ�¹���
*
*	ˢ�¹��̻����������ģ�����ڳ��ڲ�ʹ�õ���ģ��
*	��������������ڶ�̬������������ʹ��
*/
void Font::Flush()
{
}

/**
*	\brief ���������ļ�
*/
void Font::LoadFont()
{
	Logger::Info("Start loading Font.");

	if (IsLoaded() || mFontName.empty())
	{
		return;
	}
	if (FT_Init_FreeType(&ft))
	{
		Debug::Error("Faild to init FT_Library.");
	}
	if (!FileData::IsFileExists(mFontName))
	{
		Debug::Error("Font file is not existed.");
	}
	string data = FileData::ReadFile(mFontName);
	if (data.empty())
	{
		Debug::Error("Failed to open font file.");
	}
	if (FT_New_Memory_Face(ft, (unsigned char*)data.c_str(), data.size(), 0, &face))
	{
		Debug::Error("Failed to create new font face.");
	}
	FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	FT_Set_Pixel_Sizes(face, 0, mFontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	mAscent = face->ascender >> 6;
	mDescent = face->descender >> 6;
	
	// ����ͳһ�����ɼ��ַ���С��33��
	for (unsigned int code = 0; code < 33; code++)
	{
		if (FT_Load_Char(face, code , FT_LOAD_RENDER))
		{
			Debug::Warning("failed to load char.");
			continue;
		}
		Glyph glyph;
		glyph.advance = face->glyph->advance.x;
		glyph.charCode = code;
		glyph.size = Size(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		glyph.bearing = Size(face->glyph->bitmap_left, face->glyph->bitmap_top);

		mGlyphs[code] = glyph;
	}

	int ammount = 0;
	if (!IsDynamicLoadTexture())
	{
		// ��δ������̬���������ʱ�򣬼���ȫ����ģ�������������RGBA�ĸ�ͨ��
		// �ٶȹ�����������ʹ���첽����
		mTexture = ResourceCache::GetInstance().CreateTexture2DBySize("mFontName", { MAX_FONT_TEXTURE_WIDTH, MAX_FONT_TEXTURE_HEIGHT });

		glBindTexture(GL_TEXTURE_2D, mTexture->GetTextureID());
		Point2 startPos = Point2(0, 0);
		const int advanceStep = 5;

		// ȫ������Ĭ�ϼ���ȫ�������ַ��������ַ�
		mFontCodeRanges.emplace_back(FontCodeRange(FONT_CODE_LATIN_START, FONT_CODE_LATIN_END));
		mFontCodeRanges.emplace_back(FontCodeRange(FONT_CODE_CHINESE_START, FONT_CODE_CHINESE_END));

		for (const auto& fontCodeRange : mFontCodeRanges)
		{
			for (uint32_t code = fontCodeRange.codeBegin; code <= fontCodeRange.codeEnd; code += 4)
			{
				std::vector<unsigned int> mDstColor;
				unsigned int maxCharWidth = 0;
				unsigned int maxCharHeight = 0;
				for (int i = 0; i < 4; i++)
				{
					ammount++;
					if (FT_Load_Char(face, code + i, FT_LOAD_RENDER))
					{
						Debug::Warning("failed to load char.");
						continue;
					}
					if ((unsigned int)face->glyph->bitmap.pitch > maxCharWidth)
					{
						maxCharWidth = face->glyph->bitmap.pitch;
					}
					if (face->glyph->bitmap.rows > maxCharHeight)
					{
						maxCharHeight = face->glyph->bitmap.rows;
					}
				}
				if (maxCharHeight == 0 || maxCharWidth == 0)
				{
					continue;
				}

				mDstColor.resize(maxCharWidth * maxCharHeight);
				auto ToRGBA = [maxCharWidth, maxCharHeight](GLenum curFormat, const unsigned char* src,
					std::vector<unsigned int>& dst, int srcPitch, int srcRow)
				{
					for (int row = 0; row < srcRow; row++)
					{
						for (int col = 0; col < srcPitch; col++)
						{
							switch (curFormat)
							{
							case GL_RED:
								dst[row * maxCharWidth + col] |= src[row * srcPitch + col] << 0;
								break;
							case GL_GREEN:
								dst[row * maxCharWidth + col] |= src[row * srcPitch + col] << 8;
								break;
							case GL_BLUE:
								dst[row * maxCharWidth + col] |= src[row * srcPitch + col] << 16;
								break;
							case GL_ALPHA:
								dst[row * maxCharWidth + col] |= src[row * srcPitch + col] << 24;
							default:
								break;
							}
						}
					}
				};

				for (int i = 0; i < 4; i++)
				{
					if (FT_Load_Char(face, code + i, FT_LOAD_RENDER))
					{
						Debug::Warning("failed to load char.");
						continue;
					}

					Glyph glyph;
					glyph.advance = face->glyph->advance.x;
					glyph.charCode = code + i;
					glyph.chanel = i;
					glyph.size = Size(face->glyph->bitmap.width, face->glyph->bitmap.rows);
					glyph.bearing = Size(face->glyph->bitmap_left, face->glyph->bitmap_top);

					float u0 = (float(startPos.x) / MAX_FONT_TEXTURE_WIDTH);
					float v0 = (float(startPos.y) / MAX_FONT_TEXTURE_HEIGHT);
					float u1 = (float(startPos.x + face->glyph->bitmap.width)) / MAX_FONT_TEXTURE_WIDTH;
					float v1 = (float(startPos.y + face->glyph->bitmap.rows)) / MAX_FONT_TEXTURE_HEIGHT;
					glyph.u0 = u0;
					glyph.v0 = v0;
					glyph.u1 = u1;
					glyph.v1 = v1;
					mGlyphs[code + i] = glyph;

					ToRGBA(GL_RED + i, face->glyph->bitmap.buffer, mDstColor, face->glyph->bitmap.pitch, face->glyph->bitmap.rows);

				}

				if (startPos.x + maxCharWidth >= MAX_FONT_TEXTURE_WIDTH)
				{
					startPos.x = 0;
					if (startPos.y + maxCharHeight > MAX_FONT_TEXTURE_HEIGHT)
					{
						break;
					}
					startPos.y += maxCharHeight + advanceStep;
				}
				unsigned char* pixels = (unsigned char*)mDstColor.data();
				glTexSubImage2D(GL_TEXTURE_2D, 0, startPos.x, startPos.y, maxCharWidth, maxCharHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				startPos.x += (face->glyph->advance.x >> 6) + advanceStep;
			}
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}
	else
	{
		// ��̬ά��������Ϣ����Ҫָ�������С��Ĭ�������С��ͨ��LRU����ά��
		// δ���汾�������
		for (uint32_t code = FONT_CODE_CHINESE_START; code <= FONT_CODE_CHINESE_END; code ++)
		{
			if (FT_Load_Char(face, code, FT_LOAD_RENDER))
			{
				Debug::Warning("failed to load char.");
				continue;
			}
			Glyph glyph;
			glyph.advance = face->glyph->advance.x;
			glyph.charCode = code;
			glyph.chanel = GL_RED;
			mGlyphs[code] = glyph;
		}
	}
	mIsLoaded = true;
	Logger::Info("Load Font successed.");
}

void Font::BuildLookupTable()
{
}

void Font::RenderText()
{
	Render({ 100,300 }, { 100,100 }, 0);
}

/**
*	\brief ��Ⱦ���ֵ�ָ��λ��
*	\param size �Ӻ�
*	\param pos  λ��
*	\param cols ����
*	\param clipRect �ü�����
*	\param renderText ��Ⱦ����
*/
void Font::RenderText(float size, const Point2 & pos, const Rect & clipRect, float wrapWidth, const string& renderText)
{
	if (renderText.empty())
	{
		return;
	}

	const char* textBegin = renderText.c_str();
	RenderText(size, pos, clipRect, wrapWidth, textBegin);
}

/**
*	\brief ��Ⱦ���ֵ�ָ��λ��
*	\param size �Ӻ�
*	\param pos  λ��
*	\param cols ����
*	\param clipRect �ü�����
*	\param renderText ��Ⱦ����
*/
void Font::RenderText(float size, const Point2 & pos, const Rect & clipRect, float wrapWidth, const char * textBegin, const char * textEnd)
{
	int posX = pos.x;
	int posY = pos.y;

	if (posY >= clipRect.height + clipRect.y)
	{
		return;
	}

	float scale = size / mFontSize;
	int lineHeight = (int)size;
	bool wrapEnabled = (wrapWidth > 0);
	const char* wrapChar = nullptr;
	const char* curCharPtr = textBegin;

	if (textEnd == nullptr)
	{
		textEnd = textBegin + strlen(textBegin);
	}

	// ѭ�������ַ����Ƚ������Ⱦ
	while (curCharPtr < textEnd)
	{
		// ������
		if (wrapEnabled)
		{
			if (wrapChar == nullptr)
			{
				wrapChar = implementation::GetWordWraoPosition(curCharPtr, textEnd, scale, wrapWidth - (posX - pos.x));
			}
			if (curCharPtr >= wrapChar)
			{
				posX = pos.x;
				posY += lineHeight;
				wrapChar = nullptr;

				continue;
			}
		}
		unsigned int curChar = (unsigned int)(*curCharPtr);
		if (curChar < 0x80)
		{	// ���ֽ�
			curCharPtr++;
		}
		else
		{	// ���ֽ�
			curCharPtr += implementation::ParsedUtf8(curChar, curCharPtr, textEnd);
			if (curChar == 0)
			{
				Debug::Warning(string("Invalid text to render. ") + textBegin);
				break;
			}
		}
		// ����ת�Ʒ�
		if (curChar < 32)
		{
			if (curChar == '\n')
			{
				posX = pos.x;
				posY += lineHeight;
				if (posY >= clipRect.height + clipRect.y)
				{
					break;
				}
				if (!wrapEnabled && posY + lineHeight < clipRect.y)
				{
					while (curCharPtr < textEnd && *curCharPtr != '\n')
						curCharPtr++;
				}
				continue;
			}
		}


		// render
		int charWidth = 0;
		if (const Glyph* glyph = FindGlyph(curChar))
		{
			charWidth = (glyph->advance >> 6);

			// ���ڿո�����Ʊ����ֱ�Ӳ�������Ⱦ����
			if (curChar != ' ' && curChar != '\t')
			{
				const Size charSize = Size(int(glyph->size.width * scale),
					int(glyph->size.height * scale));

				SetCharTexs(glyph->u0, glyph->v0, glyph->u1, glyph->v1);
				SetCharSize(charSize);
				if (!IsDynamicLoadTexture())
					SetCharChanel((float)glyph->chanel);

				int x = posX + (int)(glyph->bearing.width * scale);
				int y = posY + (mAscent - (int)glyph->bearing.height);

				Render(Point2(x, y), charSize, 0);
			}
		}
		posX += (int)(charWidth * scale) + mLetterSpacing;
	}
}

/***** ***** *****  Glyph ***** ***** *****/

Font::Glyph* Font::FindGlyph(wchar code)
{
	auto itor = mGlyphs.find(code);
	if (itor != mGlyphs.end())
	{
		return &itor->second;
	}
	return nullptr;
}

const Font::Glyph* Font::FindGlyph(wchar code) const
{
	auto itor = mGlyphs.find(code);
	if (itor != mGlyphs.end())
	{
		return &itor->second;
	}
	return nullptr;
}

/**
*	\brief ����������ɫ
*
*   ��ʹ���������ɫ��ʱ��ֱ��Ӱ����ʾ����ɫ
*/
void Font::SetFontColor(const Color4B & color)
{
	mQuad.lt.colors = color;
	mQuad.lb.colors = color;
	mQuad.rt.colors = color;
	mQuad.rb.colors = color;
}

const Color4B Font::GetFontColor() const
{
	return mQuad.rb.colors;
}

void Font::SetFontSize(int size)
{
	mFontSize = size;
}

int Font::GetFontSize() const
{
	return mFontSize;
}

void Font::SetLetterSpacing(int spacing)
{
	mLetterSpacing = spacing;
}

int Font::GetLetterSpacing() const
{
	return mLetterSpacing;
}

void Font::SetDynamicLoadTexture(bool isLoadAll, const Size & size)
{
	if (IsLoaded())
	{
		Debug::Warning("Set Load all texture must before font::load().");
		return;
	}
	mIsDynamicLoad = isLoadAll;
	mFontTextureSize = size;
}

bool Font::IsDynamicLoadTexture() const
{
	return mIsDynamicLoad;
}

/**
*	\biref ��ʼ����Ⱦ״̬
*/
void Font::InitRender()
{
	// test init
	SetCharSize({ 512, 480 });
	SetCharTexs(0.0f, 0.0f, 1.0f, 1.0f);
	SetFontSize(16);
	SetFontColor(Color4B::WHITE);
	SetLetterSpacing(0);

	// ��ʼ����Ⱦ״̬
	mProgramState = ResourceCache::GetInstance().GetGLProgramState(GLProgramState::DEFAULT_FONT_NORMAL_PROGRAMSTATE_NAME);
	mModelView = Renderer::GetInstance().GetCameraMatrix();
}

void Font::CalcTextureRect(int width, int height)
{
}

/**
*	\brief ��renderer�ύ��Ⱦ����
*/
void Font::Render(Renderer & renderer, const Matrix4 & transform)
{
	Debug::CheckAssertion(mProgramState != nullptr, "Invaild programState in Font::Rener().");

	// Ϊ�����ɶ��quadCommand����Ϊ��֧�ֶ�̬�����ﴴ��ֻ��ָ�봫��,�ֶ������ڴ�
	// δ����Ҫ���Ǹ����Ĵ��ݷ�ʽ
	auto quadCommand = new QuadCommand();

	quadCommand->Init(0, mProgramState,
		mTexture != nullptr ? mTexture->GetTextureID() : 0,	// ������Ҫ����������ɫ��
		mQuad, 1, mBlendFunc, transform, mModelView);

	renderer.PushCommand(quadCommand);
}

/**
*	\brief ��ָ��λ�á�ָ����С��ָ���ǶȻ�����
*
*   ����λ�úͽǶ����ɶ�Ӧ�ı任����
*/
void Font::Render(const Point2 & pos, const Size& size, float rotate)
{
	Matrix4 transfomr;
	// ��Ϊ���ҳ����꣬���в���˳��Ϊ��������
	// �ƶ���ָ��λ��
	transfomr = Matrix4::Translate(Vec3f((float)pos.x, (float)pos.y, .0f));

	// �ƶ���ԭ����ת���ƻ�
	transfomr *= Matrix4::Translate(Vec3f(size.width*0.5f, size.height*0.5f, 0.0f));
	transfomr *= Matrix4::Rotate(Vec3f(0.0f, 0.0f, 1.0f), rotate);
	transfomr *= Matrix4::Translate(Vec3f(-size.width*0.5f, -size.height*0.5f, 0.0f));

	Render(Renderer::GetInstance(), transfomr);
}

void Font::SetCharSize(const Size& size)
{
	float x1 = 0;
	float y1 = 0;
	float x2 = x1 + size.width;
	float y2 = y1 + size.height;

	mQuad.lb.vertices = Vec3f(x1, y1, .0f);
	mQuad.lt.vertices = Vec3f(x1, y2, .0f);
	mQuad.rb.vertices = Vec3f(x2, y1, .0f);
	mQuad.rt.vertices = Vec3f(x2, y2, .0f);
}

void Font::SetCharTexs(float u0, float v0, float u1, float v1)
{
	mQuad.lt.texs.u = u0;
	mQuad.lt.texs.v = v1;
	mQuad.lb.texs.u = u0;
	mQuad.lb.texs.v = v0;
	mQuad.rt.texs.u = u1;
	mQuad.rt.texs.v = v1;
	mQuad.rb.texs.u = u1;
	mQuad.rb.texs.v = v0;
}

/**
*	\brief ���õ����ַ������ű���
*
*	��������Ĵ�С�ڼ�������ʱ���Ѿ����ã�����scale������
*	����ָ����С�趨���ű���
*/
void Font::SetCharScale(float scale)
{
	mScale = scale;
}

/**
*	\brief ���õ�ǰ�ַ�����Ⱦ������ͨ��
*
*	��ʹ��ȫ�����ص�ʱ��ʹ��
*/
void Font::SetCharChanel(float chanel)
{
	mQuad.lb.vertices.z = chanel;
	mQuad.lt.vertices.z = chanel;
	mQuad.rb.vertices.z = chanel;
	mQuad.rt.vertices.z = chanel;
}

/**
*	\brief ����utf8�����ַ�
*	\param outChar �������ַ�ֵ
*	\param textBegin ���뿪ʼ��λ��
*	\param textEnd ���������λ��
*	\return utf8���ֽڸ���
*
*	10xxxxxx
*	110xxxxx 10xxxxxx
*	1110xxxx 10xxxxxx 10xxxxxx
*	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/
int implementation::ParsedUtf8(unsigned int& outChar, const char * textBegin, const char * textEnd)
{
	unsigned int result = (unsigned int)-1;
	const unsigned char* curChar = (const unsigned char*)textBegin;

	// ���ֽ�
	if ((*curChar & 0xc0) == 0x80)
	{
		result = (unsigned int)(*curChar++);
		outChar = result;
		return 1;
	}
	// 2�ֽ� 
	if ((*curChar & 0xe0) == 0xc0)
	{
		result = (unsigned int)((*curChar++ & 0x1F) << 6);
		if ((*curChar & 0xc0) != 0x80)
			return 2;
		result += (unsigned int)(*curChar++ & 0x3f);
		outChar = result;
		return 2;
	}

	// 3�ֽ�
	if ((*curChar & 0xf0) == 0xe0)
	{
		result = (unsigned int)((*curChar++ & 0x0F) << 12);
		if ((*curChar & 0xc0) != 0x80)
			return 3;
		result += (unsigned int)((*curChar++ & 0x3f) << 6);
		if ((*curChar & 0xc0) != 0x80)
			return 3;
		result += (unsigned int)(*curChar++ & 0x3f);
		outChar = result;
		return 3;
	}

	// 4�ֽ�
	if ((*curChar & 0xf8) == 0xf0)
	{
		result = (unsigned int)((*curChar++ & 0x07) << 18);
		if ((*curChar & 0xc0) != 0x80)
			return 4;
		result += (unsigned int)((*curChar++ & 0x3f) << 12);
		if ((*curChar & 0xc0) != 0x80)
			return 4;
		result += (unsigned int)((*curChar++ & 0x3f) << 6);
		if ((*curChar & 0xc0) != 0x80)
			return 4;
		result += (unsigned int)(*curChar++ & 0x3f);
		outChar = result;
		return 4;
	}
	outChar = 0;
	return 0;
}

/**
*	\brief ���㻻�е��ַ�
*/
const char* implementation::GetWordWraoPosition(const char * textBegin, const char * textEnd, float scale, float wrapWidth)
{
	float wordWidth = 0.0f;
	const char* curCharPtr = textBegin;
	const char* nextCharPtr = nullptr;

	// ��Ҫ����utf8���ֽںͶ��ֽ����
	while (curCharPtr < textEnd)
	{
		unsigned int curChar = (unsigned int)*curCharPtr;
		if (curChar < 0x80)
		{
			nextCharPtr = curCharPtr + 1;
		}
		else
		{
			nextCharPtr = curCharPtr + implementation::ParsedUtf8(curChar, curCharPtr, textEnd);
		}
		if (curChar == 0)
		{
			break;
		}
		
		const float charWidth = 0;
		wordWidth += charWidth;

		if (wordWidth >= wrapWidth)
		{
			return nextCharPtr;
			break;
		}
		curCharPtr = nextCharPtr;

	}

	return curCharPtr;
}

}


