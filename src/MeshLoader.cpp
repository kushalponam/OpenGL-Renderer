#include "MeshLoader.h"


bool MeshLoader::LoadMesh(std::string ObjectPath)
{
	cy::TriMesh* Mesh = new cy::TriMesh();

	std::string folderPath = INPUT_OBJ_FOLDER + ObjectPath;
	bool loaded = Mesh->LoadFromFileObj(folderPath.c_str(), false);
	if (!loaded) {
		printf("Could not load %s", ObjectPath.c_str());
		return loaded;
	}

	if (!Mesh->HasNormals()) Mesh->ComputeNormals();

	meshList[ObjectPath] = Mesh;

	return loaded;
}

cy::TriMesh* MeshLoader::GetMesh(std::string ObjectPath)
{
	if( meshList[ObjectPath]!=nullptr)
	{
		return meshList[ObjectPath];
	}
	
	LoadMesh(ObjectPath);

	return meshList[ObjectPath];
}

MeshLoader::~MeshLoader()
{
	std::map<std::string, cy::TriMesh*>::iterator it;
	for(it = meshList.begin(); it!= meshList.end();*it++)
	{
		cy::TriMesh *mesh = it->second;

		delete mesh;

	}


}
