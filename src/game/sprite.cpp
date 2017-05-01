#include"sprite.h"
#include"core\texture.h"
#include"core\debug.h"
#include"core\resourceCache.h"

/**
*	\brief ����һ��Ĭ�ϵľ���
*/
Sprite::Sprite():
	mProgramState(nullptr),
	mTexture(nullptr),
	mVisible(true),
	mDirty(true),
	mAnchor(0, 0),
	mBlendFunc()
{
	SetDefaultState();
}

/**
*	\brief ����һ����������ľ���ʵ��
*/
Sprite::Sprite(const std::string & name):
	mProgramState(nullptr),
	mTexture(nullptr),
	mVisible(true),
	mDirty(true),
	mAnchor(0,0),
	mBlendFunc()
{
	InitWithFile(name);
	SetDefaultState();
}

/**
*	\brief ����һ����������ľ���ʵ��
*/
Sprite::Sprite(TexturePtr & tex):
	mProgramState(nullptr),
	mTexture(tex),
	mVisible(true),
	mDirty(true),
	mAnchor(0, 0),
	mBlendFunc()
{
	InitWithTexture(mTexture);
	SetDefaultState();
}

/**
*	\brief ����һ����ɫ��ʹ������ľ���ʵ��
*	\param �������ɫ
*	\param ����Ĵ�С
*/
Sprite::Sprite(const Color4B & color, const Size & size):
	mProgramState(nullptr),
	mTexture(nullptr),
	mVisible(true),
	mDirty(true),
	mAnchor(0, 0),
	mBlendFunc()
{
	SetSize(size);
	SetColor(color);
}

Sprite::~Sprite()
{
	mChildSprites.clear();
}

/**
*	\brief ��������״̬ͬʱ��������children
*/
void Sprite::Update()
{
	if (mSuspended)
		return;

	for (auto& sprite : mChildSprites)
		sprite->Update();
}

/**
*	\brief ���ƾ���
*
*   �Ծ��鱣���λ�ú���ת�ǶȻ��ƾ���
*/
void Sprite::Draw()
{
	if (IsDirty())
		UpdateTransform();

	if(mVisible)
		Draw(GetPos(), GetRotated());
}

/**
*	\brief ��ָ��λ�ú�ָ���ǶȻ��ƾ���
*
*   ����λ�úͽǶ����ɶ�Ӧ�ı任����
*/
void Sprite::Draw(const Point2 & pos, float rotate)
{
	Matrix4 transfomr;
	// ��Ϊ���ҳ����꣬���в���˳��Ϊ��������
	// �ƶ���ָ��λ��
	transfomr =  Matrix4::Translate(Vec3f((float)pos.x,(float)pos.y, .0f));

	// �ƶ���ԭ����ת���ƻ�
	transfomr *= Matrix4::Translate(Vec3f(mSize.width*0.5f, mSize.height*0.5f, 0.0f));
	transfomr *= Matrix4::Rotate(Vec3f(0.0f, 0.0f, 1.0f), rotate);
	transfomr *= Matrix4::Translate(Vec3f(-mSize.width*0.5f, -mSize.height*0.5f, 0.0f));

	Draw(Renderer::GetInstance(), transfomr);
}

/**
*	\brief ��renderer�ύ��Ⱦ����
*/
void Sprite::Draw(Renderer & renderer, const Matrix4 & transform)
{
	Debug::CheckAssertion(mProgramState != nullptr, "Invaild programState in Sprite::Draw().");

	mQuadCommand.Init(GetGlobalOrder(),mProgramState,
		mTexture != nullptr ? mTexture->GetTextureID(): 0,	// ������Ҫ����������ɫ��
		&mQuad, 1, mBlendFunc,transform,mModelView);

	renderer.PushCommand(&mQuadCommand);
}

/**
*	\brief ���øþ����Ƿ�ɼ�
*/
void Sprite::SetVisible(bool visible)
{
	mVisible = visible;
}

/**
*	\brief ���õ�ǰ����
*
*	ֻ�Ǹ�����ǰtexurePtr,�����Ҫ���»������Ĵ�С
*   ʹ��InitWithTexture
*/
void Sprite::SetTexture(TexturePtr texture)
{
	mTexture = texture;
}

/**
*	\brief �����������
*	\param �������
*	\param �Ƿ���������С,Ĭ�ϲ�����
*/
void Sprite::SetTextureRect(const Rect & rect,bool bResize)
{
	SetTextureCroods(rect);
	if (bResize)
		SetSize(rect.GetSize());
}

/**
*	\brief ������������
*/
void Sprite::SetTextureCroods(const Rect & rect)
{
	Debug::CheckAssertion(mTexture != nullptr, "Set TextureCrooed without texture.");

	float width =  (float)mTexture->GetSize().width;
	float height = (float)mTexture->GetSize().height;
	Point2 orgin = rect.GetPos();
	Size   size = rect.GetSize();

	float left = orgin.x / width;
	float right = (orgin.x + size.width) / width;
	float top = orgin.y / height;
	float bottom = (orgin.y + height) / height;

	mQuad.lt.texs.u = left;
	mQuad.lt.texs.v = bottom;
	mQuad.lb.texs.u = left;
	mQuad.lb.texs.v = top;
	mQuad.rt.texs.u = right;
	mQuad.rt.texs.v = bottom;
	mQuad.rb.texs.u = right;
	mQuad.rb.texs.v = top;
}

/**
*	\brief ���þ����С
*/
void Sprite::SetSize(const Size & size)
{
	mSize = size;
	SetDirty(true);
}

