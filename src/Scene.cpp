#include "Scene.h"


Scene *Scene::s_Instance;
Scene::~Scene()
{
	Scene::CleanUp();
}

void Scene::Init()
{

	meshLoader = new MeshLoader();

	skybox = new Skybox("city/", 2048, 2048, "cubemap_posx.png", "cubemap_negx.png", "cubemap_posy.png", "cubemap_negy.png",
		"cubemap_posz.png", "cubemap_negz.png");

	skybox->Init();

	irradiance = new Irradiance(8, 32, 32,256,256);
	irradiance->Init();
	
	Material *dragonRoughMat = new Material();
	dragonRoughMat->SetAlbedo(glm::vec3(1.0f, 0.6f, 0));
	dragonRoughMat->SetMetallic(0.1f);
	dragonRoughMat->SetRoughness(0.9f);

	Material *dragonSmoothMat = new Material();
	dragonSmoothMat->SetAlbedo(glm::vec3(1.0f, 0.6f, 0));
	dragonSmoothMat->SetMetallic(0.1f);
	dragonSmoothMat->SetRoughness(0.1f);

	Material *dragonMiddleMat = new Material();
	dragonMiddleMat->SetAlbedo(glm::vec3(1.0f, 0.6f, 0.0f));
	dragonMiddleMat->SetMetallic(0.5f);
	dragonMiddleMat->SetRoughness(0.5f);

	Material *whiteMat = new Material();
	whiteMat->SetAlbedo(glm::vec3(1, 1, 1));
	whiteMat->SetMetallic(0.1f);
	whiteMat->SetRoughness(0.9f);

	Material *redMat = new Material();
	redMat->SetAlbedo(glm::vec3(1.0f, 0.0f, 0.0f));
	redMat->SetMetallic(0.1f);
	redMat->SetRoughness(0.9f);

	Material *blueMat = new Material();
	blueMat->SetAlbedo(glm::vec3(0.0f, 0.0f, 0.8f));
	blueMat->SetMetallic(0.1f);
	blueMat->SetRoughness(0.9f);

	Material *greenMat = new Material();
	greenMat->SetAlbedo(glm::vec3(0.0f, 1.0f, 0.0f));
	greenMat->SetMetallic(0.1f);
	greenMat->SetRoughness(0.9f);

	Material *purple = new Material();
	purple->SetAlbedo(glm::vec3(0.5f, 0.0f, 0.5f));
	purple->SetMetallic(0.1f);
	purple->SetRoughness(0.9f);

	matList.push_back(dragonRoughMat);
	matList.push_back(dragonSmoothMat);
	matList.push_back(dragonMiddleMat);
	matList.push_back(whiteMat);
	matList.push_back(redMat);
	matList.push_back(greenMat);
	matList.push_back(blueMat);
	matList.push_back(purple);

	//RenderableObject *teapot = new RenderableObject("newTeapot.obj");
	RenderableObject *dragon = new RenderableObject("dragon/dragon.obj");
	dragon->SetTranslation(glm::vec3(-20.0f, -5.0f, 20.0f));
	//dragon->SetScale(glm::vec3(0.6f, 0.6f, 0.6f));
	dragon->SetScale(glm::vec3(20, 20, 20));
	dragon->SetMaterial(dragonRoughMat);

	RenderableObject *dragon2 = new RenderableObject("dragon/dragon.obj");
	dragon2->SetTranslation(glm::vec3(20.0f, -5.0f, 20.0f));
	//dragon2->SetScale(glm::vec3(0.6f, 0.6f, 0.6f));
	dragon2->SetScale(glm::vec3(20, 20, 20));
	dragon2->SetMaterial(dragonSmoothMat);

	RenderableObject *dragon3 = new RenderableObject("dragon/dragon.obj");
	dragon3->SetTranslation(glm::vec3(0.0f, -5.0f, 20.0f));
	//dragon3->SetScale(glm::vec3(0.6f, 0.6f, 0.6f));
	dragon3->SetScale(glm::vec3(20, 20, 20));
	dragon3->SetMaterial(dragonMiddleMat);

	//meshLoader->LoadMesh("light/light.obj");
	RenderableObject *lightMesh = new RenderableObject("light/light.obj");
	lightMesh->SetTranslation(glm::vec3(-15.0f, 29.0f, 10.0f));
	lightMesh->SetRotation(glm::radians(270.0f), glm::vec3(1, 0, 0));
	lightMesh->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	lightMesh->SetMaterial(whiteMat);

	RenderableObject *lightMesh1 = new RenderableObject("light/light.obj");
	lightMesh1->SetTranslation(glm::vec3(15.0f, 29.0f, 10.0f));
	lightMesh1->SetRotation(glm::radians(270.0f), glm::vec3(1, 0, 0));
	lightMesh1->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	lightMesh1->SetMaterial(whiteMat);
	

	//Create camera and lights.
	camera = new Camera();
	camera->InitPerspectiveCamera(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 500.0f);
	camera->SetPosition(glm::vec3(0, 0, -20));
	camera->SetViewDirection(glm::vec3(0, 0, 1));
	camera->Update();

	SpotLight* light = new SpotLight(lightMesh);
	light->Init();
	light->SetIntensity(10.0f);
	light->SetColor(glm::vec3(25, 25, 25));
	//light->SetColor(glm::vec3(0, 0, 0));
	AddLight(light);

	SpotLight* light1 = new SpotLight(lightMesh1);
	light1->Init();
	light1->SetIntensity(10.0f);
	light1->SetColor(glm::vec3(25, 25, 25));
	//light1->SetColor(glm::vec3(0, 0, 0));
	AddLight(light1);

	RenderableObject *bottomWall = new RenderableObject(RenderableObject::Quad);
	bottomWall->SetTranslation(glm::vec3(0.0f, -10.0f, 20.0f));
	bottomWall->SetRotation(glm::radians(90.0f), glm::vec3(1, 0, 0));
	bottomWall->SetScale(glm::vec3(30, 20, 1));
	bottomWall->SetMaterial(whiteMat);

	RenderableObject *topWall = new RenderableObject(RenderableObject::Quad);
	topWall->SetTranslation(glm::vec3(0.0f, 30.0f, 20.0f));
	topWall->SetRotation(glm::radians(270.0f), glm::vec3(1, 0, 0));
	topWall->SetScale(glm::vec3(30, 20, 1));
	topWall->SetMaterial(whiteMat);

	RenderableObject *backWall = new RenderableObject(RenderableObject::Quad);
	backWall->SetTranslation(glm::vec3(0.0f, 10.0f, 40.0f));
	backWall->SetScale(glm::vec3(30,20,1));
	backWall->SetMaterial(whiteMat);


	RenderableObject *leftWall = new RenderableObject(RenderableObject::Quad);
	leftWall->SetTranslation(glm::vec3(30.0f, 10.0f, 20.0f));
	leftWall->SetRotation(glm::radians(90.0f), glm::vec3(0, 1, 0));
	leftWall->SetScale(glm::vec3(20, 20, 1));
	leftWall->SetMaterial(redMat);

	RenderableObject *rightWall = new RenderableObject(RenderableObject::Quad);
	rightWall->SetTranslation(glm::vec3(-30.0f, 10.0f, 20.0f));
	rightWall->SetRotation(glm::radians(270.0f), glm::vec3(0, 1, 0));
	rightWall->SetScale(glm::vec3(20, 20, 1));
	rightWall->SetMaterial(greenMat);

	
	DepthShader* depthShader = new DepthShader("src/Shaders/Depth.vert", "src/Shaders/Depth.frag");
	depthShader->Init();
	depthShader->BindObject((Object*)dragon);
	depthShader->BindObject((Object*)dragon2);
	depthShader->BindObject((Object*)dragon3);
	depthShaderList.push_back(depthShader);


	//BlinnShader* blinn = new BlinnShader("src/Shaders/Blinn.vert", "src/Shaders/Blinn_noTexture.frag");
	PBRShader* blinn = new PBRShader("src/Shaders/PBR.vert", "src/Shaders/PBR.frag");
	blinn->Init();
	blinn->BindObject((Object*)dragon);
	blinn->BindObject((Object*)dragon2);
	blinn->BindObject((Object*)dragon3);
	blinn->BindObject((Object*)lightMesh);
	blinn->BindObject((Object*)lightMesh1);
	blinn->BindObject((Object*)bottomWall);
	blinn->BindObject((Object*)backWall);
	blinn->BindObject((Object*)leftWall);
	blinn->BindObject((Object*)rightWall);
	blinn->BindObject((Object*)topWall);
	ShaderList.push_back(blinn);

	BlinnShader* basic = new BlinnShader("src/Shaders/Blinn.vert", "src/Shaders/Blinn_noTexture.frag");
	basic->Init();
	basic->BindObject((Object*)bottomWall);
	basic->BindObject((Object*)leftWall);
	basic->BindObject((Object*)rightWall);
	basic->BindObject((Object*)topWall);
	basic->BindObject((Object*)backWall);
	basic->BindObject((Object*)dragon2);
	envList.push_back(basic);
	
	for(unsigned int i=0;i<lights.size();i++)
	{
		lights[i]->InitDepthTexture(false, WIDTH, HEIGHT);
	}
	
	//Precompute Irradiance for Diffuse and Specular
	irradiance->CaptureEnvironment();

	//reset camera after capturing environment;
	camera->InitPerspectiveCamera(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 500.0f);
	camera->SetPosition(glm::vec3(0, 0, -20));
	camera->SetViewDirection(glm::vec3(0, 0, 1));
	camera->Update();

	irradiance->FilterDiffuse();
	irradiance->FilterSpecular();
	irradiance->FilterBRDF();
}

