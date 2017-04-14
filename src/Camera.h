#ifndef _CAMERA_H
#define _CAMERA_H

#include "Core.h"


class Camera : GLUpdateObject, public MouseInput, public KeyboardInput {

public:
	Camera();
	void InitPerspectiveCamera(float i_fov,float i_aspect,float i_zNear,float i_zFar);
	void InitOrthographicCamera(float left, float right, float bottom, float top);

	void SetPosition(glm::vec3 i_position);
	void SetViewDirection(glm::vec3 i_direction);
	void Update();

	void OnMouseMove(int x, int y);
	void OnMouseButton(int button, int state, int x, int y);

    void OnKeyDown(unsigned char key, int x, int y);
	void OnKeyUp(unsigned char key, int x, int y) {};
	void OnSpecialKeyDown(int key, int x, int y) {};
	void OnSpecialKeyUp(int key, int x, int y) {};

	
	glm::mat4 GetProjMatrix();
	glm::mat4 GetViewMatrix();

private:
	
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 upVector;
	
};



#endif
