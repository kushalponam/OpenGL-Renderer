#include "Scene.h"
#include "Shader.h"

DepthShader::DepthShader(std::string i_vertexShaderName, std::string i_fragmentShader) : Shader(i_vertexShaderName, i_fragmentShader)
{
	DepthShader::LoadShaders();
	DepthShader::RegisterUniforms();
}


bool DepthShader::LoadShaders()
{
	bool loaded = program.BuildFiles(vertexShaderName.c_str(), fragmentShaderName.c_str());
	if (!loaded) {
		printf("Could not load Depth Shader");
		return false;
	}
	glUseProgram(program.GetID());
	return loaded;
}

void DepthShader::RegisterUniforms()
{
	program.RegisterUniform(1, "depthMVP");
}

void DepthShader::BindObject(Object * obj)
{
	RenderableObject *rObj = (RenderableObject*)obj;
	if (!obj) {
		printf("Could not bind null object");
		return;
	}
	ObjectList.push_back(rObj);
}

void DepthShader::Update()
{
	
}

void DepthShader::Render()
{
	Bind();
	Scene *scene = Scene::getScene();
	std::vector<Light*> lights = scene->GetLights();
	
	glm::mat4 projectionMatrix = lights[currentLight]->GetProjectionMatrix();
	glm::mat4 viewMatrix = lights[currentLight]->GetViewMatrix();
	//get objects MVP and draw that object using its indices
	for (unsigned int i = 0; i < ObjectList.size(); i++) {

		RenderableObject *obj = ObjectList[i];
		obj->BindVAO();
		obj->Update();
		
		glm::mat4 modelMatrix = obj->GetModelMatrix();
		glm::mat4 mvp = projectionMatrix*viewMatrix* modelMatrix;

		program.SetUniformMatrix4(1, &mvp[0][0]);


		glDrawArrays(GL_TRIANGLES, 0, obj->GetVertexIndicesSize());
	}

}

void DepthShader::CleanUp()
{
	for (unsigned int i = 0; i < ObjectList.size(); i++) {
		RenderableObject *obj = ObjectList[i];
		delete obj;
	}
	program.Delete();
}

void DepthShader::Init()
{
}

