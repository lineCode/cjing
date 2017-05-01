#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"common\common.h"
#include"core\types.h"
#include"utils\matrix4.h"

#include<stack>

class RenderCommand;
class QuadCommand;
class CustomerCommand;
/**
*	\brief ��Ⱦ���У����ڴ��RenderCommand
*/
class RenderQueue
{
public:
	void PushCommand(RenderCommand* command);
	size_t Size()const;
	void Sort();
	void Clear();
	RenderCommand* operator[](size_t index)const;

private:
	vector<RenderCommand*>mQueueNegZ;
	vector<RenderCommand*>mQueue0;
	vector<RenderCommand*>mQueuePosZ;
};


/**
*	\brief ��Ⱦģ�飬Renderer�����ʼ����ɫ����ͬʱִ��ÿ����ѭ���ύ��
*	��Ⱦ����
*/
class Renderer
{
public:
	// system method
	static Renderer& GetInstance();

	Renderer(const Renderer&other) = delete;
	Renderer&operator=(const Renderer& other) = delete;

	// system
	void Initialize(int w, int h);
	void Quit();
	bool IsInitialized();
	void PushCommand(RenderCommand* command);
	void PushCommand(RenderCommand* command, int groupIndex);

	// render
	void RenderClear();
	void Render();
	void RenderAfterClean();

	// data
	Matrix4 GetCameraMatrix()const;
	void SetViewSize(int w, int h);

private:
	Renderer();
	~Renderer();

	void InitCamearMatrix();
	void InitDefaultProgram();
	void InitIndices();
	void InitVAOandVBO();
	void VisitRenderQueue(const RenderQueue& queue);
	void Flush();
	void DrawQuadBatches();
	void TransformQuadsToWorld(Quad* mQuads, int quadCount,const Matrix4 transform);

	std::vector<RenderQueue> mRenderGroups;
	std::stack<int> mRenderGroupsStack;

	bool mInitialized;
	bool mIsRenderer;

	GLuint mVAO;
	GLuint mVBO;
	GLuint mVEO;

	// render data
	static const uint32_t VBO_SIZE = 65536;
	Quad mQuads[VBO_SIZE];
	GLushort mIndices[VBO_SIZE * 6];
	int mQuadsCounts;
	std::vector<QuadCommand*>mQuadBatches;
	int mViewWidth, mViewHeight;
	Matrix4 mCamearMatrix;			// ȫ��ͳһ������任����

};

#endif