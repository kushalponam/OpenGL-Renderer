#ifndef _MESHLOADER_H
#define _MESHLOADER_H

#include "Core.h"

#include<map>

class MeshLoader
{
public:

	bool LoadMesh(std::string ObjectPath);
	cy::TriMesh* GetMesh(std::string ObjectPath);
	~MeshLoader();

private:
	std::map<std::string, cy::TriMesh*> meshList;
};





#endif