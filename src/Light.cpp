#include "Light.h"
#include "RenderableObject.h"
#include "Scene.h"


void Light::SetPosition(glm::vec3 i_position)
{
	position = i_position;
}



void SpotLight::Init()
{
	position = lightMesh->GetPosition();
	
	projectionMatrix = glm::perspective(glm::radians(100.0f), 1024.0f / 768.0f, 30.0f, 70.0f);
	direction = glm::normalize(glm::vec3(0,-10,20) - position);
	viewMatrix = glm::lookAt(position , position + direction, glm::vec3(0, 1, 0));
	
	radius = 10;
	
	SetPosition(position);
	lightMesh->SetTranslation(position);

	Scene* scene = Scene::getScene();
	scene->GetInputHandler()->AddToKeyboardInput(this);

}

void SpotLight::Update()
{
	modelMatrix = lightMesh->GetModelMatrix();

	/*glm::vec3 dir = glm::vec3(0, -10, 20) - position;
	viewMatrix = glm::lookAt(position, position + dir, glm::vec3(0, 1, 0));*/

}

void SpotLight::OnKeyDown(unsigned char key, int x, int y)
{
	if (key == 'l')
	{
		theta += 0.1f;
		if (theta >= 360.0f) theta = 0;
		position.x = radius * cos(theta);
		position.y = radius * sin(theta);

		lightMesh->SetTranslation(position);
	}



}

void SpotLight::OnKeyUp(unsigned char key, int x, int y)
{
}

void SpotLight::SetDirection(glm::vec3 i_direction)
{

	direction = glm::normalize(i_direction);
	viewMatrix = glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));

}
