#include "Scene.h"
#include "Shader.h"

Shader::Shader(std::string i_vertexShaderName, std::string i_fragmentShaderName)
{
	vertexShaderName = i_vertexShaderName;
	fragmentShaderName = i_fragmentShaderName;
	
}


void Shader::ReflectObjects(int direction)
{
	for (unsigned int i = 0; i < ObjectList.size(); i++) {
		ObjectList[i]->ReflectObject(direction);
	}
}


BlinnShader::BlinnShader(std::string i_vertexShaderName, std::string i_fragmentShader) : Shader(i_vertexShaderName, i_fragmentShader)
{
	BlinnShader::LoadShaders();
	BlinnShader::RegisterUniforms();
}

bool BlinnShader::LoadShaders()
{
	bool loaded = program.BuildFiles(vertexShaderName.c_str(), fragmentShaderName.c_str());
	if (!loaded) {
		printf("Could not load Blinn Shader");
		return false;
	}
	glUseProgram(program.GetID());
	return loaded;
}

void BlinnShader::RegisterUniforms()
{
	// in vertex Shader
	program.RegisterUniform(1, "MVP");
	program.RegisterUniform(2, "NormalMatrix");
	program.RegisterUniform(3, "Model");
	program.RegisterUniform(4, "View");
	
	// Reserved until 13 for 2 more lights
	Scene *scene = Scene::getScene();
	std::vector<Light*> lights = scene->GetLights();
	for (unsigned int i = 0; i < lights.size(); i++) {
		std::string number = std::to_string(i);
		int nextIter = i * 4;
		program.RegisterUniform(5 + nextIter, ("lights["+number+"].Light_Pos").c_str());
		program.RegisterUniform(6 + nextIter, ("lights["+number+"].LightMatrix").c_str());
		program.RegisterUniform(7 + nextIter, ("lights["+number+"].Intensity").c_str());
		program.RegisterUniform(8 + nextIter, ("ShadowMap["+number+"]").c_str());
	}

	program.RegisterUniform(17, "kd");
	
	// in fragment Shader
	program.RegisterUniform(18, "SpecularStength");
	program.RegisterUniform(19, "Shininess");
	program.RegisterUniform(20, "AmbientIntensity");
	program.RegisterUniform(21, "cubeMap");
	
}

void BlinnShader::BindObject(Object * obj)
{
	RenderableObject *rObj = (RenderableObject*)obj;
	if (!obj) {
		printf("Could not bind nulll object");
		return;
	}
	ObjectList.push_back(rObj);
}

void BlinnShader::Init()
{
	Scene *scene = Scene::getScene();
	std::vector<Light*> lights = scene->GetLights();

	for (unsigned int i = 0; i < lights.size(); i++) {
		glm::vec3 pos = lights[i]->GetPosition();
		int nextIter = i * 4;
		program.SetUniform(5 + nextIter, pos.x, pos.y, pos.z);
	
		program.SetUniform(7 + nextIter, lights[i]->GetIntensity());
		
		glm::mat4 lprojectionMatrix = lights[i]->GetProjectionMatrix();
		glm::mat4 lviewMatrix = lights[i]->GetViewMatrix();
		glm::mat4 lMVP = lprojectionMatrix*lviewMatrix;

		program.SetUniformMatrix4(6 + nextIter, &lMVP[0][0]);
	}

	float SpecularStength(0.9f);
	program.SetUniform(18, SpecularStength);

	float Shinines = 20;
	program.SetUniform(19, Shinines);

	float ambient = 0.2f;
	program.SetUniform(20, ambient);

}

void BlinnShader::Update()
{
	Bind();
	//Get matrices from scene camera
	Scene *scene = Scene::getScene();
	Camera* camera = scene->GetCamera();

	glm::mat4 projectionMatrix = camera->GetProjMatrix();
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	
	
	// for each object update the shaders with required matrices
	for (unsigned int i = 0; i < ObjectList.size(); i++) {
		//Objects will update their model matrix
		RenderableObject *obj = ObjectList[i];
		obj->BindVAO();
		obj->Update();

		glm::mat4 modelMatrix = obj->GetModelMatrix();
		glm::mat4 mvp = projectionMatrix*viewMatrix* modelMatrix;

		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix) * glm::mat3(modelMatrix)));

		obj->SetMVPMatrix(mvp);
		obj->SetNormalMatrix(normalMatrix);
	
	}
	program.SetUniformMatrix4(4, &viewMatrix[0][0]);

}




void BlinnShader::Render() {
	
	Bind();
	Scene *scene = Scene::getScene();

	//get objects MVP and draw that object using its indices
	for (unsigned int i = 0; i < ObjectList.size(); i++) {

		RenderableObject *obj = ObjectList[i];
		obj->BindVAO();

		glm::mat4 mvp = obj->GetMVPMatrix();
		glm::mat3 normalMatrix = obj->GetNormalMatrix();
		glm::mat4 modelMatrix = obj->GetModelMatrix();
			
		program.SetUniformMatrix4(1, &mvp[0][0]);
		program.SetUniformMatrix3(2, &normalMatrix[0][0]);
		program.SetUniformMatrix4(3, &modelMatrix[0][0]);
	
		/*scene->GetSkyBox()->ActivateCubeMap();
		program.SetUniform(11, 0);*/

		if (obj->GetMaterial()->HasTexture()) {
		
			std::string diffuseTexName = obj->GetMaterial()->GetDiffuseTexture()->GetName();
			int texID1 = TexLibrary::getInstance()->GetTextureID(diffuseTexName);
			obj->GetMaterial()->ActivateDiffuseTexture();
			program.SetUniform(10, texID1);
			

			std::string specularTexName = obj->GetMaterial()->GetSpecularTexture()->GetName();
			int texID2 = TexLibrary::getInstance()->GetTextureID(specularTexName);
			obj->GetMaterial()->ActivateSpecularTexture();
			program.SetUniform(11, texID2);
			
		}
	
		glm::vec3 diffCol = obj->GetMaterial()->GetAlbedo();
		program.SetUniform(17, diffCol.r, diffCol.g, diffCol.b);

	
		glDrawArrays(GL_TRIANGLES, 0, obj->GetVertexIndicesSize());
	}
}

void BlinnShader::CleanUp()
{
	for (unsigned int i = 0; i < ObjectList.size(); i++) {
		RenderableObject *obj = ObjectList[i];
		delete obj;
	}
	program.Delete();
}


