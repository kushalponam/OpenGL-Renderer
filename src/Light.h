#ifndef _LIGHT_H
#define _LIGHT_H

#include "Core.h"

class RenderableObject;
class Light : public GLObject
{
public:
	virtual ~Light() = default;

	explicit Light(glm::vec3 i_color = glm::vec3(1))
	{
		color = i_color;
	}
	
	void SetIntensity(float i_intensity) { intensity = i_intensity; }
	void SetPosition(glm::vec3 i_position);
	void SetColor(glm::vec3 i_color) { color = i_color; }
	
	float GetIntensity() const { return intensity; }
	glm::vec3 GetDirection() const { return direction; }
	glm::vec3 GetPosition() const { return position; }

	glm::vec3 GetColor() const { return color; }

	glm::mat4 GetViewMatrix() const { return viewMatrix; }
	glm::mat4 GetProjectionMatrix() const { return projectionMatrix; }

	void InitDepthTexture(bool depthComparision, GLsizei width, GLsizei height) { depthTexture.Initialize(depthComparision, width, height); }
	cy::GLRenderDepth2D* GetDepthTexture() { return &depthTexture; }

	void Init(){};
	void Update(){};
	void Render() {};
	void CleanUp(){};

protected:

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 MVP;

	glm::vec3 direction;
	glm::vec3 color;	
	glm::vec3 position;
	
	cy::GLRenderDepth2D depthTexture;
	float intensity;

};


class SpotLight : public Light, KeyboardInput
{
public:
	explicit SpotLight(RenderableObject* mesh = nullptr ,glm::vec3 i_color = glm::vec3(1))
		: Light(i_color), lightMesh(mesh)
	{
	}

	
	void Init();
	void Update();
	void Render(){};
	void CleanUp(){};
	
	void OnKeyDown(unsigned char key, int x, int y);
	void OnKeyUp(unsigned char key, int x, int y);
	void OnSpecialKeyDown(int key, int x, int y){};
	void OnSpecialKeyUp(int key, int x, int y){};

	

private:
	RenderableObject* lightMesh;

	

	float radius;
	float theta;
	float phi;
};

class AreaLight : public Light
{
public:





};




#endif

