#ifndef _SCENE_H
#define _SCENE_H

#include "Core.h"
#include "Shader.h"
#include "RenderableObject.h"
#include "Camera.h"
#include "Skybox.h"
#include "Light.h"
#include "InputHandler.h"
#include "MeshLoader.h"
#include "IrradianceMap.h"

#include <map>

#define WIDTH 1360
#define HEIGHT 768
#define XPOS 100
#define YPOS 100
#define PI 3.14159f

class Scene : GLObject {

public:
	static Scene* getScene() {
		if (s_Instance == nullptr) {
			s_Instance = new Scene();
		}
		return s_Instance;
	}	
	~Scene();
	void Init();
	void Update();
	void Render();
	void CleanUp();
	void ShadowPass();
	Camera* GetCamera() const;
	
	Skybox* GetSkyBox() const;

	InputHandler* GetInputHandler();

	MeshLoader* GetMeshLoader() const { return meshLoader; }
	std::vector<Light*> GetLights() const;
	std::vector<Shader*> GetEnvironmentList()const { return envList; }

	void AddLight(Light* light);
	//QuadObject* GetQuadObject() { return quadObject; }
private:
	Scene() {
		printf("SceneInitialized\n");
	}
	static Scene *s_Instance;
	
	Camera *camera;
	InputHandler *inputHandler;

	std::vector<Shader*> ShaderList;
	std::vector<Shader*> depthShaderList;
	std::vector<Shader*> envList;
	std::vector<Light*> lights;
	std::vector<Material*> matList;

	Skybox *skybox;
	MeshLoader *meshLoader;
	Irradiance *irradiance;
	
	//TODO: Temporary Use only. Move this to some other class 
	cy::GLRenderTexture<GL_TEXTURE_RECTANGLE> renderBuffer;
	
};










#endif
