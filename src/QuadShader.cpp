#include "Scene.h"
#include "Shader.h"


QuadShader::QuadShader(std::string i_vertexShaderName, std::string i_fragmentShader) : Shader(i_vertexShaderName, i_fragmentShader) {
	LoadShaders();
	RegisterUniforms();
}

bool QuadShader::LoadShaders()
{
	bool loaded = program.BuildFiles(vertexShaderName.c_str(), fragmentShaderName.c_str());
	if (!loaded) {
		printf("Could not load Basic Shader");
		return FALSE;
	}
	glUseProgram(program.GetID());
	return loaded;
}

void QuadShader::RegisterUniforms() {

	// in vertex Shader
	program.RegisterUniform(1, "MVP");
	/*program.RegisterUniform(2, "NormalMatrix");
	program.RegisterUniform(3, "Model");
	program.RegisterUniform(4, "View");
	program.RegisterUniform(5, "Light_Pos");
	program.RegisterUniform(10, "kd");*/

	// in fragment Shader
	/*program.RegisterUniform(6, "SpecularStength");
	program.RegisterUniform(7, "LightIntensity");
	program.RegisterUniform(8, "Shininess");
	program.RegisterUniform(9, "AmbientIntensity");*/
	program.RegisterUniform(2, "RTTTexture");

}

void QuadShader::BindObject(Object * obj)
{
	//QuadObject* qObj = (QuadObject*)obj;
	RenderableObject *qObj = (RenderableObject*)obj;
	if (!qObj) {
		printf("Could not bind nulll object");
		return;
	}
	ObjectList.push_back(qObj);
}

void QuadShader::Init()
{
	/*Scene *scene = Scene::getScene();
	glm::vec3 lightPos = scene->GetSpotLight()->GetPosition();

	program.SetUniform(5, lightPos.x, lightPos.y, lightPos.z);

	float SpecularStength(0.7f);
	program.SetUniform(6, SpecularStength);

	float intensity = scene->GetSpotLight()->GetIntensity();
	program.SetUniform(7, intensity);

	float Shinines = 20;
	program.SetUniform(8, Shinines);

	float ambient = 0.2f;
	program.SetUniform(9, ambient);

	glm::vec3 diffCol = glm::vec3(0.5f, 0.5f, 0.5f);
	program.SetUniform(10, diffCol.r, diffCol.g, diffCol.b);*/
	program.SetUniform(2, 7);
}


void QuadShader::Update()
{
	program.Bind();
	//Get matrices from scene camera
	Scene *scene = Scene::getScene();
	Camera* camera = scene->GetCamera();

	glm::mat4 projectionMatrix = camera->GetProjMatrix();
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	/*glm::mat4 projectionMatrix = scene->GetSpotLight()->GetProjectionMatrix();
	glm::mat4 viewMatrix = scene->GetSpotLight()->GetViewMatrix();*/

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
		//obj->SetNormalMatrix(normalMatrix);
	}
	//program.SetUniformMatrix4(4, &viewMatrix[0][0]);

}

void QuadShader::Render()
{
	program.Bind();
	Scene *scene = Scene::getScene();
	//get objects MVP and draw that object using its indices
	for (unsigned int i = 0; i < ObjectList.size(); i++) {

		RenderableObject *obj = ObjectList[i];
		obj->BindVAO();

		glm::mat4 mvp = obj->GetMVPMatrix();
		glm::mat3 normalMatrix = obj->GetNormalMatrix();
		glm::mat4 modelMatrix = obj->GetModelMatrix();

		program.SetUniformMatrix4(1, &mvp[0][0]);
		/*program.SetUniformMatrix3(2, &normalMatrix[0][0]);
		program.SetUniformMatrix4(3, &modelMatrix[0][0]);*/

	
		glDrawArrays(GL_TRIANGLES, 0, obj->GetVertexIndicesSize());
	}
}

void QuadShader::CleanUp()
{
	for (unsigned int i = 0; i < ObjectList.size(); i++) {
		RenderableObject *obj = ObjectList[i];
		delete obj;
	}
	program.Delete();
}

