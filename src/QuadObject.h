#ifndef _QUADOBJECT_H

#include "Core.h"


class QuadObject:Object,GLUpdateObject{

public:
	QuadObject();
	
	~QuadObject();

	void Update();
	
	void SetPosition(glm::vec3 translate);
	void SetRotation(float radians, glm::vec3 axis);
	void SetScale(glm::vec3 scale);
	void SetMVPMatrix(glm::mat4 mvp);
	void SetNormalMatrix(glm::mat4 nM);
	void BindVAO() { glBindVertexArray(VAO); }

	glm::mat4 GetMVPMatrix() { return mvpMatrix; }
	glm::mat4 GetModelMatrix() { return modelMatrix; }
	glm::mat4 GetNormalMatrix() { return normalMatrix; }
	
private:

	float QuadVertexData[18] = { 
		-0.5f, -0.5f, 0.0f, //Left Bottom
		0.5f, -0.5f, 0.0f,  // Right Bottom
		-0.5f,  0.5f, 0.0f, // Left Top
		-0.5f,  0.5f, 0.0f, // Left Top
		0.5f, -0.5f, 0.0f,  // Right Bottom
		0.5f,  0.5f, 0.0f, // Right Top
	};
	float QuadNormalData[18] = {
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
	};

	GLuint VAO;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	glm::mat4 modelMatrix;
	glm::mat4 normalMatrix;
	glm::mat4 mvpMatrix;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;

	

};



#endif // !_QUADOBJECT_H