/**
*	\brief ���þ��鶥����ɫ
*
*   ��ʹ���������ɫ��ʱ��ֱ��Ӱ����ʾ����ɫ
*/
void Sprite::SetColor(const Color4B & color)
{
	mQuad.lt.colors = color;
	mQuad.lb.colors = color;
	mQuad.rt.colors = color;
	mQuad.rb.colors = color;
}

/**
*	\brief ����ΪĬ�ϵľ���״̬,������ɫ�����任����
*/
void Sprite::SetDefaultState()
{
	mProgramState =  ResourceCache::GetInstance().GetGLProgramState(GLProgramState::DEFAULT_SPRITE_NORMAL_PROGRAMSTATE_NAME);
	mModelView = Renderer::GetInstance().GetCameraMatrix();
}

/**
*	\brief ����ָ���ľ�����ɫ��״̬
*/
void Sprite::SetProgramState(GLProgramStatePtr programstate)
{
	mProgramState = programstate;
}

/**
*	\brief ���õ�ǰΪ�ྫ�飬����Ⱦǰ���������mquad
*/
void Sprite::SetDirty(bool dirty)
{
	mDirty = dirty;
}

/**
*	\brief ���õ�ǰê��
*/
void Sprite::SetAnchor(const Point2 & anchor)
{
	mAnchor = anchor;
	SetDirty(true);
}

/**
*	\brief ���û��ģʽ
*/
void Sprite::SetBlendFunc(const BlendFunc & blendFunc)
{
	mBlendFunc = blendFunc;
}

void Sprite::SetModelView(const Matrix4 & modelView)
{
	mModelView = modelView;
}

/**
*	\brief �����Ƿ���ͣ����������update����
*/
void Sprite::SetSuspended(bool suspend)
{
	mSuspended = suspend;
}

/**
*	\brief ��������
*/
TexturePtr Sprite::GetTexture() const
{
	return mTexture;
}

/**
*	\brief ���ؾ�����Ч����
*/
Rect Sprite::GetRect() const
{
	return Rect();
}

/**
*	\brief ���ش�С
*/
Size Sprite::GetSize() const
{
	return mSize;
}

/**
*	\brief ���ؾ��飨���㣩��ɫ
*/
Color4B Sprite::GetColor() const
{
	return mQuad.lt.colors;
}

/**
*	\brief ������ɫ��״̬
*/
GLProgramStatePtr Sprite::GetProgramState() const
{
	return mProgramState;
}

/**
*	\brief ����ê��
*/
Point2 Sprite::GetAnchor() const
{
	return mAnchor;
}

/**
*	\brief ���ػ��ģʽ
*/
BlendFunc Sprite::GetBlendFunc() const
{
	return mBlendFunc;
}

/**
*	\brief �Ƿ��ݶ�
*/
bool Sprite::IsSuspended() const
{
	return mSuspended;
}

/**
*	\brief �Ƿ��ྫ�飨�Ƿ���Ҫ����λ�ã�
*/
bool Sprite::IsDirty() const
{
	return mDirty;
}

/**
*	\brief �Ƿ�ɼ�
*/
bool Sprite::IsVisible() const
{
	return mVisible;
}

/**
*	\brief �Ƿ��Ƕ������飬Sprite��Ȼ����false;
*/
bool Sprite::IsAnimationed() const
{
	return false;
}

void Sprite::AddChildSprite(std::shared_ptr<Sprite> childSprite)
{
	mChildSprites.push_back(childSprite);
}

/**
*	\brief ��ͼƬfileName��ʼ��sprite
*/
bool Sprite::InitWithFile(const std::string & name)
{
	auto texturePtr = ResourceCache::GetInstance().LoadTexture2D(name);
	if (texturePtr == nullptr)
	{
		Debug::Error("Invalid texture name.");
		return false;
	}
	Rect rect(texturePtr->GetSize());
	return InitWithTexture(texturePtr,rect);
}

/**
*	\brief ������ָ���ʼ��sprite
*/
bool Sprite::InitWithTexture(TexturePtr texture)
{
	if (texture == nullptr)
		return false;
	Rect rect(texture->GetSize());
	return InitWithTexture(texture,rect);
}

/**
*	\brief ������ָ���ʼ��sprite
*	\param ����ָ��
*	\param ����Χ��rect��ʾ
*/
bool Sprite::InitWithTexture(TexturePtr texture, const Rect & rect)
{
	memset(&mQuad, 0, sizeof(mQuad));
	mQuad.lt.colors = Color4B::WHITE;
	mQuad.lb.colors = Color4B::WHITE;
	mQuad.rt.colors = Color4B::WHITE;
	mQuad.rb.colors = Color4B::WHITE;

	SetSize(rect.GetSize());
	SetTexture(texture);
	SetTextureRect(rect);
	SetDirty(true);

	return true;
}

/**
*	\brief ���µ�ǰmQuad����
*
*   һ������Ϊdirtyʱ�����Զ�����
*/
void Sprite::UpdateTransform()
{
	float x1 = 0 + (float)mAnchor.x;
	float y1 = 0 + (float)mAnchor.y;
	float x2 = x1 + mSize.width;	
	float y2 = y1 + mSize.height;

	mQuad.lb.vertices = Vec3f(x1, y1, .0f);
	mQuad.lt.vertices = Vec3f(x1, y2, .0f);
	mQuad.rb.vertices = Vec3f(x2, y1, .0f);
	mQuad.rt.vertices = Vec3f(x2, y2, .0f);

	SetDirty(false);
}