void Scene::Update()
{
	//update camera

	camera->Update();
	
	for (unsigned int i = 0; i<lights.size(); i++)
	{
		lights[i]->Update();
	}

	for (unsigned int i = 0; i < ShaderList.size(); i++) {
		ShaderList[i]->Update();
	}
}

void Scene::ShadowPass()
{
	

	

}

void Scene::Render()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	for (unsigned int i = 0; i<lights.size(); i++)
	{
		lights[i]->GetDepthTexture()->Bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		for (unsigned int j = 0; j < depthShaderList.size(); j++) {
			static_cast<DepthShader*>(depthShaderList[j])->SetCurrentLight(i);
			depthShaderList[j]->Render();
		}
	}

	glDisable(GL_CULL_FACE);

	lights[0]->GetDepthTexture()->Unbind();
	for (unsigned int i = 0; i<lights.size(); i++)
	{
		lights[i]->GetDepthTexture()->BindTexture(1 + i);
	}
	
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Render PASS START@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox->Render();
	for (unsigned int i = 0; i < lights.size(); i++) {
		int nextIter = i * 4;
		for (unsigned int j = 0; j < ShaderList.size(); j++) {

			ShaderList[j]->SetUniform(8 + nextIter, 1 + i);
		}
	}

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (unsigned int i = 0; i < ShaderList.size(); i++) {

		ShaderList[i]->Render();
	}
	//########################################### Render PASS END #########################################
	
}

