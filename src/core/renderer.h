#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"common\common.h"
#include"core\types.h"
#include<stack>

class RenderCommand;

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

	void Initialize();
	void Quit();

	void RenderClear();
	void Render();
	void RenderAfterClean();

	bool IsInitialized();
	void PushCommand(RenderCommand* command);
	void PushCommand(RenderCommand* command,int groupIndex);

private:
	Renderer();
	~Renderer();

	void InitDefaultProgram();
	void InitIndices();
	void InitVAOandVBO();
	void VisitRenderQueue(const RenderQueue& queue);
	void Flush();
	void DrawQuadBatches();

	std::vector<RenderQueue> mRenderGroups;
	std::stack<int> mRenderGroupsStack;

	bool mInitialized;
	bool mIsRenderer;

	GLuint mVAO;
	GLuint mVBO;
	GLushort mVEO;

	static const uint32_t VBO_SIZE = 65536;
	GLushort mIndices[VBO_SIZE * 6];
	int mQuadsCounts;
	std::vector<RenderCommand*>mQuadBatches;

};

#endif