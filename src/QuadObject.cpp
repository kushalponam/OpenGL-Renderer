#include "QuadObject.h"


bool CtrlPressed = false;

QuadObject::QuadObject()
{
	glGenVertexArrays(1, &VAO);
	BindVAO();

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glNamedBufferStorage(vertexBuffer, sizeof(QuadVertexData), QuadVertexData, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glNamedBufferStorage(normalBuffer, sizeof(QuadNormalData), QuadNormalData, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}



QuadObject::~QuadObject()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteVertexArrays(1, &VAO);
}

void QuadObject::Update()
{
	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void QuadObject::SetPosition(glm::vec3 translate)
{
	translationMatrix = glm::translate(translate) * translationMatrix;

}

void QuadObject::SetRotation(float radians, glm::vec3 axis)
{
	rotationMatrix = glm::rotate(radians, axis) * rotationMatrix;
}

void QuadObject::SetScale(glm::vec3 scale)
{
	scaleMatrix = glm::scale(scale) * scaleMatrix;
}

void QuadObject::SetMVPMatrix(glm::mat4 mvp)
{
	mvpMatrix = mvp;
}

void QuadObject::SetNormalMatrix(glm::mat4 nM)
{
	normalMatrix = nM;
}

