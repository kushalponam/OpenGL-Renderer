
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "Scene.h"
#include <crtdbg.h>



Scene *scene;
void Init(int argc,char **argv) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	scene = Scene::getScene();
	scene->Init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)*/;
}


void Update() {
	scene->Update();
	glutPostRedisplay();
}


void Render() {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->Render();
	glutSwapBuffers();
}



void OnKeyDown(unsigned char key, int x, int y) {
	
	scene->GetInputHandler()->UpdateKeyDown(key, x, y);

	switch (key)
	{
	case 033:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}
void OnKeyUp(unsigned char key, int x, int y) {
	scene->GetInputHandler()->UpdateKeyUp(key, x, y);
}

void OnSpecialKeyDown(int key, int x, int y) {
	scene->GetInputHandler()->UpdateSpecialKeyDown(key, x, y);
}
void OnSpecialKeyUp(int key, int x, int y) {
	scene->GetInputHandler()->UpdateKeyUp(key, x, y);
}

void OnMouseButton(int button, int state, int x, int y) {
	
	scene->GetInputHandler()->UpdateMouseButton(button, state, x, y);

}

void OnMouseMove(int x, int y) {

	scene->GetInputHandler()->UpdateMouseMove(x, y);
}



void CleanUp() {
	printf("Cleaning up");
	if (scene) {
		delete scene;
		scene = nullptr;
	}
}


int main(int argc, char **argv) {
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(XPOS, YPOS);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutCreateWindow("CS6610");
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(err));
		return 1;
	}

	Init(argc,argv);
	
	
	glutDisplayFunc(Render);
	glutIdleFunc(Update);

	glutKeyboardFunc(OnKeyDown);
	glutKeyboardUpFunc(OnKeyUp);
	glutSpecialFunc(OnSpecialKeyDown);
	glutSpecialUpFunc(OnSpecialKeyUp);
	glutMouseFunc(OnMouseButton);
	glutMotionFunc(OnMouseMove);
	glutMainLoop();
	
	CleanUp();
	
	_CrtDumpMemoryLeaks();
	
	return 0;

}
