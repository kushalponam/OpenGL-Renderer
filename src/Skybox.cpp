#include "Skybox.h"
#include "LoadPNG.h"
#include "Scene.h"


Skybox::Skybox(std::string folderPath, unsigned int imageWidth, unsigned int imageHeight, std::string i_rightFaceName, std::string i_leftFaceName, std::string i_topFaceName,
	std::string i_bottonFaceName, std::string i_backFaceName, std::string i_frontFaceName)
{
	textureNames[0] = i_rightFaceName;
	textureNames[1] = i_leftFaceName;
	textureNames[2] = i_topFaceName;
	textureNames[3] = i_bottonFaceName;
	textureNames[4] = i_backFaceName;
	textureNames[5] = i_frontFaceName;

	std::string path = INPUT_CUBEMAP_FOLDER + folderPath;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenTextures(1, &skyboxTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	for (int i = 0; i < 6; i++) {
		std::vector<unsigned char> image;
		unsigned error = lodepng::decode(image, imageWidth, imageHeight, path + textureNames[i]);
		if (error) {
			printf("could not load %s", textureNames[i].c_str());
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


}

void Skybox::Init()
{

	Scene* scene = Scene::getScene();
	cubeMesh = scene->GetMeshLoader()->GetMesh("cube.obj");

	if (!cubeMesh) {
		printf("Could not load skybox mesh");
	}

	for (unsigned int i = 0; i < cubeMesh->NF(); i++) {
		for (int j = 0; j < 3; j++) {
			int indice = cubeMesh->F(i).v[j];
			cy::Point3f vert = cubeMesh->V(indice);
			index_vertices.push_back(vert);
		}
	}

	scaleMatrix = glm::mat4(1);

	bool ls = program.BuildFiles("src/Shaders/skyboxVertexShader.vert", "src/Shaders/skyboxFragShader.frag");
	if (!ls) {
		printf("Could not load skybox shaders");
	}
	
	program.RegisterUniform(1, "projectionMatrix");
	program.RegisterUniform(2, "cubeMap");
	program.RegisterUniform(3, "viewMatrix");
	program.Bind();

	program.SetUniform(2, 8);

	glGenBuffers(1, &cubeMeshBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMeshBuffer);
	glNamedBufferStorage(cubeMeshBuffer, index_vertices.size() * sizeof(cy::Point3f), &index_vertices[0], 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}


void Skybox::Update()
{
	
	
}

void Skybox::Render()
{
	glBindVertexArray(VAO);
	glDepthMask(GL_FALSE);
	program.Bind();

	Scene *scene = Scene::getScene();
	Camera* camera = scene->GetCamera();

	glm::mat4 projMatrix = camera->GetProjMatrix();
	glm::mat4 viewMatrix = camera->GetViewMatrix();

	viewMatrix[3][0] = 0.0f;
	viewMatrix[3][1] = 0.0f;
	viewMatrix[3][2] = 0.0f;

	viewMatrix *= scaleMatrix;
	
	program.SetUniformMatrix4(1, &projMatrix[0][0]);
	program.SetUniformMatrix4(3, &viewMatrix[0][0]);


	//program.SetUniform(2, 8);
	
	glDrawArrays(GL_TRIANGLES, 0, index_vertices.size());

	glDepthMask(GL_TRUE);
}

void Skybox::CleanUp()
{
	glDeleteBuffers(1, &cubeMeshBuffer);
	glDeleteTextures(1, &skyboxTexture);

	glDeleteVertexArrays(1, &VAO);
	program.Delete();

	//delete cubeMesh;
	//cubeMesh = nullptr;
}

void Skybox::ActivateCubeMap()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
}

void Skybox::ReflectSkybox(float direction)
{
	scaleMatrix[1][1] = direction;
}
