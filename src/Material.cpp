#include "Material.h"


Material::Material()
{}

Material::Material(cy::TriMesh::Mtl _mtl)
{
	mtl = _mtl;
	if (mtl.map_Kd != NULL) {
		LoadDiffuseTexture(mtl.map_Kd);
	}
	if (mtl.map_Ks != NULL) {
		LoadSpecularTexture(mtl.map_Ks);
	}

}

Material::~Material()
{
	
}


void Material::SetDiffuseColor(glm::vec3 color)
{
	mtl.Kd[0] = color.x;
	mtl.Kd[1] = color.y;
	mtl.Kd[2] = color.z;
}

void Material::LoadDiffuseTexture(std::string texName)
{
	//diffuseTexture = TexLibrary::getInstance()->GetTexture(texName);
}

void Material::LoadSpecularTexture(std::string texName)
{
	//specularTexture = TexLibrary::getInstance()->GetTexture(texName);
}

cy::TriMesh::Mtl Material::GetMTL()
{
	return mtl;
}

void Material::UnBindAlbedo()
{
	TexLibrary *texLib = TexLibrary::getInstance();
	GLuint tex = texLib->GetTextureBuffer(albedoName);
}

void Material::ActivateDiffuseTexture()
{
	if (diffuseTexture == nullptr) return;
	GLuint texBuffer = TexLibrary::getInstance()->GetTextureBuffer(diffuseTexture->GetName());
	int texId = TexLibrary::getInstance()->GetTextureID(diffuseTexture->GetName());
	
	glActiveTexture (GL_TEXTURE0 + texId);
	glBindTexture(GL_TEXTURE_2D, texBuffer);

}

void Material::ActivateSpecularTexture()
{
	if (specularTexture == nullptr) return;
	GLuint texBuffer = TexLibrary::getInstance()->GetTextureBuffer(specularTexture->GetName());
	int texId = TexLibrary::getInstance()->GetTextureID(specularTexture->GetName());

	glActiveTexture(GL_TEXTURE0 + texId);
	glBindTexture(GL_TEXTURE_2D, texBuffer);
}
