#ifndef _QUAD_COMMAND_H_
#define _QUAD_COMMAND_H_

#include"core\renderCommand.h"
#include"core\types.h"

/**
*	\brief ����quad����Ⱦ����
*/
class QuadCommand : public RenderCommand
{
public:
	QuadCommand();
	virtual ~QuadCommand();

	void Init(float globalOrder,GLuint textureID,Quad* quads,int quadCounts,const BlendFunc&blendFunc);
	void UseShade();

	int GetQuadCounts()const;
	const Quad* GetQuads()const;
	uint32_t GetShadeState()const;
	BlendFunc GetBlendFunc()const;

private:
	void GenerateShadeState();

	GLuint mTextureID;
	Quad* mQuads;
	int   mQuadCounts;
	BlendFunc mBlendFunc;
	uint32_t mShadeState;

};

#endif