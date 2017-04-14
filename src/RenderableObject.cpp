#include "Scene.h"
#include "RenderableObject.h"


void RenderableObject::LoadAllIndexes()
{
	//Load vertices
	for (unsigned int i = 0; i < Mesh->NF(); i++) {
		for (int j = 0; j < 3; j++) {
			int indice = Mesh->F(i).v[j];
			cy::Point3f vert = Mesh->V(indice);
			index_vertices.push_back(vert);
		}
	}

	//Load normals
	for (unsigned int i = 0; i < Mesh->NF(); i++) {
		for (int j = 0; j < 3; j++) {
			int indice = Mesh->FN(i).v[j];
			cy::Point3f vert = Mesh->VN(indice);
			index_normals.push_back(vert);
		}
	}

	if (!Mesh->HasTextureVertices()) return;

	//Load texcoords
	for (unsigned int i = 0; i < Mesh->NF(); i++) {
		for (int j = 0; j < 3; j++) {
			int indice = Mesh->FT(i).v[j];
			cy::Point3f vert = Mesh->VT(indice);
			index_texCoords.push_back(vert);
		}
	}
}

RenderableObject::RenderableObject(std::string ObjFileName,bool useMTL)
{
	
	Scene* scene = Scene::getScene();
    Mesh = scene->GetMeshLoader()->GetMesh(ObjFileName);

	LoadAllIndexes();

	/*if (useMTL) {
		mat = new Material(Mesh->M(0));
	}
	else {
		mat = new Material();
	}*/

	glGenVertexArrays(1, &VAO);
	BindVAO();

	//Create VertexBuffer and pass data to VertexShader
	glCreateBuffers(1, &Buffers[Vertex_Buffer]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Vertex_Buffer]);
	glNamedBufferStorage(Buffers[Vertex_Buffer], index_vertices.size() * sizeof(cy::Point3f), &index_vertices[0], 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	

	//Create NormalBuffer and pass data to VertexShader
	glCreateBuffers(1, &Buffers[Normal_Buffer]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Normal_Buffer]);
	glNamedBufferStorage(Buffers[Normal_Buffer], index_normals.size() * sizeof(cy::Point3f), &index_normals[0], 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	if (Mesh->HasTextureVertices()) {
		//Create UV buffer
		glCreateBuffers(1, &Buffers[UV_Buffer]);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[UV_Buffer]);
		glNamedBufferStorage(Buffers[UV_Buffer], index_texCoords.size() * sizeof(cy::Point3f), &index_texCoords[0], 0);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	

}

RenderableObject::RenderableObject(ObjType type)
{
	if(type == Quad)
	{
		glGenVertexArrays(1, &VAO);
		BindVAO();

		glGenBuffers(1, &Buffers[Vertex_Buffer]);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[Vertex_Buffer]);
		glNamedBufferStorage(Buffers[Vertex_Buffer], sizeof(QuadVertexData), QuadVertexData, 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glGenBuffers(1, &Buffers[Normal_Buffer]);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[Normal_Buffer]);
		glNamedBufferStorage(Buffers[Normal_Buffer], sizeof(QuadNormalData), QuadNormalData, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//mat = new Material();
	}
}

RenderableObject::~RenderableObject()
{
	/*if (Mesh) {
		delete Mesh;
	}*/
	/*if (mat) {
		delete mat;
	}*/
	glDeleteBuffers(1, &Buffers[Vertex_Buffer]);
	glDeleteBuffers(1, &Buffers[Normal_Buffer]);
	glDeleteBuffers(1, &Buffers[UV_Buffer]);
	glDeleteVertexArrays(1,&VAO);
}

void RenderableObject::SetTranslation(glm::vec3 pos)
{
	position = pos;
	translationMatrix = glm::translate(position);
}

void RenderableObject::SetRotation(float radians, glm::vec3 axis)
{
	rotationMatrix = glm::rotate(radians, axis) * rotationMatrix;
}

void RenderableObject::SetScale(glm::vec3 scaleAmount)
{
	scale = scaleAmount;
	scaleMatrix = glm::scale(scaleAmount) * scaleMatrix;
}
void RenderableObject::SetMVPMatrix(glm::mat4 mvp)
{
	mvpMatrix = mvp;
}

void RenderableObject::SetNormalMatrix(glm::mat3 normalMat)
{
	normalMatrix = normalMat;
}


void RenderableObject::Update()
{
	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void RenderableObject::ReflectObject(int direction)
{
	scaleMatrix[1][1] = direction * scale.y;
}

glm::mat4 RenderableObject::GetModelMatrix()
{
	return modelMatrix;
}

GLuint RenderableObject::GetUVBuffer()
{
	return Buffers[UV_Buffer];
}

GLuint RenderableObject::GetVertexBuffer()
{
	return Buffers[Vertex_Buffer];
}
GLuint RenderableObject::GetNormalBuffer()
{
	return Buffers[Normal_Buffer];
}

unsigned int RenderableObject::GetVertexIndicesSize()
{
	if(index_vertices.size()==0)
	{
		return 6;
	}
	return index_vertices.size();
}


glm::mat4 RenderableObject::GetMVPMatrix()
{
	return mvpMatrix;
}

glm::mat3 RenderableObject::GetNormalMatrix()
{
	return normalMatrix;
}

