#include"app.h"
#include"core\debug.h"
#include"core\logger.h"
#include"core\system.h"
#include"core\fileData.h"
#include"core\video.h"
#include"core\renderer.h"
#include<Windows.h>

//test

App::App():
	mLuaContext(nullptr),
	mExiting(false)
{
	Logger::Info("Cjing start initializing.");
	
	Logger::Info("Open data file");
	string dataPath = ".";
	if (!FileData::OpenData("", dataPath))
		Debug::Die("No data file was found int the direcion:" + dataPath);
	
	// initialize system
	Logger::Info("Initialize system modules");
	System::Initialize();

	// initialize render
	Logger::Info("Initialize render system");

	// initialize lua
	mLuaContext = std::unique_ptr<LuaContext>(new LuaContext(this));
	mLuaContext->Initialize();

	// test
	testSprite = std::make_shared<Sprite>("sprite/test.png");
	testSprite->SetPos(Point2(320, 240));
	testSprite->SetRotated(45);
	//testSprite->SetSize(Size(320, 100));
}

App::~App()
{
	if (mLuaContext != nullptr)
		mLuaContext->Exit();

	FileData::CloseData();
	System::Quit();
}

/**
*	\brief ��ѭ������

*	�ڸú����и��ݹ̶���Ƶ��ѭ��ˢ��
*/
void App::Run()
{
	Logger::Info("Simulation started");

	uint32_t lastFrameDate = System::RealTime();
	uint32_t lag = 0;
	uint32_t timeDropped = 0;

	while (!IsExiting())
	{
		uint32_t now = System::RealTime() - timeDropped;
		uint32_t lastFrameDuration = now - lastFrameDate;
		lastFrameDate = now;
		lag += lastFrameDuration;

		// �п�����ΪһЩ�����������ش��ļ�)������һ֡ʱ��
		// �ر𳤣������׷�أ���ʱ���¼��TimeDropped
		if (lag >= 200)
		{
			timeDropped += lag - System::timeStep;
			lag = System::timeStep;
			lastFrameDate = System::RealTime() - timeDropped;
		}

		// 1.check input
		glfwPollEvents();
		CheckInput();

		// 2.update	�������Ļ�������Ҫѭ����ζ�����һЩ���ƹ���
		int updates = 0;
		while (lag >= System::timeStep && updates < 10 && !IsExiting())
		{
			Update();
			lag -= System::timeStep;
			++updates;
		}

		// 3.render
		if(updates > 0)
			Render();

		// 4.sleep
		lastFrameDuration = (System::RealTime() - timeDropped) - lastFrameDate;
		if (lastFrameDuration < System::timeStep)
			System::Sleeps((System::timeStep - lastFrameDuration));
	}

	Logger::Info("Simulation finished");
}

void App::Update()
{
	// game update

	mLuaContext->Update();
	System::Update();
}

void App::SetExiting(bool t)
{
	mExiting = t;
}

bool App::IsExiting()
{
	return mExiting || Video::IsExiting();
}

/**
*	\brief ��ȡinputEvent
*/
void App::CheckInput()
{
	std::unique_ptr<InputEvent> ent = InputEvent::GetEvent();
	while (ent != nullptr)
	{
		NotifyInput(*ent);
		ent = InputEvent::GetEvent();
	}
}

/**
*	\brief ִ�л��Ʋ���
*/
void App::Render()
{
	Video::CleanCanvas();

	testSprite->Draw();

	Video::Rendercanvas();
}

/**
*	\brief ��ӦinputEvent����event�ַ����ⲿ
*/
void App::NotifyInput(const InputEvent & ent)
{
	if (ent.IsWindowClosing())
		SetExiting(true);
	bool handle = mLuaContext->NotifyInput(ent);
}
