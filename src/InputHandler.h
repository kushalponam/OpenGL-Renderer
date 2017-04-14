#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Core.h"
#include<vector>




class InputHandler
{
public:
	
	void AddToKeyboardInput(KeyboardInput *object);
	void AddToMouseInput(MouseInput *object);

	void UpdateKeyDown(unsigned char key, int x, int y);
	void UpdateKeyUp(unsigned char key, int x, int y);
	void UpdateSpecialKeyDown(int key, int x, int y);
	void UpdateSpecialKeyUp(int key, int x, int y);
	void UpdateMouseButton(int button, int state, int x, int y);
	void UpdateMouseMove(int x, int y);


private:
	std::vector<MouseInput*> mouseInputList;
	std::vector<KeyboardInput*> keyboardInputList;


};






#endif

