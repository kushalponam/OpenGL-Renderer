#include "Scene.h"
#include "Shader.h"


PBRShader::PBRShader(std::string i_vertexShaderName, std::string i_fragmentShader) : Shader(i_vertexShaderName, i_fragmentShader)
{
	PBRShader::LoadShaders();
	PBRShader::RegisterUniforms();
}

bool PBRShader::LoadShaders()
{
	bool loaded = program.BuildFiles(vertexShaderName.c_str(), fragmentShaderName.c_str());
	if (!loaded) {
		printf("Could not load Blinn Shader");
		return false;
	}
	glUseProgram(program.GetID());
	return loaded;
}

void PBRShader::RegisterUniforms()
{
	// in vertex Shader
	program.RegisterUniform(1, "MVP");
	program.RegisterUniform(2, "NormalMatrix");
	program.RegisterUniform(3, "Model");
	program.RegisterUniform(4, "View");

	// Reserved until 28 for 2 more lights
	Scene *scene = Scene::getScene();
	std::vector<Light*> lights = scene->GetLights();
	for (unsigned int i = 0; i < lights.size(); i++) {
		std::string number = std::to_string(i);
		int nextIter = i * 7;
		program.RegisterUniform(5 + nextIter, ("lights[" + number + "].Light_Pos").c_str());
		program.RegisterUniform(6 + nextIter, ("lights[" + number + "].LightMatrix").c_str());
		program.RegisterUniform(7 + nextIter, ("lights[" + number + "].Light_Col").c_str());
		program.RegisterUniform(8 + nextIter, ("ShadowMap[" + number + "]").c_str());
		program.RegisterUniform(9 + nextIter, ("lights[" + number + "].Light_dir").c_str());
		program.RegisterUniform(10 + nextIter, ("lights[" + number + "].cutoff").c_str());
		program.RegisterUniform(11 + nextIter, ("lights[" + number + "].outerCutoff").c_str());
	}

	// in fragment Shader
	program.RegisterUniform(33, "cubeMap");
	program.RegisterUniform(34, "albedo");
	program.RegisterUniform(35, "metallic");
	program.RegisterUniform(36, "roughness");
	program.RegisterUniform(37, "irradianceMap");
	program.RegisterUniform(38, "specularFilter");
	program.RegisterUniform(39, "brdfFilter");
	program.RegisterUniform(40, "albedoTexture");
	program.RegisterUniform(41, "metallicTexture");
	program.RegisterUniform(42, "roughnessTexture");

}

void PBRShader::BindObject(Object * obj)
{
	RenderableObject *rObj = (RenderableObject*)obj;
	if (!obj) {
		printf("Could not bind nulll object");
		return;
	}
	ObjectList.push_back(rObj);
}

void PBRShader::Init()
{
	Scene *scene = Scene::getScene();
	std::vector<Light*> lights = scene->GetLights();

	for (unsigned int i = 0; i < lights.size(); i++) {
		glm::vec3 pos = lights[i]->GetPosition();
		int nextIter = i * 7;
		program.SetUniform(5 + nextIter, pos.x, pos.y, pos.z);

		glm::mat4 lprojectionMatrix = lights[i]->GetProjectionMatrix();
		glm::mat4 lviewMatrix = lights[i]->GetViewMatrix();
		glm::mat4 lMVP = lprojectionMatrix*lviewMatrix;

		program.SetUniformMatrix4(6 + nextIter, &lMVP[0][0]);
		program.SetUniform(7 + nextIter, lights[i]->GetColor().x, lights[i]->GetColor().y, lights[i]->GetColor().z);

		glm::vec3 dir = lights[i]->GetDirection();
		program.SetUniform(9 + nextIter, dir.x, dir.y, dir.z);

		SpotLight* l = (SpotLight*)lights[i];
		program.SetUniform(10 + nextIter, l->GetCutoff());
		program.SetUniform(11 + nextIter, l->GetOuterCutoff());
	}

	program.SetUniform(37, 5);
	program.SetUniform(38, 6);
	program.SetUniform(39, 7);
}


void PBRShader::Update()
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

void PBRShader::Render()
{
	Bind();
	Scene *scene = Scene::getScene();
	TexLibrary *texLib = TexLibrary::getInstance();

	//get objects MVP and draw that object using its indices
	for (unsigned int i = 0; i < ObjectList.size(); i++) {

		RenderableObject *obj = ObjectList[i];
		obj->BindVAO();

		glm::mat4 mvp = obj->GetMVPMatrix();
		glm::mat3 normalMatrix = obj->GetNormalMatrix();
		glm::mat4 modelMatrix = obj->GetModelMatrix();

		glm::vec3 albedo = obj->GetMaterial()->GetAlbedo();
		float mettalic = obj->GetMaterial()->GetMetallic();
		float roughness = obj->GetMaterial()->GetRoughness();

		program.SetUniformMatrix4(1, &mvp[0][0]);
		program.SetUniformMatrix3(2, &normalMatrix[0][0]);
		program.SetUniformMatrix4(3, &modelMatrix[0][0]);
		
		program.SetUniform(34, albedo.x, albedo.y, albedo.z);
		program.SetUniform(35, mettalic);
		program.SetUniform(36, roughness);

		/*scene->GetSkyBox()->ActivateCubeMap();
		program.SetUniform(11, 0);*/

		if (obj->GetMaterial()->HasTexture()) {
			texLib->BindTexture(obj->GetMaterial()->GetAlbedoName());
			texLib->BindTexture(obj->GetMaterial()->GetMetallicName());
			texLib->BindTexture(obj->GetMaterial()->GetRoughnessName());
			program.SetUniform(40, obj->GetMaterial()->GetAlbedoUnit());
			program.SetUniform(41, obj->GetMaterial()->GetMetallicUnit());
			program.SetUniform(42, obj->GetMaterial()->GetRoughnessUnit());
		}else
		{
			glActiveTexture(GL_TEXTURE0 + 10);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0 + 11);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0 + 12);
			glBindTexture(GL_TEXTURE_2D, 0);

		}

		glDrawArrays(GL_TRIANGLES, 0, obj->GetVertexIndicesSize());
	}
}

void PBRShader::CleanUp()
{
	for (unsigned int i = 0; i < ObjectList.size(); i++) {
		RenderableObject *obj = ObjectList[i];
		delete obj;
	}
	program.Delete();
}
