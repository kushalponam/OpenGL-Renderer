#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Core.h"
#include "Texture2D.h"

class Material {
public:
	Material();
	Material(cy::TriMesh::Mtl _mtl);
	~Material();
	glm::vec3 GetDiffuseColor() { return glm::vec3(mtl.Kd[0], mtl.Kd[1], mtl.Kd[2]);}
	glm::vec3 GetSpecularColor() { return glm::vec3(mtl.Ks[0], mtl.Ks[1], mtl.Ks[2]); }
	glm::vec3 GetAmbientColor() { return glm::vec3(mtl.Ka[0], mtl.Ka[1], mtl.Ka[2]); }
	
	void SetDiffuseColor(glm::vec3 color);

	void SetAlbedo(glm::vec3 i_albedo) { albedo = i_albedo; }
	void SetMetallic(float i_metallic) { metallic = i_metallic; }
	void SetRoughness(float i_roughness) { roughness = i_roughness; }

	glm::vec3 GetAlbedo() const { return albedo; }
	float GetMetallic() const { return metallic; }
	float GetRoughness() const { return roughness; }

	void LoadTexture(std::string texName);

	void LoadDiffuseTexture(std::string texName);
	void LoadSpecularTexture(std::string texName);

	Texture2D* GetDiffuseTexture() { return diffuseTexture; };
	Texture2D* GetSpecularTexture() { return specularTexture; };
	cy::TriMesh::Mtl GetMTL();

	void ActivateDiffuseTexture();
	void ActivateSpecularTexture();
	bool HasTexture() { return diffuseTexture != nullptr; }
private:
	cy::TriMesh::Mtl mtl;
	Texture2D *diffuseTexture;
	Texture2D *specularTexture;

	glm::vec3 albedo;
	float metallic;
	float roughness;


};





#endif
