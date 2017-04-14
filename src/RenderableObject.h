#ifndef _RENDERABLE_OBJECT_H
#define _RENDERABLE_OBJECT_H

#include "Core.h"
#include "Material.h"
#include "QuadObject.h"




class RenderableObject:Object,GLUpdateObject{

public:
	enum ObjType
	{
		Quad,
		Triangle
	};
	RenderableObject(std::string name , bool useMTL = FALSE);
	RenderableObject(ObjType type);

	~RenderableObject();
	void SetTranslation(glm::vec3 pos);
	void SetRotation(float radians, glm::vec3 axis);
	void SetScale(glm::vec3 scale);
	void SetMVPMatrix(glm::mat4 mvp);
	void SetNormalMatrix(glm::mat3 normalMat);
	void Update();
	void ReflectObject(int direction);
	void BindVAO() { glBindVertexArray(VAO); }


	glm::vec3 GetPosition() const { return position; }
	glm::mat4 GetModelMatrix();
	GLuint GetUVBuffer();
	GLuint GetVertexBuffer();
	GLuint GetNormalBuffer();
	unsigned int GetVertexIndicesSize();

	glm::mat4 GetMVPMatrix();
	glm::mat3 GetNormalMatrix();

	Material* GetMaterial() const { return mat; }
	void SetMaterial(Material *i_mat) { mat = i_mat; }

private:
	enum BufferIndexes {
		Vertex_Buffer =0,
		Normal_Buffer =1,
		UV_Buffer=2
	};
	void LoadAllIndexes();

	float QuadVertexData[18] = {
		1.0f,  1.0f, 0.0f, // Right Top
		1.0f, -1.0f, 0.0f,  // Right Bottom
		-1.0f,  1.0f, 0.0f, // Left Top
		-1.0f,  1.0f, 0.0f, // Left Top
		1.0f, -1.0f, 0.0f,  // Right Bottom
		-1.0f, -1.0f, 0.0f, //Left Bottom
	};
	float QuadNormalData[18] = {
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
	};


	cy::TriMesh *Mesh;
	Material *mat;
	
	std::vector<cy::Point3f> index_vertices;
	std::vector<cy::Point3f> index_normals;
	std::vector<cy::Point3f> index_texCoords;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 modelMatrix;
	
	glm::mat4 normalMatrix;
	glm::mat4 mvpMatrix;
	glm::vec3 scale;

	glm::vec3 position;

	GLuint Buffers[3];
	GLuint VAO;
	
};



#endif // !_RENDERABLE_OBJECT_H

