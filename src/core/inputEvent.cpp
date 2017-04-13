#include "inputEvent.h"

std::set<InputEvent::KeyboardKey> InputEvent::mKeyPressed;
std::queue<InputEvent::KeyEvent> InputEvent::mEventQueue;

/**
*	\brief ��ȡ��ǰ�������¼�
*
*	�¼���mEventQueue�л�ȡ����checkInput()����
*/
std::unique_ptr<InputEvent> InputEvent::GetEvent()
{
	InputEvent* result = nullptr;
	if (!mEventQueue.empty())
	{
		KeyEvent ent= mEventQueue.front();
		mEventQueue.pop();
		result = new InputEvent(ent);
	}
	return std::unique_ptr<InputEvent>(result);
}

/**
*	\brief ��������ص���������glfw����
*/
void InputEvent::key_callback(GLFWwindow * window, int key_in, int scancode, int action, int mode)
{
	KeyEvent ent;
	if (key_in >= 0 && key_in < 1024)
	{
		KeyboardKey key = static_cast<KeyboardKey>(key_in);
		ent.key = key;
		if (action == GLFW_PRESS)
		{	// ��������ʱ����Ҫ�ж��Ƿ���һֱ��ס��
			ent.repeat = false;
			ent.type = KEYDOWN;
			if (!mKeyPressed.insert(key).second)
				ent.repeat = 1;	// �ظ�����
		}
		else if (action == GLFW_RELEASE)
		{
			ent.repeat = 0;
			ent.type = KEYUP;
			if (mKeyPressed.erase(key) == 0)
				ent.repeat = 1;	// �ظ�����
		}
		else
		{
			ent.type = KEYDOWN;
			ent.repeat = 1;
		}
		mEventQueue.push(ent);
	}
}

InputEvent::InputEvent(const KeyEvent& ent) :
	mKeyEvent(ent)
{
}

/**
*	\brief ��ȡ�������µİ���
*
*	Ŀǰ��֧��keyboard Event�����Ե����øú���ʱ���ض����ڰ���������
*/
InputEvent::KeyboardKey InputEvent::GetKeyBoardKey() const
{
	return mKeyEvent.key;
}

/**
*	\brief �Ƿ��м��̰���������
*/
bool InputEvent::IsKeyBoardPressed() const
{
	return (mKeyEvent.type == KEYDOWN) && (!mKeyEvent.repeat);
}

/**
*	\brief �Ƿ���ָ�����̰���������
*/
bool InputEvent::IsKeyBoardPressed(KeyboardKey key) const
{
	return IsKeyBoardPressed() && (GetKeyBoardKey() == key);
}

/**
*	\brief �Ƿ��м��̰������ͷ�
*/
bool InputEvent::IsKeyBoardReleased() const
{
	return (mKeyEvent.type == KEYUP) && (!mKeyEvent.repeat);
}

/**
*	\brief �Ƿ���ָ�����̰������ͷ�
*/
bool InputEvent::IsKeyBoardReleased(KeyboardKey key) const
{
	return IsKeyBoardReleased() && (GetKeyBoardKey() == key);
}

/**
*	\brief �ж��Ƿ��¹رմ��ڼ�
*
*	�ɸ��ģ�Ĭ�ϵ�����Esc��ʱ������ѭ������
*/
bool InputEvent::IsWindowClosing()const
{
	return (mKeyEvent.key == KEY_ESCAPE);
}