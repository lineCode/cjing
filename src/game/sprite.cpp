#include"sprite.h"
#include"core\texture.h"
#include"core\debug.h"
#include"core\resourceCache.h"
#include"lua\luaContext.h"

/**
*	\brief 创建一个默认的精灵
*/
Sprite::Sprite():
	mProgramState(nullptr),
	mTexture(nullptr),
	mVisible(true),
	mDirty(true),
	mAnchor(0, 0),
	mBlendFunc(),
	mSuspended(false)
{
	SetDefaultNormalState();
}

/**
*	\brief 创建一个基于纹理的精灵实例
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
	SetDefaultNormalState();
}

/**
*	\brief 创建一个基于纹理的精灵实例
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
	SetDefaultNormalState();
}

/**
*	\brief 创建一个纯色不使用纹理的精灵实例
*	\param 精灵的颜色
*	\param 精灵的大小
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
	SetDefaultColorProgramState();
}

Sprite::~Sprite()
{
	mChildSprites.clear();
}

/**
*	\brief 更新自身状态同时更新所有children
*/
void Sprite::Update()
{
	Drawable::Update();

	if (mSuspended)
		return;

	for (auto& sprite : mChildSprites)
		sprite->Update();
}

/**
*	\brief 绘制精灵
*
*   以精灵保存的位置和旋转角度绘制精灵
*/
void Sprite::Draw()
{
	if (IsDirty())
		UpdateTransform();

	if(mVisible)
		Draw(GetPos(), GetRotated());
}

/**
*	\brief 在指定位置和指定角度绘制精灵
*
*   根据位置和角度生成对应的变换矩阵
*/
void Sprite::Draw(const Point2 & pos, float rotate)
{
	Matrix4 transfomr;
	// 因为是右乘坐标，所有操作顺序为从下往上
	// 移动到指定位置
	transfomr =  Matrix4::Translate(Vec3f((float)pos.x,(float)pos.y, .0f));

	// 移动到原点旋转后移回
	transfomr *= Matrix4::Translate(Vec3f(mSize.width*0.5f, mSize.height*0.5f, 0.0f));
	transfomr *= Matrix4::Rotate(Vec3f(0.0f, 0.0f, 1.0f), rotate);
	transfomr *= Matrix4::Translate(Vec3f(-mSize.width*0.5f, -mSize.height*0.5f, 0.0f));

	Draw(Renderer::GetInstance(), transfomr);
}

/**
*	\brief 向renderer提交渲染命令
*/
void Sprite::Draw(Renderer & renderer, const Matrix4 & transform)
{
	Debug::CheckAssertion(mProgramState != nullptr, "Invaild programState in Sprite::Draw().");

	auto quadCommand = new QuadCommand();

	quadCommand->Init(GetGlobalOrder(),mProgramState,
		mTexture != nullptr ? mTexture->GetTextureID(): 0,	// 这里需要考虑无纹理色块
		mQuad, 1, mBlendFunc,transform,mModelView);

	renderer.PushCommand(quadCommand);
}

/**
*	\brief 设置该精灵是否可见
*/
void Sprite::SetVisible(bool visible)
{
	mVisible = visible;
}

/**
*	\brief 设置当前纹理
*
*	只是更换当前texurePtr,如果需要重新获得纹理的大小
*   使用InitWithTexture
*/
void Sprite::SetTexture(TexturePtr texture)
{
	mTexture = texture;
}

/**
*	\brief 设置纹理矩形
*	\param 纹理矩形
*	\param 是否调整精灵大小,默认不调整
*/
void Sprite::SetTextureRect(const Rect & rect,bool bResize)
{
	SetTextureCroods(rect);
	if (bResize)
		SetSize(rect.GetSize());
}

/**
*	\brief 设置纹理坐标
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
	float bottom = (orgin.y + size.height) / height;

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
*	\brief 设置精灵大小
*/
void Sprite::SetSize(const Size & size)
{
	mSize = size;
	SetDirty(true);
}

/**
*	\brief 设置精灵顶点颜色
*
*   当使用无纹理的色块时，直接影响显示的颜色
*/
void Sprite::SetColor(const Color4B & color)
{
	mQuad.lt.colors = color;
	mQuad.lb.colors = color;
	mQuad.rt.colors = color;
	mQuad.rb.colors = color;
}

/**
*	\brief 设置为默认的精灵正常着色器状态,包括着色器，变换矩阵
*/
void Sprite::SetDefaultNormalState()
{
	mProgramState =  ResourceCache::GetInstance().GetGLProgramState(GLProgramState::DEFAULT_SPRITE_NORMAL_PROGRAMSTATE_NAME);
	mModelView = Renderer::GetInstance().GetCameraMatrix();
}


