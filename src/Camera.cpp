
#include "Camera.h"
#include "Scene.h"

Camera::Camera()
{
	upVector = glm::vec3(0, 1, 0);
	Scene* scene = Scene::getScene();
	scene->GetInputHandler()->AddToKeyboardInput(this);
	scene->GetInputHandler()->AddToMouseInput(this);

}

void Camera::InitPerspectiveCamera(float i_fov, float i_aspect, float i_zNear, float i_zFar)
{
	projectionMatrix = glm::perspective(i_fov, i_aspect, i_zNear, i_zFar);
}

void Camera::InitOrthographicCamera(float left,float right,float bottom, float top)
{
	projectionMatrix = glm::ortho(left, right, bottom, top);
}

void Camera::SetPosition(glm::vec3 i_position)
{
	position = i_position;
}

void Camera::SetViewDirection(glm::vec3 i_direction)
{
	direction = i_direction;
}

void Camera::Update()
{
	viewMatrix = glm::lookAt(position, position + direction, upVector);
}


void Camera::OnMouseMove(int x, int y)
{
	if (LeftButtonData.buttondown) {
		float tempx = LeftButtonData.position.x;
		tempx -= x;

		direction = glm::mat3(glm::rotate(tempx*0.05f, upVector)) * direction;
		LeftButtonData.position = glm::vec2(x, y);
	}
	else if (RightButtonData.buttondown) {
		float tempy = RightButtonData.position.y;
		tempy -= y;
		glm::vec3 rotateAround = glm::cross(direction, upVector);
		direction = glm::mat3(glm::rotate(tempy*0.05f, rotateAround)) * direction;
		RightButtonData.position = glm::vec2(x, y);
	}
}

void Camera::OnMouseButton(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			LeftButtonData.buttondown = true;
			LeftButtonData.position = glm::vec2(x, y);
			
			RightButtonData.buttondown = false;
			break;
		case GLUT_RIGHT_BUTTON:
			RightButtonData.buttondown = true;
			RightButtonData.position = glm::vec2(x, y);
			
			LeftButtonData.buttondown = false;
			break;
		default:
			break;
	}
}

void Camera::OnKeyDown(unsigned char key, int x, int y)
{
	glm::vec3 strafeDirection = glm::cross(direction, upVector);
	switch (key)
	{
	case 'w':
		position += 0.5f * direction;

		break;
	case 's':
		position -= 0.5f * direction;
		break;
	case 'a':
		
		position += -0.5f * strafeDirection;
		break;
	case 'd':
		
		position += 0.5f * strafeDirection;
		break;
	case 'q':
		position += 0.5f * upVector;
		break;
	case 'e':
		position += -0.5f * upVector;
		break;
	default:
		break;
	}
}

glm::mat4 Camera::GetProjMatrix()
{
	return projectionMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
	
	return viewMatrix;
}

