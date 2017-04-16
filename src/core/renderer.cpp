#include"renderer.h"
#include"renderCommand.h"
#include"debug.h"
#include<algorithm>

namespace
{
	bool CompareRenderCommand(RenderCommand* command1, RenderCommand* command2)
	{
		return command1->GetGlobalOrder() < command2->GetGlobalOrder();
	}
	
	const int DEFAULT_RENDER_QUEUE = 0;
	const int QUAD_BATHCHES_RESERVE = 64;
}

/**
*	\brief ��������������RenderCommand
*
*	����command��globalOrder�������������һ����
*/
void RenderQueue::PushCommand(RenderCommand * command)
{
	float globalOrder = command->GetGlobalOrder();
	if (globalOrder < 0.0f)
		mQueueNegZ.push_back(command);
	else if (globalOrder > 0.0f)
		mQueuePosZ.push_back(command);
	else
		mQueue0.push_back(command);
}

size_t RenderQueue::Size() const
{
	return mQueue0.size() + mQueueNegZ.size() + mQueuePosZ.size();
}

/**
*	\brief ����RenderCommand��globalOrder��������
*/
void RenderQueue::Sort()
{
	std::sort(mQueueNegZ.begin(), mQueueNegZ.end(), CompareRenderCommand);
	std::sort(mQueuePosZ.begin(), mQueuePosZ.end(), CompareRenderCommand);
}

void RenderQueue::Clear()
{
	mQueuePosZ.clear();
	mQueue0.clear();
	mQueuePosZ.clear();
}

RenderCommand * RenderQueue::operator[](size_t index)const
{
	if (index < mQueueNegZ.size())
		return mQueueNegZ[index];
	index -= mQueueNegZ.size();
	if (index <  mQueue0.size())
		return mQueue0[index];
	index -= mQueue0.size();
	if(index  < mQueuePosZ.size())
		return mQueuePosZ[index ];
	throw std::range_error("Ivaild: the index in render queue.");
	return nullptr;
}


/**
*	\brief ��ȡΨһʵ��
*/
Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}

Renderer::Renderer() :
	mInitialized(false),
	mVAO(0),
	mVBO(0),
	mQuadsCounts(0)
{
	mRenderGroupsStack.push(DEFAULT_RENDER_QUEUE);
	RenderQueue defaultRenderQeueue;
	mRenderGroups.push_back(defaultRenderQeueue);
	mQuadBatches.reserve(QUAD_BATHCHES_RESERVE);
}

Renderer::~Renderer()
{
	Quit();
}

/**
*	\brief ��ʼ������
*
*   ��ʼ��GLProgram,VAO,VBO
*/
void Renderer::Initialize()
{
	if (mInitialized)
		return;
	InitDefaultProgram();
	InitIndices();
	//InitVAOandVBO();
	mInitialized = true;
}

/**
*	\brief ��ʼ��Ĭ����ɫ������
*/
void Renderer::InitDefaultProgram()
{
}

/**
*	\brief ��ʼ��indices
*/
void Renderer::InitIndices()
{
	for (int i = 0; i < VBO_SIZE; ++i)
	{
		mIndices[i]   = (GLushort)(i * 4 + 0);
		mIndices[i+1] = (GLushort)(i * 4 + 1);
		mIndices[i+2] = (GLushort)(i * 4 + 2);
		mIndices[i+3] = (GLushort)(i * 4 + 2);
		mIndices[i+4] = (GLushort)(i * 4 + 3);
		mIndices[i+5] = (GLushort)(i * 4 + 0);
	}
}

/**
*	\brief ��ʼ��VAO �� VBO
*/
void Renderer::InitVAOandVBO()
{
	Debug::CheckAssertion(mVAO == 0 && mVBO == 0, "The VAO and VBO already assign.");
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindVertexArray(0);
}

/**
*	\brief �ͷź���
*/
void Renderer::Quit()
{
	if (!mInitialized)
		return;
	mRenderGroups.clear();
	mQuadBatches.clear();
	mVAO = mVBO = 0;
	mInitialized = false;
}

/**
*	\brief �����ǰ�ӿ�
*
*	Ĭ��ʹ�ú�ɫ����
*/
void Renderer::RenderClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
*	\brief ִ�л���
*/
void Renderer::Render()
{
	mIsRenderer = true;
	if (IsInitialized())
	{
		for (auto& queue : mRenderGroups)
			queue.Sort();
		VisitRenderQueue(mRenderGroups[0]);
		Flush();
	}
	RenderAfterClean();
	mIsRenderer = false;
}

/**
*	\brief ��commandqueue ִ����ȱ���
*/
void Renderer::VisitRenderQueue(const RenderQueue & queue)
{
	size_t queueSize = queue.Size();
	for (size_t queueIndex = 0; queueIndex < queueSize; ++queueIndex)
	{
		const auto& command = queue[queueIndex];
		switch (command->GetCommandType())
		{
		case RenderCommand::COMMAND_QUAD:

			break;
		case RenderCommand::COMMAND_CUSTOMER:
			break;
		case RenderCommand::COMMAND_BATCH:
			break;
		default:
			Debug::Error("Unknow render command.");
			break;
		}
	}
		
}

/**
*	\brief ��Ⱦ����ɨ����
*/
void Renderer::RenderAfterClean()
{
}

/**
*	\brief ִ������������
*
*	��VisitRenderQueue֮���Ѿ��õ�ȫ����quanCommand������
*	mQuadBathces�У�����DrawQuadBathches��ִ��������Ⱦ����
*/
void Renderer::Flush()
{
	DrawQuadBatches();
}

/**
*	\brief ����mQuadBatches����ִ�������ƴ���    
*/
void Renderer::DrawQuadBatches()
{
	
}

bool Renderer::IsInitialized()
{
	return mInitialized;
}

/**
*	\brief ��RenderCommand��ӵ����ϲ�
*/
void Renderer::PushCommand(RenderCommand * command)
{
	int groupIndex = mRenderGroupsStack.top();
	PushCommand(command, groupIndex);
}

/**
*	\brief ��RenderCommand��ӵ������
*/
void Renderer::PushCommand(RenderCommand * command, int groupIndex)
{
	Debug::CheckAssertion(groupIndex >= 0, "Invaild:render group index.");
	Debug::CheckAssertion(command->GetCommandType() != RenderCommand::COMMAND_UNKNOW, "Invalild: Unknow render command.");
	mRenderGroups[groupIndex].PushCommand(command);
}
