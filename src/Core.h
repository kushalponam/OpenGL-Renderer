#ifndef _CORE_H
#define _CORE_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <GL\glew.h>
#include <GL\freeglut.h>
#include "cyCodebase\cyGL.h"
#include "cyCodebase\cyTriMesh.h"
#include<glm/ext.hpp>
#include <typeinfo>

#define INPUT_OBJ_FOLDER "Input/ObjectFiles/"
#define INPUT_TEX_FOLDER "Input/Textures/"
#define INPUT_CUBEMAP_FOLDER "Input/cubemaps/"


class Object {};

class GLObject {
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void CleanUp() = 0;
};

class GLUpdateObject {
	virtual void Update() = 0;
};

class MouseInput {
public:
	struct MouseButtonData {
		glm::vec2 position;
		bool buttondown;
	};
	MouseButtonData LeftButtonData;
	MouseButtonData RightButtonData;
	virtual void OnMouseMove(int x, int y) = 0;
	virtual void OnMouseButton(int button, int state, int x, int y) = 0;
};

class KeyboardInput {
public:

	virtual void OnKeyDown(unsigned char key, int x, int y)=0;
	virtual void OnKeyUp(unsigned char key, int x, int y) = 0;
	virtual void OnSpecialKeyDown(int key, int x, int y) = 0;
	virtual void OnSpecialKeyUp(int key, int x, int y) = 0;
};


inline float LERP(float v0, float v1, float t)
{
	return (1 - t) * v0 + t * v1;
}

#endif //_CORE_H
