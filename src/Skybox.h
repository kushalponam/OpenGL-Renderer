#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "Core.h"

class Skybox:GLObject ,public KeyboardInput{

public:
	/*
	@parametes : folderPath is name of folder under which cube maps for this skybox are present
				 i_rightFaceName : CUBE_MAP_POSITIVE_X
				 i_leftFaceName : CUBE_MAP_NEGATIVE_X
				 i_topFaceName : CUBE_MAP_POSITIVE_Y
				 i_bottomFaceName : CUBE_MAP_NEGATIVE_Y
				 i_backFaceName : CUBE_MAP_POSITIVE_Z
				 i_frontFaceName : CUBE_MAP_NEGATIVE_Z
	*/
	Skybox(std::string folderPath,unsigned int imageWidth, unsigned int imageHeight, std::string i_rightFaceName,std::string i_leftFaceName, std::string i_topFaceName,
			std::string i_bottonFaceName, std::string i_backFaceName, std::string i_frontFaceName);


	GLuint GetTexture() { return skyboxTexture; }

	void Init();
	void Update();
	void Render();
	void CleanUp();

	void OnKeyDown(unsigned char key, int x, int y);
	void OnKeyUp(unsigned char key, int x, int y) {};
	void OnSpecialKeyDown(int key, int x, int y) {};
	void OnSpecialKeyUp(int key, int x, int y) {};

	void ActivateCubeMap();
	void ReflectSkybox(float direction);

private:

	GLuint VAO;
	GLuint skyboxTexture;
	std::string textureNames[6];
	cy::GLSLProgram program;

	glm::mat4 scaleMatrix;

	cy::TriMesh* cubeMesh;
	GLuint cubeMeshBuffer;
	std::vector<cy::Point3f> index_vertices;

};





#endif // !_SKYBOX_H