/**
*	\brief 设置为默认的精灵仅包含颜色着色器状态,包括着色器，变换矩阵
*/
void Sprite::SetDefaultColorProgramState()
{
	mProgramState = ResourceCache::GetInstance().GetGLProgramState(GLProgramState::DEFAULT_SPRITE_COLOR_PROGRAMSTATE_NAME);
	mModelView = Renderer::GetInstance().GetCameraMatrix();
}

/**
*	\brief 设置指定的精灵着色器状态
*/
void Sprite::SetProgramState(GLProgramStatePtr programstate)
{
	mProgramState = programstate;
}

/**
*	\brief 设置当前为脏精灵，在渲染前会重新填充mquad
*/
void Sprite::SetDirty(bool dirty)
{
	mDirty = dirty;
}

/**
*	\brief 设置当前锚点
*/
void Sprite::SetAnchor(const Point2 & anchor)
{
	mAnchor = anchor;
	SetDirty(true);
}

/**
*	\brief 设置混合模式
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
*	\brief 设置透明度(alpha)
*/
void Sprite::SetOpacity(int opacity)
{
	Debug::CheckAssertion(opacity >= 0 && opacity <= 255,
		"Invalid alpha value.");

	mQuad.lt.colors.SetAlpha(opacity);
	mQuad.lb.colors.SetAlpha(opacity);
	mQuad.rb.colors.SetAlpha(opacity);
	mQuad.rt.colors.SetAlpha(opacity);
}

void Sprite::SetScale(float s)
{
	SetScale(s, s);
}

void Sprite::SetScaleX(float x)
{
	SetScale(x, scaleY);
}

void Sprite::SetScaleY(float y)
{
	SetScale(scaleX, y);
}

void Sprite::SetScale(float x, float y)
{
	scaleX = x;
	scaleY = y;
}

/**
*	\brief 返回纹理
*/
TexturePtr Sprite::GetTexture() const
{
	return mTexture;
}

/**
*	\brief 返回矩形有效区域
*/
Rect Sprite::GetRect() const
{
	return Rect();
}

/**
*	\brief 返回大小
*/
Size Sprite::GetSize() const
{
	return mSize;
}

/**
*	\brief 返回精灵（顶点）颜色
*/
Color4B Sprite::GetColor() const
{
	return mQuad.lt.colors;
}

/**
*	\brief 返回着色器状态
*/
GLProgramStatePtr Sprite::GetProgramState() const
{
	return mProgramState;
}

/**
*	\brief 返回锚点
*/
Point2 Sprite::GetAnchor() const
{
	return mAnchor;
}

/**
*	\brief 返回混合模式
*/
BlendFunc Sprite::GetBlendFunc() const
{
	return mBlendFunc;
}

/**
*	\brief 返回透明度(alpha)
*/
int Sprite::GetOpacity() const
{
	return mQuad.lt.colors.GetAlpha();
}

void Sprite::SetSuspended(bool suspended)
{
	if (suspended != IsSuspended() )
	{
		Drawable::SetSuspended(suspended);
	}
}

/**
*	\brief 是否脏精灵（是否需要更新位置）
*/
bool Sprite::IsDirty() const
{
	return mDirty;
}

/**
*	\brief 是否可见
*/
bool Sprite::IsVisible() const
{
	return mVisible;
}

/**
*	\brief 是否是动画精灵，Sprite必然返回false;
*/
bool Sprite::IsAnimationed() const
{
	return false;
}

void Sprite::AddChildSprite(std::shared_ptr<Sprite> childSprite)
{
	mChildSprites.push_back(childSprite);
}

const string Sprite::GetLuaObjectName() const
{
	return LuaContext::module_sprite_name;
}

/**
*	\brief 以图片fileName初始化sprite
*/
bool Sprite::InitWithFile(const std::string & name)
{
	auto texturePtr = ResourceCache::GetInstance().LoadTexture2D(name);
	if (texturePtr == nullptr)
	{
		Debug::Error("Invalid texture name in sprite.");
		return false;
	}
	Rect rect(texturePtr->GetSize());
	return InitWithTexture(texturePtr,rect);
}

/**
*	\brief 以纹理指针初始化sprite
*/
bool Sprite::InitWithTexture(TexturePtr texture)
{
	if (texture == nullptr)
		return false;
	Rect rect(texture->GetSize());
	return InitWithTexture(texture,rect);
}

/**
*	\brief 以纹理指针初始化sprite
*	\param 纹理指针
*	\param 纹理范围的rect表示
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
*	\brief 更新当前mQuad数据
*
*   一般设置为dirty时，会自动调用
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
