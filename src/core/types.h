#ifndef _TYPES_H_
#define _TYPES_H_

#include"common\common.h"
#include"utils\color.h"
#include"utils\vec3.h"

/******************************************
*	\brief ���������Ⱦ�Ľṹ��
*******************************************/

/**
*	\brief ��������ṹ
*/
struct Tex2
{
	float u, v;
	Tex2() :u(.0f), v(.0f) {}
};

/**
*	\brief ���ڻ��ƵĶ���ṹ
*/
struct V3F_4CB_2TF
{
	Vec3f   vertices;
	Color4B colors;
	Tex2    texs;
};

/**
*	\brief quad
*/
struct Quad
{
	V3F_4CB_2TF lt;
	V3F_4CB_2TF lb;
	V3F_4CB_2TF rb;
	V3F_4CB_2TF rt;
};

/**
*	\brief ���ģʽ�ṹ,�ṹ�����ṩ��src��dst��ɫ�Ļ�Ϸ�����
*	enumֵ
*/
struct BlendFunc
{
	GLenum srcBlend;
	GLenum dstBlend;
	BlendFunc() :srcBlend(GL_SRC_ALPHA), dstBlend(GL_ONE_MINUS_DST_ALPHA) {}
	BlendFunc(GLenum src, GLenum dst) :srcBlend(src), dstBlend(dst) {}
};

constexpr bool operator == (const BlendFunc& lhs, const BlendFunc& rhs)
{
	return lhs.dstBlend == rhs.dstBlend && lhs.srcBlend == rhs.srcBlend;
}

constexpr bool operator != (const BlendFunc& lhs, const BlendFunc& rhs)
{
	return lhs.dstBlend != rhs.dstBlend || lhs.srcBlend != rhs.srcBlend;
}


#endif