#include "video.h"
#include "debug.h"
#include "inputEvent.h"
#include "utils\Size.h"

namespace
{
	GLFWwindow*  mainWindow = nullptr;
	GLFWmonitor* mainMonitor = nullptr;
	bool fullScreenWindow = false;
	bool visibleCursor = true;
	bool initialized = false;
	Size wantedWindowSize;
	Size positionWindow;
	string tilename;
}

/**
*	\brief ��ʼ��videoģ��
*
*	��ʼ��openGL
*/
void Video::Initialize()
{
	Debug::CheckAssertion(!IsInitialized(), "The video already initialized.");
	wantedWindowSize = {DEFAULT_WINDOW_WIDTH,DEFAULT_WINDOW_HEIGHT};

	// ��ʼ��glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// opengl����ģʽ
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// ��������
	tilename = "CJING v0.1";
	mainMonitor = glfwGetPrimaryMonitor();
	mainWindow = glfwCreateWindow(wantedWindowSize.width, wantedWindowSize.height, tilename.c_str(), fullScreenWindow ? mainMonitor : nullptr, nullptr);
	Debug::CheckAssertion(mainWindow != nullptr, "Cannot create window.");

	// �ƶ����ڵ�����λ��
	positionWindow.width = (int)((glfwGetVideoMode(mainMonitor)->width  - wantedWindowSize.width)  * 0.5f);
	positionWindow.height = (int)((glfwGetVideoMode(mainMonitor)->height - wantedWindowSize.height) * 0.5f);
	glfwSetWindowPos(mainWindow, positionWindow.width, positionWindow.height);
	glfwMakeContextCurrent(mainWindow);

	// ��������ص�����
	Debug::CheckAssertion(!InputEvent::IsInitialized(), "The input must Initialized before video.");
	glfwSetKeyCallback(mainWindow, InputEvent::key_callback);
	if(!visibleCursor)// shadow the cursor
		glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	
	//glfwSetCursorPosCallback(window, InputEvent::mouse_callback);
	//glfwSetMouseButtonCallback(window, InputEvent::mouse_button_callback);

	// ��ʼ��glew
	glewInit();
	glViewport(0, 0, wantedWindowSize.width, wantedWindowSize.height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initialized = true;
}

void Video::Quit()
{
	if (!IsInitialized())
		return;
	if (mainWindow != nullptr)
		glfwTerminate();
}

bool Video::IsInitialized()
{
	return initialized;
}

bool Video::IsExiting()
{
	return glfwWindowShouldClose(mainWindow);
}

void Video::CreateWindow()
{
}

void Video::SetTitleName(const string & tname)
{
	if (!IsInitialized())
		return;
	glfwSetWindowTitle(mainWindow,tname.c_str());
	tilename = tname;
}

string Video::GetTitleName()
{
	return tilename;
}

/**
*	\brief ����Ϊȫ��ģʽ
*/
void Video::SetFullScreen(bool fullscreen)
{
	Debug::CheckAssertion(IsInitialized(), "No Initialized.");
	fullScreenWindow = fullscreen;
	if (fullScreenWindow)
	{
		glfwSetWindowPos(mainWindow, 0, 0);
		glfwSetWindowMonitor(mainWindow, mainMonitor, 0, 0, wantedWindowSize.width, wantedWindowSize.height, glfwGetVideoMode(mainMonitor)->refreshRate);
	}
	else
	{
		glfwSetWindowPos(mainWindow, positionWindow.width, positionWindow.height);
		glfwSetWindowMonitor(mainWindow, nullptr, positionWindow.width, positionWindow.height, wantedWindowSize.width, wantedWindowSize.height, glfwGetVideoMode(mainMonitor)->refreshRate);
	}
}

/**
*	\brief ����Ϊ����ģʽ
*/
void Video::SetWindowScreen()
{
	Debug::CheckAssertion(IsInitialized(), "No Initialized.");
	glfwSetWindowMonitor(mainWindow, nullptr, positionWindow.width, positionWindow.height, wantedWindowSize.width, wantedWindowSize.height, glfwGetVideoMode(mainMonitor)->refreshRate);

}

/**
*	\brief �Ƿ���fullScreen
*/
bool Video::IsFullScreen()
{
	return fullScreenWindow;
}
