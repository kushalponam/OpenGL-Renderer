#ifndef _IRRADIANCE_H
#define _IRRADIANCE_H

#include "Core.h"
#include "RenderableObject.h"

class Irradiance:public GLObject
{
public:
	
	Irradiance(int i_sourceMap, GLuint i_diffusewidth, GLuint i_diffuseheight,GLuint i_specularWidth,GLuint i_specularHeight);

	void Init();
	void Update(){};
	void Render(){};
	void CleanUp();
	void RenderCube();

	void CaptureEnvironment();
	
	void FilterDiffuse();
	void FilterSpecular();
	void FilterBRDF();
private:
	
	RenderableObject *quad;
	glm::mat4 captureViews[6];
	glm::mat4 cameraView[6];

	glm::mat4 projMatrix;
	std::vector<cy::Point3f> index_vertices;

	cy::GLSLProgram program;
	cy::TriMesh* cubeMesh;

	GLint prevViewport[4];
	GLint prevBuffer;
	GLuint frameBuffer;
	GLuint renderBuffer;

	GLuint diffuseWidth;
	GLuint diffuseHeight;

	GLuint specularWidth;
	GLuint specularHeight;

	GLuint VAO;
	int sourceMap;
	GLuint diffusefilterMap;
	GLuint specularfilterMap;
	GLuint brdfFilterMap;
	GLuint envMap;

	GLuint cubeMeshBuffer;
	
	
};


#endif

