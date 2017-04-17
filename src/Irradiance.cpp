#include "IrradianceMap.h"
#include "Scene.h"

Irradiance::Irradiance(int i_sourceMap, GLuint i_diffusewidth, GLuint i_diffuseHeight,GLuint i_specularWidth, GLuint i_specularHeight)
{
	sourceMap = i_sourceMap;
	diffuseWidth = i_diffusewidth;
	diffuseHeight = i_diffuseHeight;
	specularWidth = i_specularWidth;
	specularHeight = i_specularHeight;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenFramebuffers(1, &frameBuffer);
	glGenRenderbuffers(1, &renderBuffer);

	glActiveTexture(GL_TEXTURE5);
	glGenTextures(1, &diffusefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, diffusefilterMap);

	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, diffuseWidth, diffuseHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	
	glActiveTexture(GL_TEXTURE6);
	glGenTextures(1, &specularfilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, specularfilterMap);

	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, specularWidth, specularHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);


	glActiveTexture(GL_TEXTURE7);
	glGenTextures(1, &brdfFilterMap);
	glBindTexture(GL_TEXTURE_2D, brdfFilterMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glActiveTexture(GL_TEXTURE8);
	glGenTextures(1, &envMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envMap);

	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, 1024, 1024, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


}

void Irradiance::Init()
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

	projMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 50.0f);

	captureViews[0] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0, -1, 0));
	captureViews[1] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0, -1, 0));
	captureViews[2] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0, 0, 1));
	captureViews[3] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0, 0, -1));
	captureViews[4] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0, -1, 0));
	captureViews[5] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0, -1, 0));

	glm::vec3 camPos = glm::vec3(0, 0, 10);
	cameraView[0] = glm::lookAt(camPos, camPos+ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0, -1, 0));
	cameraView[1] = glm::lookAt(camPos, camPos+ glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0, -1, 0));
	cameraView[2] = glm::lookAt(camPos, camPos+ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0, 0, 1));
	cameraView[3] = glm::lookAt(camPos, camPos+ glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0, 0, -1));
	cameraView[4] = glm::lookAt(camPos, camPos+ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0, -1, 0));
	cameraView[5] = glm::lookAt(camPos, camPos+ glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0, -1, 0));
	
	glGenBuffers(1, &cubeMeshBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMeshBuffer);
	glNamedBufferStorage(cubeMeshBuffer, index_vertices.size() * sizeof(cy::Point3f), &index_vertices[0], 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}


void Irradiance::RenderCube()
{
	glDrawArrays(GL_TRIANGLES, 0, index_vertices.size());
}

void Irradiance::CaptureEnvironment()
{
	
	Scene *scene = Scene::getScene();
	Camera* camera = scene->GetCamera();
	

	std::vector<Shader*> envList = scene->GetEnvironmentList();

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevBuffer);
	glGetIntegerv(GL_VIEWPORT, prevViewport);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envMap);
	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);


	glViewport(0, 0, 1024, 1024);
	
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 50.0f);
	
	camera->SetProjectionMatrix(projection);
	//scene->ShadowPass();
	for (unsigned int i = 0; i < 6; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		camera->SetViewMatrix(cameraView[i]);
		
		for(unsigned int j=0;j<envList.size();j++)
		{
			envList[j]->Update();
			envList[j]->Render();
		}
	
	}
	//UnbindFrame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, prevBuffer);
	glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
}



// This should only run once. We are just pre computing irradiance
void Irradiance::FilterDiffuse()
{
	bool loaded = program.BuildFiles("src/Shaders/skyboxVertexShader.vert", "src/Shaders/irradiance.frag");
	if(!loaded)
	{
		printf("Could not load diffuse shader");
		return;
	}
	program.Bind();

	program.RegisterUniform(1, "projectionMatrix");
	program.RegisterUniform(2, "cubeMap");
	program.RegisterUniform(3, "viewMatrix");
	
	// SourceMap is where we have cubeMap texture 
	program.SetUniform(2, sourceMap);
	program.SetUniformMatrix4(1, &projMatrix[0][0]);
	
	
	glBindVertexArray(VAO);

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevBuffer);
	glGetIntegerv(GL_VIEWPORT, prevViewport);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, diffusefilterMap);

	// Bind FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	glViewport(0, 0 ,diffuseWidth, diffuseHeight);

	for (unsigned int i = 0; i<6; i++)
	{
		glm::mat4 view = captureViews[i];
		view[3][0] = 0.0f;
		view[3][1] = 0.0f;
		view[3][2] = 0.0f;

		program.SetUniformMatrix4(3, &view[0][0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, diffusefilterMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderCube();
	}

	//UnbindFrame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, prevBuffer);
	glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);

}

void Irradiance::FilterSpecular()
{
	bool loaded = program.BuildFiles("src/Shaders/skyboxVertexShader.vert", "src/Shaders/filterSpecular.frag");
	if (!loaded)
	{
		printf("Could not load diffuse shader");
		return;
	}
	program.Bind();

	program.RegisterUniform(1, "projectionMatrix");
	program.RegisterUniform(2, "cubeMap");
	program.RegisterUniform(3, "viewMatrix");
	program.RegisterUniform(4, "roughness");
	
	glBindVertexArray(VAO);
	
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevBuffer);
	glGetIntegerv(GL_VIEWPORT, prevViewport);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, specularfilterMap);

	// Bind FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	

	// SourceMap is where we have curbeMap texture 
	program.SetUniform(2, sourceMap);
	program.SetUniformMatrix4(1, &projMatrix[0][0]);

	unsigned int maxMipLevel = 5;
	for(unsigned int mip = 0 ; mip < maxMipLevel; ++mip)
	{
		unsigned int mipWidth = specularWidth * std::pow(0.5, mip);
		unsigned int mipHeight = specularHeight * std::pow(0.5, mip);

		glViewport(0, 0, mipWidth, mipHeight);
		float roughness = (float)mip / (float)(maxMipLevel - 1);

		program.SetUniform(4, roughness);
		for(unsigned int i=0; i<6; i++)
		{
			glm::mat4 view = captureViews[i];
			view[3][0] = 0.0f;
			view[3][1] = 0.0f;
			view[3][2] = 0.0f;

			program.SetUniformMatrix4(3, &view[0][0]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, specularfilterMap, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			RenderCube();
		}
	}
	//UnbindFrame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, prevBuffer);
	glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
}

void Irradiance::FilterBRDF()
{
	bool loaded = program.BuildFiles("src/Shaders/Default.vert", "src/Shaders/filterBRDF.frag");
	if (!loaded)
	{
		printf("Could not load diffuse shader");
		return;
	}
	program.Bind();

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevBuffer);
	glGetIntegerv(GL_VIEWPORT, prevViewport);
	
	quad = new RenderableObject(RenderableObject::Quad);
	quad->BindVAO();

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, brdfFilterMap);

	// Bind FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glViewport(0, 0, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfFilterMap, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, quad->GetVertexIndicesSize());

	//UnbindFrame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, prevBuffer);
	glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
}


void Irradiance::CleanUp()
{
	if(quad)
	{
		delete quad;
	}

	program.Delete();
	
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteRenderbuffers(1, &renderBuffer);
	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(1, &cubeMeshBuffer);
	glDeleteTextures(1, &diffusefilterMap);
	glDeleteTextures(1, &specularfilterMap);
	glDeleteTextures(1, &brdfFilterMap);
	glDeleteTextures(1, &envMap);
}
