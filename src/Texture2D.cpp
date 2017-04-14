#include "Texture2D.h"

TexLibrary *TexLibrary::t_Instance = nullptr;


TexLibrary::~TexLibrary()
{
	std::map<std::string, Texture2D*>::iterator it;
	for (it = lib_tex.begin(); it != lib_tex.end(); it++) {
		delete it->second;
	}



}

Texture2D* TexLibrary::GetTexture(std::string texName)
{
	if (lib_tex[texName]) {
		return lib_tex[texName];
	}
	
	glGenTextures(1, &TextureBuffers[texNumber]);
	
	glActiveTexture(GL_TEXTURE0 + texNumber);
	glBindTexture(GL_TEXTURE_2D, TextureBuffers[texNumber]);

	//Load the texture
	Texture2D *tex = new Texture2D(texName);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->GetTexWidth(), tex->GetTexHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &tex->GetTextureData()[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	lib_tex[texName] = tex;
	lib_id[texName] = texNumber;
	
	texNumber += 1;
	
	return lib_tex[texName];
}


Texture2D::Texture2D(std::string texName)
{
	std::string folderPath = INPUT_TEX_FOLDER + texName;
	//lodepng::load_file(buffer, folderPath);
	
	unsigned error = lodepng::decode(image, width, height, folderPath);

	if (error) {
		printf("Could not load %s texture", texName.c_str());
		return;
	}

	name = texName;
}

