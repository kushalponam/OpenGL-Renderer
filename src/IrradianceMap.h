#ifndef _IRRADIANCE_H
#define _IRRADIANCE_H

#include "Core.h"

class Irradiance:public GLObject
{
public:
	
	Irradiance(GLuint i_sourceMap, GLuint i_diffusewidth, GLuint i_diffuseheight,GLuint i_specularWidth,GLuint i_specularHeight);

	void Init();
	void Update(){};
	void Render(){};
	void CleanUp();
	void RenderCube();

	void FilterDiffuse();
	void FilterSpecular();
private:
	
	glm::mat4 captureViews[6];
	glm::mat4 projMatrix;
	std::vector<cy::Point3f> index_vertices;

	cy::GLSLProgram program;
	cy::TriMesh* cubeMesh;

	GLint prevViewport[4];
	GLint prevBuffer;
	GLuint frameBuffer;
	

	GLuint diffuseWidth;
	GLuint diffuseHeight;

	GLuint specularWidth;
	GLuint specularHeight;

	GLuint VAO;
	GLuint sourceMap;
	GLuint diffusefilterMap;
	GLuint specularfilterMap;

	GLuint cubeMeshBuffer;
	
	
};


#endif