void Scene::CleanUp()
{
	//Clean all objects in shaders and shaders itself
	for (unsigned int i = 0; i < ShaderList.size(); i++) {
		Shader *shader = ShaderList[i];
		shader->CleanUp();
		delete shader;
	}
	for (unsigned int i = 0; i < depthShaderList.size(); i++) {
		Shader *shader = depthShaderList[i];
		delete shader;
	}

	for(unsigned int i=0;i<envList.size();i++)
	{
		Shader *shader = envList[i];
		delete shader;
	}

	for(unsigned int i=0; i<matList.size();i++)
	{
		Material * mat = matList[i];
		delete mat;
	}

	//delete camera
	if (camera) {
		delete camera;
	}

	if (inputHandler)
	{
		delete inputHandler;
	}
	
	if(meshLoader)
	{
		delete meshLoader;
	}

	if (TexLibrary::getInstance()) {
		delete TexLibrary::getInstance();
	}

	if (skybox) {
		skybox->CleanUp();
		delete skybox;
	}
	if(irradiance)
	{
		irradiance->CleanUp();
		delete irradiance;
	}
	
	for(unsigned int i=0; i<lights.size();i++)
	{
		Light *l = lights[i];
		l->GetDepthTexture()->Delete();
		l->CleanUp();
		delete l;
	}

	renderBuffer.Delete();
}


Camera * Scene::GetCamera() const
{
	if (camera) {
		return camera;
	}
	printf("Camera not initalized");
	return nullptr;
}

Skybox * Scene::GetSkyBox() const
{
	if (skybox) {
		return skybox;
	}
	return nullptr;
}


InputHandler* Scene::GetInputHandler() 
{
	if(inputHandler == nullptr)
	{
		inputHandler = new InputHandler();
	}
	return inputHandler;
}

std::vector<Light*> Scene::GetLights() const
{
	return lights;
}

void Scene::AddLight(Light* light)
{
	lights.push_back(light);
}

