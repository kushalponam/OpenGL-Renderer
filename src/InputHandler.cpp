#include "InputHandler.h"

void InputHandler::AddToKeyboardInput(KeyboardInput * object)
{
	if (object) {
		keyboardInputList.push_back(object);
	}
	else
	{
		printf("Could not add MouseInput");
	}
}

void InputHandler::AddToMouseInput(MouseInput * object)
{
	if (object) {
		mouseInputList.push_back(object);
	}else
	{
		printf("Could not add MouseInput");
	}
}

void InputHandler::UpdateKeyDown(unsigned char key, int x, int y)
{
	for(unsigned int i=0; i < keyboardInputList.size(); i++)
	{
		keyboardInputList[i]->OnKeyDown(key, x, y);
	}
}

void InputHandler::UpdateKeyUp(unsigned char key, int x, int y)
{
	for (unsigned int i = 0; i < keyboardInputList.size(); i++)
	{
		keyboardInputList[i]->OnKeyUp(key, x, y);
	}
}

void InputHandler::UpdateSpecialKeyDown(int key, int x, int y)
{
	for (unsigned int i = 0; i < keyboardInputList.size(); i++)
	{
		keyboardInputList[i]->OnSpecialKeyDown(key, x, y);
	}
}

void InputHandler::UpdateSpecialKeyUp(int key, int x, int y)
{
	for (unsigned int i = 0; i < keyboardInputList.size(); i++)
	{
		keyboardInputList[i]->OnSpecialKeyUp(key, x, y);
	}
}

void InputHandler::UpdateMouseButton(int button, int state, int x, int y)
{
	for (unsigned int i = 0; i < mouseInputList.size(); i++)
	{
		mouseInputList[i]->OnMouseButton(button, state, x,y);
	}
}

void InputHandler::UpdateMouseMove(int x, int y)
{
	for (unsigned int i = 0; i < mouseInputList.size(); i++)
	{
		mouseInputList[i]->OnMouseMove(x, y);
	}
}
